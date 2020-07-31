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
#include <cmath>
#include <string>

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "mxmlTree2MsrOah.h"
#include "lpsr2LilypondOah.h"

#include "lpsr2LilypondTranslator.h"


using namespace std;

namespace MusicXML2
{

// for comments highlighting in the generated Lilypond code
const int commentFieldWidth = 30;

//______________________________________________________________________________
S_lpsrRepeatDescr lpsrRepeatDescr::create (
  S_msrRepeat repeat,
  int         repeatEndingsNumber)
{
  lpsrRepeatDescr* o = new
    lpsrRepeatDescr (
      repeat,
      repeatEndingsNumber);
  assert(o!=0);
  return o;
}

lpsrRepeatDescr::lpsrRepeatDescr (
  S_msrRepeat repeat,
  int         repeatEndingsNumber)
{
  fRepeat = repeat;

  fRepeatEndingsNumber  = repeatEndingsNumber;
  fRepeatEndingsCounter = 0;

  fEndOfRepeatHasBeenGenerated = false;
}

lpsrRepeatDescr::~lpsrRepeatDescr ()
{}

string lpsrRepeatDescr::repeatDescrAsString () const
{
  stringstream s;

  s <<
    "fRepeatEndingsNumber = " <<
    fRepeatEndingsNumber <<
    ", fRepeatEndingsCounter = " <<
    fRepeatEndingsCounter <<
    ", fEndOfRepeatHasBeenGenerated = " <<
    booleanAsString (
      fEndOfRepeatHasBeenGenerated);

  return s.str ();
}

void lpsrRepeatDescr::print (ostream& os) const
{
  const int fieldWidth = 29;

  os << left <<
    setw (fieldWidth) <<
    "fRepeatEndingsNumber" << " : " <<
    fRepeatEndingsNumber <<
    endl <<
    setw (fieldWidth) <<
    "fRepeatEndingsCounter" << " : " <<
    fRepeatEndingsCounter <<
    endl <<
    setw (fieldWidth) <<
    "fEndOfRepeatHasBeenGenerated" << " : " <<
    fEndOfRepeatHasBeenGenerated <<
    endl;
}

ostream& operator<< (ostream& os, const S_lpsrRepeatDescr& elt)
{
  elt->print (os);
  return os;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::setCurrentOctaveEntryReferenceFromTheLilypondOah ()
{
  if (gLpsr2LilypondOah->fRelativeOctaveEntrySemiTonesPitchAndOctave) {
    // option '-rel, -relative' has been used:
    fCurrentOctaveEntryReference =
      msrNote::createNoteFromSemiTonesPitchAndOctave (
        K_NO_INPUT_LINE_NUMBER,
        gLpsr2LilypondOah->fRelativeOctaveEntrySemiTonesPitchAndOctave);
  }
  else {
    fCurrentOctaveEntryReference = nullptr;
    // the first note in the voice will become the initial reference
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesOctaveEntry) {
    fLogOutputStream <<
      "setCurrentOctaveEntryReferenceFromTheLilypondOah()" <<
      ", octaveEntryKind is" <<
      lpsrOctaveEntryKindAsString (gLpsr2LilypondOah->fOctaveEntryKind) <<
      endl <<
      "Initial fCurrentOctaveEntryReference is ";

    if (fCurrentOctaveEntryReference) {
      fLogOutputStream <<
        "'" <<
        fCurrentOctaveEntryReference->asString () <<
        "'";
    }
    else {
      fLogOutputStream << "none";
    }
    fLogOutputStream << endl;
  }
#endif
}

//________________________________________________________________________
lpsr2LilypondTranslator::lpsr2LilypondTranslator (
  S_lpsrScore lpsrScore,
  S_msrOah&   msrOpts,
  S_lpsrOah&  lpsrOpts,
  ostream&    logOstream,
  ostream&    lilypondCodeOstream)
    : fLogOutputStream (
        logOstream),
      fLilypondCodeOstream (
        lilypondCodeOstream)
{
  fMsrOah  = msrOpts;
  fLpsrOah = lpsrOpts;

  // the LPSR score we're visiting
  fVisitedLpsrScore = lpsrScore;

  // inhibit the browsing of measures repeats replicas,
  // since Lilypond only needs the repeat measure
  fVisitedLpsrScore->
    getMsrScore ()->
      setInhibitMeasuresRepeatReplicasBrowsing ();

/* JMI
  // inhibit the browsing of measures repeat replicas,
  // since Lilypond only needs the measure number
  fVisitedLpsrScore->
    getMsrScore ()->
      setInhibitRestMeasuresBrowsing ();
*/

  // header handling
  fOnGoingHeader = false;

  // time
  fVoiceIsCurrentlySenzaMisura = false;
  fOnGoingVoiceCadenza = false;

  // staves
  fOnGoingStaff = false;

  // voices
  fOnGoingVoice = false;

  // octaves entry
  // ------------------------------------------------------
  /* initialize reference is:
        mobile in relative mode
        unused in absolute mode
        fixed  in fixed mode
  */
  switch (gLpsr2LilypondOah->fOctaveEntryKind) {
    case kOctaveEntryRelative:
      setCurrentOctaveEntryReferenceFromTheLilypondOah ();
      break;

    case kOctaveEntryAbsolute:
      fCurrentOctaveEntryReference = nullptr;
      break;

    case kOctaveEntryFixed:
      // sanity check
      msrAssert (
        gLpsr2LilypondOah->fFixedOctaveEntrySemiTonesPitchAndOctave != nullptr,
        "gLpsr2LilypondOah->fFixedOctaveEntrySemiTonesPitchAndOctave is null");

      fCurrentOctaveEntryReference =
        msrNote::createNoteFromSemiTonesPitchAndOctave (
          K_NO_INPUT_LINE_NUMBER,
          gLpsr2LilypondOah->fFixedOctaveEntrySemiTonesPitchAndOctave);
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesOctaveEntry) {
    fLogOutputStream <<
      "lpsr2LilypondTranslator()" <<
      ", octaveEntryKind is" <<
      lpsrOctaveEntryKindAsString (gLpsr2LilypondOah->fOctaveEntryKind) <<
      endl <<
      "Initial fCurrentOctaveEntryReference is ";

    if (fCurrentOctaveEntryReference) {
      fLogOutputStream <<
        "'" <<
        fCurrentOctaveEntryReference->asString () <<
        "'";
    }
    else {
      fLogOutputStream << "none";
    }
    fLogOutputStream << endl;
  }
#endif

  // harmonies
  fOnGoingHarmonyVoice = false;
  fPowerChordHaveAlreadyBeenGenerated = false;

  // figured bass
  fOnGoingFiguredBassVoice = false;
  fCurrentFiguredBassFiguresCounter = 0;

  // repeats

  // rest measures
  fRemainingRestMeasuresNumber = 0;
  fOnGoingRestMeasures = false;

  // measures
  fCurrentVoiceMeasuresCounter = -1;

  // durations
  fLastMetWholeNotes = rational (0, 1);

  // notes
  fCurrentNotePrinObjectKind = kPrintObjectYes; // default value

  // grace notes
  fOnGoingGraceNotesGroup = false;

  // articulations
  fCurrentArpeggioDirectionKind = kDirectionNone;

  // stems
  fCurrentStemKind = msrStem::kStemNone;

  // double tremolos

  // chords
  fOnGoingChord = false;

  // trills
  fOnGoingTrillSpanner = false;

  // spanners
  fCurrentSpannerPlacementKind = kPlacementNone;

  // stanzas
  fGenerateCodeForOngoingNonEmptyStanza = false;

  // book blocks
  fOnGoingBookBlock = false;

  // book part blocks
  fOnGoingBookPartBlock = false;

  // score blocks
  fOnGoingScoreBlock = false;

  // part group blocks
  fNumberOfPartGroupBlocks = -1;
  fPartGroupBlocksCounter  = 0;

  // part blocks
  fNumberOfPartGroupBlockElements = -1;
  fPartGroupBlockElementsCounter  = 0;

  // staff blocks
  fNumberOfStaffBlocksElements = -1;
  fStaffBlocksCounter  = 0;
};

lpsr2LilypondTranslator::~lpsr2LilypondTranslator ()
{}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateLilypondCodeFromLpsrScore ()
{
  if (fVisitedLpsrScore) {
    // browse a msrScore browser
    msrBrowser<lpsrScore> browser (this);
    browser.browse (*fVisitedLpsrScore);
  }
}

//________________________________________________________________________
string lpsr2LilypondTranslator::absoluteOctaveAsLilypondString (
  int absoluteOctave)
{
  string result;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    fLilypondCodeOstream <<
      endl <<
      "%{ absoluteOctave = " << absoluteOctave << " %} " <<
      endl;
  }
#endif

  // generate LilyPond absolute octave
  switch (absoluteOctave) {
    case 0:
      result = ",,,";
      break;
    case 1:
      result = ",,";
      break;
    case 2:
      result = ",";
      break;
    case 3:
      result = "";
      break;
    case 4:
      result = "'";
      break;
    case 5:
      result = "''";
      break;
    case 6:
      result = "'''";
      break;
    case 7:
      result = "''''";
      break;
    case 8:
      result = "'''''";
      break;
    default:
      {
        /* JMI
        stringstream s;

        s <<
          "%{absolute octave " << absoluteOctave << "???%}";

        result = s.str ();
        */
      }
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::alterationKindAsLilypondString (
  msrAlterationKind alterationKind)
{
  string result;

  switch (alterationKind) {
    case kTripleFlat:
      result = "TRIPLE-FLAT";
      break;
    case kDoubleFlat:
      result = "DOUBLE-FLAT";
      break;
    case kSesquiFlat:
      result = "THREE-Q-FLAT";
      break;
    case kFlat:
      result = "FLAT";
      break;
    case kSemiFlat:
      result = "SEMI-FLAT";
      break;
    case kNatural:
      result = "NATURAL";
      break;
    case kSemiSharp:
      result = "SEMI-SHARP";
      break;
    case kSharp:
      result = "SHARP";
      break;
    case kSesquiSharp:
      result = "THREE-Q-SHARP";
      break;
    case kDoubleSharp:
      result = "DOUBLE-SHARP";
      break;
    case kTripleSharp:
      result = "TRIPLE-SHARP";
      break;
    case k_NoAlteration:
      result = "alteration???";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::lilypondOctaveInRelativeEntryMode (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // generate LilyPond octave relative to fCurrentOctaveEntryReference

  // in MusicXML, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();

  msrDiatonicPitchKind
    noteDiatonicPitchKind =
      note->
        noteDiatonicPitchKind (
          inputLineNumber);

  msrDiatonicPitchKind
    referenceDiatonicPitchKind =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKind (
          inputLineNumber);

  string
    referenceDiatonicPitchKindAsString =
      fCurrentOctaveEntryReference->
        noteDiatonicPitchKindAsString (
          inputLineNumber);

  int
    referenceAbsoluteOctave =
      fCurrentOctaveEntryReference->
        getNoteOctave ();

  /*
    If no octave changing mark is used on a pitch, its octave is calculated
    so that the interval with the previous note is less than a fifth.
    This interval is determined without considering accidentals.
  */

  int
    noteAboluteDiatonicOrdinal =
      noteAbsoluteOctave * 7
        +
      noteDiatonicPitchKind - kC,

    referenceAboluteDiatonicOrdinal =
      referenceAbsoluteOctave * 7
        +
      referenceDiatonicPitchKind - kC;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesOctaveEntry) {
    const int fieldWidth = 28;

    fLogOutputStream << left <<
      "lilypondOctaveInRelativeEntryMode() 1" <<
      endl <<

      setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal <<
      endl <<

      setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString" <<
      " = " <<
      referenceDiatonicPitchKindAsString <<
      endl <<
      setw (fieldWidth) <<
      "% referenceAbsoluteOctave" <<
       " = " <<
      referenceAbsoluteOctave <<
      endl <<
      setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      " = " <<
      referenceAboluteDiatonicOrdinal <<
      endl <<
      endl;
  }
#endif

  stringstream s;

  // generate the octaves as needed
  if (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
    noteAboluteDiatonicOrdinal -= 4;
    while (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
      s << "'";
      noteAboluteDiatonicOrdinal -= 7;
    } // while
  }

  else {
    noteAboluteDiatonicOrdinal += 4;
    while (noteAboluteDiatonicOrdinal <= referenceAboluteDiatonicOrdinal) {
      s << ",";
      noteAboluteDiatonicOrdinal += 7;
    } // while
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesOctaveEntry) {
    fLogOutputStream <<
      "lilypondOctaveInRelativeEntryMode() 2" <<
      ", result = " << s.str () <<
      endl <<
      endl;
  }
#endif

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::lilypondOctaveInFixedEntryMode (
  S_msrNote note)
{
  // generate LilyPond octave relative to fCurrentOctaveEntryReference

  // in MusicXML, octave number is 4 for the octave starting with middle C

  /*
    Pitches in fixed octave entry mode only need commas or quotes
    when they are above or below fCurrentOctaveEntryReference,
    hence when their octave is different that the latter's.
  */

  int
    noteAbsoluteOctave =
      note->getNoteOctave ();

  int
    referenceAbsoluteOctave =
      fCurrentOctaveEntryReference->
        getNoteOctave ();

  int absoluteOctavesDifference =
    noteAbsoluteOctave - referenceAbsoluteOctave;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesOctaveEntry) {
    fLogOutputStream << left <<
      "% noteAbsoluteOctave = " <<
      noteAbsoluteOctave <<
      ", referenceAbsoluteOctave = " <<
      referenceAbsoluteOctave <<
      ", referenceAbsoluteOctave = " <<
      absoluteOctavesDifference <<
      endl;
  }
#endif

  stringstream s;

  // generate the octaves as needed
  switch (absoluteOctavesDifference) {
    case -12:
      s << ",,,,,,,,,,,,";
      break;
    case -11:
      s << ",,,,,,,,,,,";
      break;
    case -10:
      s << ",,,,,,,,,,";
      break;
    case -9:
      s << ",,,,,,,,,";
      break;
    case -8:
      s << ",,,,,,,,";
      break;
    case -7:
      s << ",,,,,,,";
      break;
    case -6:
      s << ",,,,,,";
      break;
    case -5:
      s << ",,,,,";
      break;
    case -4:
      s << ",,,,";
      break;
    case -3:
      s << ",,,";
      break;
    case -2:
      s << ",,";
      break;
    case -1:
      s << ",";
      break;
    case 0:
      break;
    case 1:
      s << "'";
      break;
    case 2:
      s << "''";
      break;
    case 3:
      s << "'''";
      break;
    case 4:
      s << "''''";
      break;
    case 5:
      s << "'''''";
      break;
    case 6:
      s << "''''''";
      break;
    case 7:
      s << "'''''''";
      break;
    case 8:
      s << "''''''''";
      break;
    case 9:
      s << "'''''''''";
      break;
    case 10:
      s << "''''''''''";
      break;
    case 11:
      s << "'''''''''''";
      break;
    case 12:
      s << "''''''''''''";
      break;
    default:
      s << "!!!";
  } // switch

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::stringTuningAsLilypondString (
  int               inputLineNumber,
  S_msrStringTuning stringTuning)
{
  msrDiatonicPitchKind
    stringTuningDiatonicPitchKind =
      stringTuning->
        getStringTuningDiatonicPitchKind ();

  msrAlterationKind
    stringTuningAlterationKind =
      stringTuning->
       getStringTuningAlterationKind ();

  int
    stringTuningOctave =
      stringTuning->
       getStringTuningOctave ();

  // compute the quartertones pitch
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromDiatonicPitchAndAlteration (
        inputLineNumber,
        stringTuningDiatonicPitchKind,
        stringTuningAlterationKind);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceScordaturas) {
    int
      getStringTuningNumber =
        stringTuning->
          getStringTuningNumber ();

    fLogOutputStream <<
      endl <<
      "%getStringTuningNumber = " <<
      getStringTuningNumber <<
      endl <<
      "%stringTuningDiatonicPitchKind = " <<
      msrDiatonicPitchKindAsString (
        stringTuningDiatonicPitchKind) <<
      endl <<
      "%stringTuningAlterationKind = " <<
      alterationKindAsLilypondString (
        stringTuningAlterationKind) <<
      endl <<
      "%stringTuningOctave = " <<
      stringTuningOctave <<
      endl <<
      "%quarterTonesPitchKind = " <<
      quarterTonesPitchKind <<
      endl <<
      "%quarterTonesPitchKindAsString: " <<
      msrQuarterTonesPitchKindAsString (
        gLpsrOah->
          fLpsrQuarterTonesPitchesLanguageKind,
          quarterTonesPitchKind) <<
      endl <<
      endl;
  }
#endif

  stringstream s;

  s <<
    msrQuarterTonesPitchKindAsString (
      gLpsrOah->
        fLpsrQuarterTonesPitchesLanguageKind,
        quarterTonesPitchKind) <<
    absoluteOctaveAsLilypondString (
      stringTuningOctave);

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::notePitchAsLilypondString (
  S_msrNote note)
{
  stringstream s;

  // should an editorial accidental be generated?
  switch (note->getNoteEditorialAccidentalKind ()) {
    case kEditorialAccidentalYes:
      s <<
        "\\editorialAccidental ";
      break;
    case kEditorialAccidentalNo:
      break;
  } // switch

  // get the note quarter tones pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      note->
        getNoteQuarterTonesPitchKind ();

  // fetch the quarter tones pitch as string
  string
    quarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
        noteQuarterTonesPitchKind);

  // get the note quarter tones display pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesDisplayPitchKind =
      note->
        getNoteQuarterTonesDisplayPitchKind ();

  // fetch the quarter tones display pitch as string
  string
    quarterTonesDisplayPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
        noteQuarterTonesDisplayPitchKind);

  // generate the pitch
  s <<
    quarterTonesPitchKindAsString;

  // in MusicXML, octave number is 4 for the octave
  // starting with middle C, LilyPond's c'
  int
    noteAbsoluteOctave =
      note->getNoteOctave ();

  // should an absolute octave be generated?
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesOctaveEntry) {
    int
      noteAbsoluteDisplayOctave =
        note->getNoteDisplayOctave ();

    const int fieldWidth = 39;

    fLogOutputStream << left <<
      "notePitchAsLilypondString() 1" <<
      endl <<

      setw (fieldWidth) <<
      "% quarterTonesPitchKindAsString" <<
      " = " <<
      quarterTonesPitchKindAsString <<
      endl <<
      setw (fieldWidth) <<
      "% quarterTonesDisplayPitchKindAsString" <<
      " = " <<
      quarterTonesDisplayPitchKindAsString <<
      endl <<

      setw (fieldWidth) <<
      "% noteAbsoluteOctave" <<
      " = " <<
      noteAbsoluteOctave <<
      endl <<
      setw (fieldWidth) <<
      "% noteAbsoluteDisplayOctave" <<
      " = " <<
      noteAbsoluteDisplayOctave <<
      endl <<

      setw (fieldWidth) <<
      "% line" <<
      " = " <<
      note->getInputLineNumber () <<
      endl;
  }
#endif

  switch (gLpsr2LilypondOah->fOctaveEntryKind) {
    case kOctaveEntryRelative:
      if (! fCurrentOctaveEntryReference) {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceNotesOctaveEntry) {
          fLogOutputStream <<
            "notePitchAsLilypondString() 2: fCurrentOctaveEntryReference is null" <<
            " upon note " << note->asString () <<
            ", line " << note->getInputLineNumber () <<
            endl;
        }
#endif

        // generate absolute octave
        s <<
          absoluteOctaveAsLilypondString (
            noteAbsoluteOctave);

        // fCurrentOctaveEntryReference will be set to note later
      }
      else {
#ifdef TRACE_OAH
        if (gTraceOah->fTraceNotesOctaveEntry) {
          fLogOutputStream <<
            "notePitchAsLilypondString() 3: fCurrentOctaveEntryReference is '" <<
            fCurrentOctaveEntryReference->asString () <<
            "' upon note " << note->asString () <<
            ", line " << note->getInputLineNumber () <<
            endl;
        }
#endif

        // generate octave relative to mobile fCurrentOctaveEntryReference
        s <<
          lilypondOctaveInRelativeEntryMode (note);
      }
      break;

    case kOctaveEntryAbsolute:
      // generate LilyPond absolute octave
      s <<
        absoluteOctaveAsLilypondString (
          noteAbsoluteOctave);
      break;

    case kOctaveEntryFixed:
      // generate octave relative to fixed fCurrentOctaveEntryReference
      s <<
        lilypondOctaveInFixedEntryMode (note);
      break;
  } // switch

  // should an accidental be generated? JMI this can be fine tuned with cautionary
  switch (note->getNoteAccidentalKind ()) {
    case kAccidentalNone:
      break;
    default:
      s <<
        "!";
      break;
  } // switch

  // should an editorial accidental be generated?
  switch (note->getNoteEditorialAccidentalKind ()) {
    case kEditorialAccidentalYes:
      s <<
        "?";
      break;
    case kEditorialAccidentalNo:
      break;
  } // switch

  // should a cautionary accidental be generated?
  switch (note->getNoteCautionaryAccidentalKind ()) {
    case kCautionaryAccidentalYes:
      s <<
        "?";
      break;
    case kCautionaryAccidentalNo:
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesOctaveEntry) {
    fLogOutputStream << endl;
  }
#endif

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::durationAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes)
{
  string result;

  bool generateExplicitDuration;

  if (wholeNotes != fLastMetWholeNotes) {
    generateExplicitDuration = true;
    fLastMetWholeNotes = wholeNotes;
  }
  else {
    generateExplicitDuration =
      gLpsr2LilypondOah->fAllDurations;
  }

  if (generateExplicitDuration) {
    result =
      wholeNotesAsLilypondString (
        inputLineNumber,
        wholeNotes);
  }

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::pitchedRestAsLilypondString (
  S_msrNote note)
{
  stringstream s;

  // get the note quarter tones pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesPitchKind =
      note->
        getNoteQuarterTonesPitchKind ();

  // fetch the quarter tones pitch as string
  string
    noteQuarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
        noteQuarterTonesPitchKind);

  // get the note quarter tones display pitch
  msrQuarterTonesPitchKind
    noteQuarterTonesDisplayPitchKind =
      note->
        getNoteQuarterTonesDisplayPitchKind ();

  // fetch the quarter tones display pitch as string
  string
    quarterTonesDisplayPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
        noteQuarterTonesDisplayPitchKind);

  // generate the display pitch
  s <<
    note->
      noteDisplayPitchKindAsString ();
//    note->notePitchAsString (); JMI
//    quarterTonesDisplayPitchAsString;

  // should an absolute octave be generated?
  int
    noteAbsoluteDisplayOctave =
      note->getNoteDisplayOctave ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    // in MusicXML, octave number is 4 for the octave starting with middle C
    int noteAbsoluteOctave =
      note->getNoteOctave ();

    const int fieldWidth = 28;

    fLogOutputStream << left <<
      "pitchedRestAsLilypondString()" <<
      endl <<

      setw (fieldWidth) <<
      "% noteQuarterTonesPitchKindAsString" <<
      " = " <<
      noteQuarterTonesPitchKindAsString <<
      endl <<
      setw (fieldWidth) <<
      "% quarterTonesDisplayPitch" <<
      " = " <<
      quarterTonesDisplayPitchKindAsString <<
      endl <<

      setw (fieldWidth) <<
      "% noteAbsoluteOctave" <<
      " = " <<
      noteAbsoluteOctave <<
      endl <<
      setw (fieldWidth) <<
      "% noteAbsoluteDisplayOctave" <<
      " = " <<
      noteAbsoluteDisplayOctave <<
      endl <<

      setw (fieldWidth) <<
      "% line" <<
      " = " <<
      note->getInputLineNumber () <<
      endl;
  }
#endif

  switch (gLpsr2LilypondOah->fOctaveEntryKind) {
    case kOctaveEntryRelative:
    // generate LilyPond octave relative to fCurrentOctaveEntryReference
    s <<
      lilypondOctaveInRelativeEntryMode (note);
      break;
    case kOctaveEntryAbsolute:
      // generate LilyPond absolute octave
      s <<
        absoluteOctaveAsLilypondString (
          noteAbsoluteDisplayOctave);
      break;
    case kOctaveEntryFixed:
      // generate LilyPond octave relative to fCurrentOctaveEntryReference
      s <<
        lilypondOctaveInFixedEntryMode (note);
      break;
  } // switch

  // generate the skip duration
  s <<
    durationAsLilypondString (
      note->getInputLineNumber (),
      note->
        getNoteSoundingWholeNotes ());

  // generate the '\rest'
  s <<
    "\\rest ";

  return s.str ();
}

void lpsr2LilypondTranslator::generateNoteHeadColor (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // note color, unofficial ??? JMI
  msrAlphaRGBColor
    noteColor =
      note->getNoteColor ();

  if (! noteColor.isEmpty ()) { // JMI
//  if (gLpsr2LilypondOah->) { // JMI
    // generate code for color RGB
    string noteRGB = noteColor.getColorRGB ();

    if (noteRGB.size () == 6) {
      string
        noteR = noteRGB.substr (0, 2),
        noteG = noteRGB.substr (2, 2),
        noteB = noteRGB.substr (4, 2);

       // \once \override NoteHead.color = #(map (lambda (x) (/ x 255)) '(#X00 #X00 #XFF))

        fLilypondCodeOstream <<
          "\\once \\override NoteHead.color = #(map (lambda (x) (/ x 255)) "
          "'(" <<
          "#X" << noteRGB [0] << noteRGB [1] <<
          " " <<
          "#X" << noteRGB [2] << noteRGB [3] <<
          " " <<
          "#X" << noteRGB [4] << noteRGB [5] <<
          "))" <<
          endl;
      }
    else {
      stringstream s;

      s <<
        "note RGB color '" <<
        noteRGB <<
        "' is ill-formed" <<
        ", line " << inputLineNumber;

      msrInternalError (
        gOahOah->fInputSourceName,
        inputLineNumber,
        __FILE__, __LINE__,
        s.str ());
    }

    // ignore color alpha
  }
}

void lpsr2LilypondTranslator::generateNoteLigatures (
  S_msrNote note)
{
  list<S_msrLigature>
    noteLigatures =
      note->getNoteLigatures ();

  if (noteLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin ();
      i!=noteLigatures.end ();
      i++
    ) {
      S_msrLigature ligature = (*i);

      switch (ligature->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;

        case msrLigature::kLigatureStart:
          {
            /*
              the edge height is relative to the voice,
              i.e. a positive value points down in voices 1 and 3
              and it points up in voices 2 and 4,
            */

            // fetch note's voice
            S_msrVoice
              noteVoice =
                note->
                  getNoteMeasureUpLink ()->
                    getMeasureSegmentUpLink ()->
                      getSegmentVoiceUpLink ();

            // determine vertical flipping factor
            int ligatureVerticalFlippingFactor = 0;

            switch (noteVoice->getRegularVoiceStaffSequentialNumber ()) {
              case 1:
              case 3:
                ligatureVerticalFlippingFactor = 1;
                break;
              case 2:
              case 4:
                ligatureVerticalFlippingFactor = -1;
                break;
              default:
                ;
            } // switch

#ifdef TRACE_OAH
            if (gTraceOah->fTraceLigatures) {
              fLogOutputStream <<
                "Ligature vertical flipping factore for note '" <<
                note->asString () <<
                "' in voice \"" <<
                noteVoice->getVoiceName () <<
                "\" is " <<
                ligatureVerticalFlippingFactor <<
                ", line " << ligature->getInputLineNumber () <<
                endl;
            }
#endif

            // compute ligature start edge height
            const float edgeHeightAbsValue = 0.75;

            float       ligatureStartEdgeHeight = 0.0;

            switch (ligature->getLigatureLineEndKind ()) {
              case msrLigature::kLigatureLineEndUp:
                ligatureStartEdgeHeight =
                  - ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigature::kLigatureLineEndDown:
                ligatureStartEdgeHeight =
                  ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigature::kLigatureLineEndBoth: // JMI
                ligatureStartEdgeHeight =
                  - ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigature::kLigatureLineEndArrow: // JMI
                fLilypondCodeOstream <<
                  "%{ligatureLineEndArrow???%} ";
                break;

              case msrLigature::kLigatureLineEndNone:
                ligatureStartEdgeHeight = 0;
                break;
            } // switch

            // fetch ligature's other end
            S_msrLigature
              ligatureOtherEnd =
                ligature->
                  getLigatureOtherEndSideLink ();

            // compute ligature end edge height
            float ligatureEndEdgeHeight = 0.0;

            switch (ligatureOtherEnd->getLigatureLineEndKind ()) {
              case msrLigature::kLigatureLineEndUp:
                ligatureEndEdgeHeight =
                  - ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigature::kLigatureLineEndDown:
                ligatureEndEdgeHeight =
                  ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigature::kLigatureLineEndBoth: // JMI
                ligatureEndEdgeHeight =
                  - ligatureVerticalFlippingFactor * edgeHeightAbsValue;
                break;

              case msrLigature::kLigatureLineEndArrow: // JMI
                fLilypondCodeOstream <<
                  "%{ligatureLineEndArrow???%} ";
                break;

              case msrLigature::kLigatureLineEndNone:
                ligatureEndEdgeHeight = 0;
                break;
            } // switch

            // generate the code the the edge-height pair of values
            fLilypondCodeOstream <<
              endl <<
              "\\once \\override Staff.LigatureBracket.edge-height = #'(" <<
              setprecision (2) <<
              ligatureStartEdgeHeight <<
              " . " <<
              setprecision (2) <<
              ligatureEndEdgeHeight <<
              ")" <<
              " %{ " <<
              ligature->getInputLineNumber () <<
              " %}" <<
              endl;
          }

          // generate ligature line type if any
          switch (ligature->getLigatureLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override LigatureBracket.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override LigatureBracket.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override LigatureBracket.style = #'zigzag" <<
                endl;
              break;
          } // switch

          fLilypondCodeOstream << "\\[ ";
          break;

        case msrLigature::kLigatureContinue:
          break;

        case msrLigature::kLigatureStop:
   // JMI       fLilypondCodeOstream << "\\] ";
          break;
      } // switch
    } // for
  }
}

string lpsr2LilypondTranslator::stemAsLilypondString (
  msrStem::msrStemKind stemKind)
{
  string result;

  switch (stemKind) {
    case msrStem::kStemNone:
      result = "\\stemNeutral ";
      break;
    case msrStem::kStemUp:
      result = "\\stemUp ";
      break;
    case msrStem::kStemDown:
      result = "\\stemDown ";
      break;
    case msrStem::kStemDouble: // JMI ???
      break;
  } // switch

  return result;
}

void lpsr2LilypondTranslator::generateNoteStemIfNeeded (
  S_msrStem stem)
{
  // the LilyPond \stem* commands have a persistent effect,
  // hence the use of fCurrentStemKind

  if (stem) {
    msrStem::msrStemKind
      stemKind =
        stem->getStemKind ();

    // should a stem direction command be generated?
    if (stemKind != fCurrentStemKind) {
      fLilypondCodeOstream <<
        stemAsLilypondString (stemKind);

      fCurrentStemKind = stemKind;
    }
  }

  else {
//    msrStem::msrStemKind noteStemKind = msrStem::kStemNone;  // JMI
  }
}

void lpsr2LilypondTranslator::generateNoteHead (
  S_msrNote note)
{
  if (! note->getNoteIsARest ()) { // JMI ???
    msrNote::msrNoteHeadKind
      noteHeadKind =
        note->getNoteHeadKind ();

    // these tweaks should occur right before the note itself
    switch (noteHeadKind) {
      case msrNote::kNoteHeadSlash:
        fLilypondCodeOstream << "\\tweak style #'slash ";
        break;
      case msrNote::kNoteHeadTriangle:
        fLilypondCodeOstream << "\\tweak style #'triangle ";
        break;
      case msrNote::kNoteHeadDiamond:
   // JMI     fLilypondCodeOstream << "\\tweak style #'diamond ";
        fLilypondCodeOstream << "\\harmonic ";
        break;
      case msrNote::kNoteHeadSquare:
        fLilypondCodeOstream << "\\tweak style #'la ";
        break;
      case msrNote::kNoteHeadCross:
        fLilypondCodeOstream << "\\tweak style #'cross ";
        break;
      case msrNote::kNoteHeadX:
        fLilypondCodeOstream << "\\tweak style #'cross %{x%} ";
        break;
      case msrNote::kNoteHeadCircleX:
        fLilypondCodeOstream << "\\tweak style #'xcircle ";
        break;
      case msrNote::kNoteHeadInvertedTriangle:
        fLilypondCodeOstream << "%{kNoteHeadInvertedTriangle%} ";
        break;
      case msrNote::kNoteHeadArrowDown:
        fLilypondCodeOstream << "%{kNoteHeadArrowDown%} ";
        break;
      case msrNote::kNoteHeadArrowUp:
        fLilypondCodeOstream << "%{kNoteHeadArrowUp%} ";
        break;
      case msrNote::kNoteHeadSlashed:
        fLilypondCodeOstream << "%{kNoteHeadSlashed%} ";
        break;
      case msrNote::kNoteHeadBackSlashed:
        fLilypondCodeOstream << "%{kNoteHeadBackSlashed%} ";
        break;
      case msrNote::kNoteHeadNormal:
   // JMI     fLilypondCodeOstream << "%{kNoteHeadNormal%} ";
        break;
      case msrNote::kNoteHeadCluster:
        fLilypondCodeOstream << "%{kNoteHeadCluster%} ";
        break;
      case msrNote::kNoteHeadCircleDot:
        fLilypondCodeOstream << "%{kNoteHeadCircleDot%} ";
        break;
      case msrNote::kNoteHeadLeftTriangle:
        fLilypondCodeOstream << "%{kNoteHeadLeftTriangle%} ";
        break;
      case msrNote::kNoteHeadRectangle:
        fLilypondCodeOstream << "%{kNoteHeadRectangle%} ";
        break;
      case msrNote::kNoteHeadNone:
        fLilypondCodeOstream << "\\once\\omit NoteHead ";
        break;
      case msrNote::kNoteHeadDo:
        fLilypondCodeOstream << "\\tweak style #'do ";
        break;
      case msrNote::kNoteHeadRe:
        fLilypondCodeOstream << "\\tweak style #'re ";
        break;
      case msrNote::kNoteHeadMi:
        fLilypondCodeOstream << "\\tweak style #'mi ";
        break;
      case msrNote::kNoteHeadFa:
        fLilypondCodeOstream << "\\tweak style #'fa ";
        break;
      case msrNote::kNoteHeadFaUp:
        fLilypondCodeOstream << "\\tweak style #'triangle ";
        break;
      case msrNote::kNoteHeadSo:
        fLilypondCodeOstream << "\\tweak style #'sol ";
        break;
      case msrNote::kNoteHeadLa:
        fLilypondCodeOstream << "\\tweak style #'la ";
        break;
      case msrNote::kNoteHeadTi:
        fLilypondCodeOstream << "\\tweak style #'ti ";
        break;
    } // switch
  }
}

void lpsr2LilypondTranslator::generateCoda (S_msrCoda coda)
{
  switch (coda->getCodaKind ()) {
    case msrCoda::kCodaFirst:
      // generate the coda
      fLilypondCodeOstream <<
        "\\mark \\markup { \\musicglyph #\"scripts.coda\" }%{first%}";
      break;
    case msrCoda::kCodaSecond:
      // generate the coda
      fLilypondCodeOstream <<
        "\\mark \\markup { \\musicglyph #\"scripts.coda\" }%{second%}";
      break;
  } // switch
  fLilypondCodeOstream << endl;
}

void lpsr2LilypondTranslator::generateSegno (S_msrSegno segno)
{
  // generate the segno
  fLilypondCodeOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.segno\" }" <<
    endl;
}

void lpsr2LilypondTranslator::generateCodeRightBeforeNote (S_msrNote note)
{
  if (! fOnGoingChord) {
    // generate the note codas if any
    const list<S_msrCoda>&
      noteCodas =
        note->getNoteCodas ();

    if (noteCodas.size ()) {
      list<S_msrCoda>::const_iterator i;
      for (i=noteCodas.begin (); i!=noteCodas.end (); i++) {
        S_msrCoda coda = (*i);

        // generate only the second coda before the note
        switch (coda->getCodaKind ()) {
          case msrCoda::kCodaFirst:
            break;
          case msrCoda::kCodaSecond:
            // generate the coda
            generateCoda (coda);
            break;
        } // switch
      } // for
    }

    // generate the note segnos if any
    const list<S_msrSegno>&
      noteSegnos =
        note->getNoteSegnos ();

    if (noteSegnos.size ()) {
      list<S_msrSegno>::const_iterator i;
      for (i=noteSegnos.begin (); i!=noteSegnos.end (); i++) {
      } // for
    }
  }

  // print the note head color
  generateNoteHeadColor (note);

  // print the note ligatures if any
  list<S_msrLigature>
    noteLigatures =
      note->getNoteLigatures ();

  if (noteLigatures.size ()) {
    generateNoteLigatures (note);
  }

  // print note stem kind if needed
  if (! fOnGoingChord) {
    S_msrStem
      noteStem =
        note->getNoteStem ();

    generateNoteStemIfNeeded (noteStem);
  }

  // handling note head
  generateNoteHead (note);
}

void lpsr2LilypondTranslator::generateCodeForNote (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // print the note itself
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  switch (note->getNoteKind ()) {

    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
      {
        // get pitched rest status
        bool noteIsAPitchedRest =
          note->noteIsAPitchedRest ();

        if (noteIsAPitchedRest) {
          // pitched rest
          fLilypondCodeOstream <<
            pitchedRestAsLilypondString (note);

          // this note is the new relative octave reference
          // (the display quarter tone pitch and octave
          // have been copied to the note octave
          // in the msrNote::msrNote () constructor,
          // since the note octave is used in relative code generation)
          switch (gLpsr2LilypondOah->fOctaveEntryKind) {
            case kOctaveEntryRelative:
              fCurrentOctaveEntryReference = note;
              break;
            case kOctaveEntryAbsolute:
              break;
            case kOctaveEntryFixed:
              break;
          } // switch
        }

        else {
          // unpitched rest
          // get the note sounding whole notes
          rational
            noteSoundingWholeNotes =
              note->getNoteSoundingWholeNotes ();

          // get note's voice
          S_msrVoice
            noteVoice =
              note->
                getNoteMeasureUpLink ()->
                  getMeasureSegmentUpLink ()->
                    getSegmentVoiceUpLink ();

          // print the rest name and duration
          if (note->getNoteOccupiesAFullMeasure ()) {
            // take voice kind into account JMI shouldn't be necessary?
            switch (noteVoice->getVoiceKind ()) {
              case msrVoice::kVoiceRegular:
                fLilypondCodeOstream <<
                  "R%{1%}";
                break;

              case msrVoice::kVoiceHarmony:
              case msrVoice::kVoiceFiguredBass:
                fLilypondCodeOstream <<
                  "s%{1%}";
                break;
            } // switch

            fLilypondCodeOstream <<
              durationAsLilypondString (
                inputLineNumber,
                noteSoundingWholeNotes);
          }

          else {
            // take voice kind into account JMI shouldn't be necessary?
            switch (noteVoice->getVoiceKind ()) {
              case msrVoice::kVoiceRegular:
                fLilypondCodeOstream <<
                  "r%{2%}";
                break;

              case msrVoice::kVoiceHarmony:
              case msrVoice::kVoiceFiguredBass:
                fLilypondCodeOstream <<
                  "s%{2%}";
                break;
            } // switch

            fLilypondCodeOstream <<
              durationAsLilypondString (
                inputLineNumber,
                noteSoundingWholeNotes);

/* JMI BOF
            if (fOnGoingVoiceCadenza) { // JMI
              if (noteSoundingWholeNotes != rational (1, 1)) {
                / * JMI
                // force the generation of the duration if needed
                if (! gLpsr2LilypondOah->fAllDurations) {
                  fLilypondCodeOstream << // JMI
                    wholeNotesAsLilypondString (
                      inputLineNumber,
                      noteSoundingWholeNotes);
                }
                * /

                // generate the multiplying factor
                fLilypondCodeOstream << // JMI
                  "*" <<
                  noteSoundingWholeNotes <<
                  "";
              }
            }
                  */
          }

          // an unpitched rest is no relative octave reference,
          // the preceding one is kept
        }
      }
      break;

    case msrNote::kSkipNote:
      if (gLpsr2LilypondOah->fPositionsInMeasures) {
        // print the rest name to help pin-point bugs
        fLilypondCodeOstream << "r%{3%}";
      }
      else {
        // print the skip name
        fLilypondCodeOstream << "s%{44%}";
      }

      // print the skip duration
      fLilypondCodeOstream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteSoundingWholeNotes ());

      // a skip is no relative octave reference,
      // the preceding one is kept
      break;

    case msrNote::kUnpitchedNote:
      {
        // print the note name, "e" by convention
        fLilypondCodeOstream <<
            "e";

        rational
          noteSoundingWholeNotes =
            note->
              getNoteSoundingWholeNotes ();

        // print the note duration
        fLilypondCodeOstream <<
          durationAsLilypondString (
            inputLineNumber,
            noteSoundingWholeNotes);

        // handle delayed ornaments if any
        if (note->getNoteDelayedTurnOrnament ()) {
          // c2*2/3 ( s2*1/3\turn) JMI
          // we need the explicit duration in all cases,
          // regardless of gGeneralOah->fAllDurations
          fLilypondCodeOstream <<
            wholeNotesAsLilypondString (
              inputLineNumber,
              noteSoundingWholeNotes) <<
            "*" <<
            gLpsr2LilypondOah->fDelayedOrnamentsFraction;
        }

/* JMI
        // print the tie if any
        {
          S_msrTie noteTie = note->getNoteTie ();

          if (noteTie) {
            if (noteTie->getTieKind () == msrTie::kTieStart) {
              fLilypondCodeOstream <<
                "%{line " << inputLineNumber << "%}" <<
                " ~  %{kUnpitchedNote%}"; // JMI spaces???
            }
          }
        }
        */
      }
      break;

    case msrNote::kRegularNote:
      {
        // print the note name
        fLilypondCodeOstream <<
          notePitchAsLilypondString (note);

        rational
          noteSoundingWholeNotes =
            note->
              getNoteSoundingWholeNotes ();

        // print the note duration
        fLilypondCodeOstream <<
          durationAsLilypondString (
            inputLineNumber,
            noteSoundingWholeNotes);

        // handle delayed ornaments if any
        if (note->getNoteDelayedTurnOrnament ()) {
          // c2*2/3 ( s2*1/3\turn) JMI
          // we need the explicit duration in all cases,
          // regardless of gGeneralOah->fAllDurations
          fLilypondCodeOstream <<
          //* JMI TOO MUCH
            wholeNotesAsLilypondString (
              inputLineNumber,
              noteSoundingWholeNotes) <<
              //*/
            "*" <<
            gLpsr2LilypondOah->fDelayedOrnamentsFraction;
        }

        // print the tie if any
        {
          S_msrTie noteTie = note->getNoteTie ();

          if (noteTie) {
            if (noteTie->getTieKind () == msrTie::kTieStart) {
      //        fLilypondCodeOstream << " ~ %{kRegularNote%}"; // JMI
            }
          }
        }

        // this note is the new relative octave reference
        switch (gLpsr2LilypondOah->fOctaveEntryKind) {
          case kOctaveEntryRelative:
            fCurrentOctaveEntryReference = note;
            break;
          case kOctaveEntryAbsolute:
            break;
          case kOctaveEntryFixed:
            break;
        } // switch
      }
      break;

    case msrNote::kDoubleTremoloMemberNote:
      // print the note name
      fLilypondCodeOstream <<
        notePitchAsLilypondString (note);

      // print the note duration
      fLilypondCodeOstream <<
        durationAsLilypondString (
          inputLineNumber,
          note->getNoteSoundingWholeNotes ());

      // handle delayed ornaments if any
      if (note->getNoteDelayedTurnOrnament ()) {
        // c2*2/3 ( s2*1/3\turn JMI
        fLilypondCodeOstream <<
          "*" <<
          gLpsr2LilypondOah->fDelayedOrnamentsFraction;
      }

/* JMI
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();

        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeOstream <<
              "%{line " << inputLineNumber << "%}" <<
              " ~ %{kDoubleTremoloMemberNote%}";
          }
        }
      }
*/

      // this note is the new relative octave reference
      switch (gLpsr2LilypondOah->fOctaveEntryKind) {
        case kOctaveEntryRelative:
          fCurrentOctaveEntryReference = note;
          break;
        case kOctaveEntryAbsolute:
          break;
        case kOctaveEntryFixed:
          break;
      } // switch
      break;

    case msrNote::kGraceNote:
      // print the note name
      fLilypondCodeOstream <<
        notePitchAsLilypondString (note);

      // print the grace note's graphic duration
      fLilypondCodeOstream <<
        msrDurationKindAsString (
          note->
            getNoteGraphicDurationKind ());

      // print the dots if any JMI ???
      for (int i = 0; i < note->getNoteDotsNumber (); i++) {
        fLilypondCodeOstream << ".";
      } // for

      // don't print the tie if any, 'acciacattura takes care of it
      /*
      {
        S_msrTie noteTie = note->getNoteTie ();

        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeOstream <<
              "%{line " << inputLineNumber << "%}" <<
              "~  %{kGraceNote%}";
          }
        }
      }
      */

      // this note is the new relative octave reference
      switch (gLpsr2LilypondOah->fOctaveEntryKind) {
        case kOctaveEntryRelative:
          fCurrentOctaveEntryReference = note;
          break;
        case kOctaveEntryAbsolute:
          break;
        case kOctaveEntryFixed:
          break;
      } // switch
      break;

    case msrNote::kGraceChordMemberNote:
      // print the note name
      fLilypondCodeOstream <<
        notePitchAsLilypondString (note);

      // dont't print the grace note's graphic duration

      // print the dots if any JMI ???
      for (int i = 0; i < note->getNoteDotsNumber (); i++) {
        fLilypondCodeOstream << ".";
      } // for

      // don't print the tie if any, 'acciacattura takes care of it
      /*
      {
        S_msrTie noteTie = note->getNoteTie ();

        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeOstream <<
              "%{line " << inputLineNumber << "%}" <<
              "~  %{kGraceChordMemberNote%}";
          }
        }
      }
      */

      // inside chords, a note is relative to the preceding one
      switch (gLpsr2LilypondOah->fOctaveEntryKind) {
        case kOctaveEntryRelative:
          fCurrentOctaveEntryReference = note;
          break;
        case kOctaveEntryAbsolute:
          break;
        case kOctaveEntryFixed:
          break;
      } // switch
      break;

    case msrNote::kChordMemberNote:
      {
        // print the note name
        fLilypondCodeOstream <<
          notePitchAsLilypondString (note);

        // don't print the note duration,
        // it will be printed for the chord itself

        // don't print the string number if any,
        // it should appear after the chord itself
        const list<S_msrTechnicalWithInteger>&
          chordMemberNoteTechnicalsWithIntegers =
            note->getNoteTechnicalWithIntegers ();

        if (chordMemberNoteTechnicalsWithIntegers.size ()) {
          list<S_msrTechnicalWithInteger>::const_iterator i;
          for (
            i=chordMemberNoteTechnicalsWithIntegers.begin ();
            i!=chordMemberNoteTechnicalsWithIntegers.end ();
            i++
          ) {
            S_msrTechnicalWithInteger
              technicalWithInteger = (*i);

            switch (technicalWithInteger->getTechnicalWithIntegerKind ()) {
              case msrTechnicalWithInteger::kFingering:
                break;
              case msrTechnicalWithInteger::kFret:
                break;
              case msrTechnicalWithInteger::kString:
                if (fOnGoingChord) {
                  fPendingChordMemberNotesStringNumbers.push_back (
                    technicalWithInteger->
                        getTechnicalWithIntegerValue ());
                }
                break;
            } // switch
          } // for
        }

        // inside chords, a note is relative to the preceding one
        switch (gLpsr2LilypondOah->fOctaveEntryKind) {
          case kOctaveEntryRelative:
            fCurrentOctaveEntryReference = note;
            break;
          case kOctaveEntryAbsolute:
            break;
          case kOctaveEntryFixed:
            break;
        } // switch
      }
      break;

    case msrNote::kTupletMemberNote:
      if (gLpsr2LilypondOah->fIndentTuplets) {
        fLilypondCodeOstream << endl;
      }

      // print the note name
      fLilypondCodeOstream <<
        notePitchAsLilypondString (note);

      // print the note display duration
      fLilypondCodeOstream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

/* JMI
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();

        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeOstream <<
              "%{line " << inputLineNumber << "%}" <<
              "~  %{kTupletMemberNote%}"; // JMI spaces???
          }
        }
      }
*/

      // this note is the new relative octave reference
      switch (gLpsr2LilypondOah->fOctaveEntryKind) {
        case kOctaveEntryRelative:
          fCurrentOctaveEntryReference = note;
          break;
        case kOctaveEntryAbsolute:
          break;
        case kOctaveEntryFixed:
          break;
      } // switch
      break;

    case msrNote::kTupletRestMemberNote:
      if (gLpsr2LilypondOah->fIndentTuplets) {
        fLilypondCodeOstream << endl;
      }

      // print the note name
      fLilypondCodeOstream <<
        string (
          note->getNoteOccupiesAFullMeasure ()
            ? "s%{6%}" // JMI ??? "R"
            : "r%{5%}");

      // print the note display duration
      fLilypondCodeOstream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

/* JMI
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();

        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeOstream <<
              "%{line " << inputLineNumber << "%}" <<
              "~  %{kTupletMemberNote%}"; // JMI spaces???
          }
        }
      }
*/

      // a rest is no relative octave reference,
      break;

    case msrNote::kTupletUnpitchedMemberNote:
      if (gLpsr2LilypondOah->fIndentTuplets) {
        fLilypondCodeOstream << endl;
      }

      // print the note name
      fLilypondCodeOstream <<
        "e"; // by convention

      // print the note (display) duration
      fLilypondCodeOstream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

/* JMI
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();

        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeOstream <<
              "%{line " << inputLineNumber << "%}" <<
              "~  %{kTupletUnpitchedMemberNote%}";
          }
        }
      }
      */
      break;

    case msrNote::kGraceTupletMemberNote:
      if (gLpsr2LilypondOah->fIndentTuplets) {
        fLilypondCodeOstream << endl;
      }

      // print the note name
      if (note->getNoteIsARest ()) {
        fLilypondCodeOstream <<
          string (
            note->getNoteOccupiesAFullMeasure ()
              ? "R%{4%}"
              : "r%{6%}");
      }
      else {
        fLilypondCodeOstream <<
          notePitchAsLilypondString (note);
      }

      // print the note display duration
      fLilypondCodeOstream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();

        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeOstream <<
              "%{line " << inputLineNumber << "%}" <<
              "~  %{kGraceTupletMemberNote%}"; // JMI spaces???
          }
        }
      }

      // this note is no new relative octave reference JMI ???
      // this note is the new relative octave reference
      switch (gLpsr2LilypondOah->fOctaveEntryKind) {
        case kOctaveEntryRelative:
          fCurrentOctaveEntryReference = note;
          break;
        case kOctaveEntryAbsolute:
          break;
        case kOctaveEntryFixed:
          break;
      } // switch
      break;
  } // switch

  fLilypondCodeOstream << ' ';
}

void lpsr2LilypondTranslator::generateCodeRightAfterNote (
  S_msrNote note)
{
  if (! fOnGoingChord) {
    // generate the note codas if any
    const list<S_msrCoda>&
      noteCodas =
        note->getNoteCodas ();

    if (noteCodas.size ()) {
      list<S_msrCoda>::const_iterator i;
      for (i=noteCodas.begin (); i!=noteCodas.end (); i++) {
        S_msrCoda coda = (*i);

        // generate only the first coda before the note
        switch (coda->getCodaKind ()) {
          case msrCoda::kCodaFirst:
            // generate the coda
            generateCoda (coda);
            break;
          case msrCoda::kCodaSecond:
            break;
        } // switch
      } // for
    }

    // generate the note dal segnos if any
    const list<S_msrDalSegno>&
      noteDalSegnos =
        note->getNoteDalSegnos ();

    if (noteDalSegnos.size ()) {
      list<S_msrDalSegno>::const_iterator i;
      for (i=noteDalSegnos.begin (); i!=noteDalSegnos.end (); i++) {
        // generate the dal segno
        fLilypondCodeOstream <<
          endl <<
          "\\override Score.RehearsalMark.break-visibility = #begin-of-line-invisible" <<
          endl <<
          "\\tweak self-alignment-X #RIGHT" <<
          endl <<
          "\\mark \\markup { " <<
          (*i)->getDalSegnoString () <<
          " }" <<
          endl;
      } // for
    }
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateNoteArticulation (
  S_msrArticulation articulation)
{
  // should the placement be generated?
  bool doGeneratePlacement = true;

  // generate note articulation preamble if any
  switch (articulation->getArticulationKind ()) {
    case msrArticulation::k_NoArticulation:
       break;

    case msrArticulation::kAccent:
       break;
    case msrArticulation::kBreathMark:
      doGeneratePlacement = false;
      break;
    case msrArticulation::kCaesura:
      doGeneratePlacement = false;
      break;
    case msrArticulation::kSpiccato:
      doGeneratePlacement = false;
      break;
    case msrArticulation::kStaccato:
      doGeneratePlacement = true;
      break;
    case msrArticulation::kStaccatissimo:
      doGeneratePlacement = true;
      break;
    case msrArticulation::kStress:
      doGeneratePlacement = false;
      break;
    case msrArticulation::kUnstress:
      doGeneratePlacement = false;
      break;
    case msrArticulation::kDetachedLegato:
      doGeneratePlacement = true;
      break;
    case msrArticulation::kStrongAccent:
      doGeneratePlacement = true;
      break;
    case msrArticulation::kTenuto:
      doGeneratePlacement = true;
      break;

    case msrArticulation::kFermata:
      doGeneratePlacement = true;
      break;

    case msrArticulation::kArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      doGeneratePlacement = false;
      break;
    case msrArticulation::kNonArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      doGeneratePlacement = false;
      break;

    case msrArticulation::kDoit:
      doGeneratePlacement = true;
      break;
    case msrArticulation::kFalloff:
      doGeneratePlacement = true;
      break;
    case msrArticulation::kPlop:
      doGeneratePlacement = false;
      break;
    case msrArticulation::kScoop:
      doGeneratePlacement = false;
      break;
  } // switch

  if (doGeneratePlacement) {
    switch (articulation->getArticulationPlacementKind ()) {
      case kPlacementNone:
        fLilypondCodeOstream << "-";
        break;
      case kPlacementAbove:
        fLilypondCodeOstream << "^";
        break;
      case kPlacementBelow:
        fLilypondCodeOstream << "_";
        break;
    } // switch
  }

  // generate note articulation itself
  switch (articulation->getArticulationKind ()) {
    case msrArticulation::k_NoArticulation:
      fLilypondCodeOstream << ">";
      break;

    case msrArticulation::kAccent:
      fLilypondCodeOstream << ">";
      break;
    case msrArticulation::kBreathMark:
      fLilypondCodeOstream << "\\breathe";
      break;
    case msrArticulation::kCaesura:
    /* JMI
          fLilypondCodeOstream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl;
     */
      fLilypondCodeOstream <<
        endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        endl <<
        "\\breathe" <<
        endl;
      break;
    case msrArticulation::kSpiccato:
      fLilypondCodeOstream <<
        "%{spiccato???%}";
      break;
    case msrArticulation::kStaccato:
      fLilypondCodeOstream <<
        ".";
      break;
    case msrArticulation::kStaccatissimo:
      fLilypondCodeOstream <<
        "!";
      break;
    case msrArticulation::kStress:
      fLilypondCodeOstream <<
        "%{stress???%}";
      break;
    case msrArticulation::kUnstress:
      fLilypondCodeOstream <<
        "%{unstress???%}";
      break;
    case msrArticulation::kDetachedLegato:
      fLilypondCodeOstream <<
        "_"; // portato
      break;
    case msrArticulation::kStrongAccent:
      fLilypondCodeOstream <<
        "^"; // marcato
      break;
    case msrArticulation::kTenuto:
      fLilypondCodeOstream <<
        "-";
      break;

    case msrArticulation::kFermata:
      if (
        // fermata?
        S_msrFermata
          fermata =
            dynamic_cast<msrFermata*>(&(*articulation))
        ) {
        switch (fermata->getFermataTypeKind ()) {
          case msrFermata::kFermataTypeNone:
            // no placement needed
            break;
          case msrFermata::kFermataTypeUpright:
            // no placement needed
            break;
          case msrFermata::kFermataTypeInverted:
            fLilypondCodeOstream << "_";
            break;
        } // switch

        switch (fermata->getFermataKind ()) {
          case msrFermata::kNormalFermataKind:
            fLilypondCodeOstream << "\\fermata ";
            break;
          case msrFermata::kAngledFermataKind:
            fLilypondCodeOstream << "\\shortfermata ";
            break;
          case msrFermata::kSquareFermataKind:
            fLilypondCodeOstream << "\\longfermata ";
            break;
        } // switch
      }
      else {
        stringstream s;

        s <<
          "note articulation '" <<
          articulation->asString () <<
          "' has 'fermata' kind, but is not of type S_msrFermata" <<
          ", line " << articulation->getInputLineNumber ();

        msrInternalError (
          gOahOah->fInputSourceName,
          articulation->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrArticulation::kArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      break;
    case msrArticulation::kNonArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      break;
    case msrArticulation::kDoit:
      fLilypondCodeOstream <<
        "\\bendAfter #+4";
      break;
    case msrArticulation::kFalloff:
      fLilypondCodeOstream <<
        "\\bendAfter #-4";
      break;
    case msrArticulation::kPlop:
      fLilypondCodeOstream <<
        "%{plop???%}";
      break;
    case msrArticulation::kScoop:
      fLilypondCodeOstream <<
        "%{scoop???%}";
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateChordArticulation (
  S_msrArticulation articulation)
{
  switch (articulation->getArticulationPlacementKind ()) {
    case kPlacementNone:
      fLilypondCodeOstream << "-";
      break;
    case kPlacementAbove:
      fLilypondCodeOstream << "^";
      break;
    case kPlacementBelow:
      fLilypondCodeOstream << "_";
      break;
  } // switch

  switch (articulation->getArticulationKind ()) {
    case msrArticulation::k_NoArticulation:
      break;

    case msrArticulation::kAccent:
      fLilypondCodeOstream << ">";
      break;
    case msrArticulation::kBreathMark:
      fLilypondCodeOstream << "\\breathe";
      break;
    case msrArticulation::kCaesura:
    /* JMI
          fLilypondCodeOstream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl;
     */
      fLilypondCodeOstream <<
        endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        endl <<
      "\\breathe" <<
        endl;
      break;
    case msrArticulation::kSpiccato:
      fLilypondCodeOstream <<
        "%{spiccato???%}";
      break;
    case msrArticulation::kStaccato:
      fLilypondCodeOstream <<
        "\\staccato"; // JMI "-.";
      break;
    case msrArticulation::kStaccatissimo:
      fLilypondCodeOstream << "!";
      break;
    case msrArticulation::kStress:
      fLilypondCodeOstream <<
        "%{stress???%}";
      break;
    case msrArticulation::kUnstress:
      fLilypondCodeOstream <<
        "%{unstress%}";
      break;
    case msrArticulation::kDetachedLegato:
      fLilypondCodeOstream <<
        "_"; // portato
      break;
    case msrArticulation::kStrongAccent:
      fLilypondCodeOstream <<
        "^"; // marcato
      break;
    case msrArticulation::kTenuto:
      fLilypondCodeOstream << "-";
      break;

    case msrArticulation::kFermata:
      if (
        // fermata?
        S_msrFermata
          fermata =
            dynamic_cast<msrFermata*>(&(*articulation))
        ) {
        switch (fermata->getFermataTypeKind ()) {
          case msrFermata::kFermataTypeNone:
            // no placement needed
            break;
          case msrFermata::kFermataTypeUpright:
            // no placement needed
            break;
          case msrFermata::kFermataTypeInverted:
            fLilypondCodeOstream << "_";
            break;
        } // switch

        switch (fermata->getFermataKind ()) {
          case msrFermata::kNormalFermataKind:
            fLilypondCodeOstream << "\\fermata ";
            break;
          case msrFermata::kAngledFermataKind:
            fLilypondCodeOstream << "\\shortfermata ";
            break;
          case msrFermata::kSquareFermataKind:
            fLilypondCodeOstream << "\\longfermata ";
            break;
        } // switch
      }
      else {
        stringstream s;

        s <<
          "chord articulation '" <<
          articulation->asString () <<
          "' has 'fermata' kind, but is not of type S_msrFermata" <<
          ", line " << articulation->getInputLineNumber ();

        msrInternalError (
          gOahOah->fInputSourceName,
          articulation->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrArticulation::kArpeggiato:
      fLilypondCodeOstream <<
        "\\arpeggio";
      break;
    case msrArticulation::kNonArpeggiato:
      fLilypondCodeOstream <<
        "\\arpeggio";
      break;
    case msrArticulation::kDoit:
      fLilypondCodeOstream <<
        "\\bendAfter #+4";
      break;
    case msrArticulation::kFalloff:
      fLilypondCodeOstream <<
        "\\bendAfter #-4";
      break;
    case msrArticulation::kPlop:
      fLilypondCodeOstream <<
        "%{plop%}";
      break;
    case msrArticulation::kScoop:
      fLilypondCodeOstream <<
        "%{scoop%}";
      break;
  } // switch
}

//________________________________________________________________________
string lpsr2LilypondTranslator::technicalAsLilypondString (
  S_msrTechnical technical)
{
  string result;

  switch (technical->getTechnicalKind ()) {
    case msrTechnical::kArrow:
      result = "%{\\Arrow???%}";
      break;
    case msrTechnical::kDoubleTongue:
      result = "-\\tongue #2";
      break;
    case msrTechnical::kDownBow:
      result = "\\downbow";
      break;
    case msrTechnical::kFingernails:
      result = "%{\\Fingernails???%}";
      break;
    case msrTechnical::kHarmonic:
      result = "\\flageolet"; // JMI "\\once\\override Staff.NoteHead.style = #'harmonic-mixed";
      break;
    case msrTechnical::kHeel:
      result = "\\lheel"; // rheel??? JMI
      break;
    case msrTechnical::kHole:
      result = "%{\\Hole???%}";
      break;
    case msrTechnical::kOpenString:
      result = "\\open"; // halfopen ??? JMI
      break;
    case msrTechnical::kSnapPizzicato:
      result = "\\snappizzicato";
      break;
    case msrTechnical::kStopped:
      result = "\\stopped"; // or -+ JMI
      break;
    case msrTechnical::kTap:
      result = "%{\\Tap???%}";
      break;
    case msrTechnical::kThumbPosition:
      result = "\\thumb";
      break;
    case msrTechnical::kToe:
      result = "\\ltoe"; // rtoe ??? JMI
      break;
    case msrTechnical::kTripleTongue:
      result = "-\\tongue #3";
      break;
    case msrTechnical::kUpBow:
      result = "\\upbow";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::technicalWithIntegerAsLilypondString (
  S_msrTechnicalWithInteger technicalWithInteger)
{
  stringstream s;

  switch (technicalWithInteger->getTechnicalWithIntegerKind ()) {
     case msrTechnicalWithInteger::kFingering:
      s <<
        "- " <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
    case msrTechnicalWithInteger::kFret:
      // LilyPond will take care of that JMI
      break;
    case msrTechnicalWithInteger::kString:
      s << // no space is allowed between the backSlash and the number
        "\\" <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
  } // switch

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::technicalWithFloatAsLilypondString (
  S_msrTechnicalWithFloat technicalWithFloat)
{
  stringstream s;

  switch (technicalWithFloat->getTechnicalWithFloatKind ()) {
    case msrTechnicalWithFloat::kBend:
      s <<
        "\\bendAfter " <<
       technicalWithFloat->
          getTechnicalWithFloatValue ();
      break;
  } // switch

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::technicalWithStringAsLilypondString (
  S_msrTechnicalWithString technicalWithString)
{
  string result;

  switch (technicalWithString->getTechnicalWithStringKind ()) {
    case msrTechnicalWithString::kHammerOn:
      break;
    case msrTechnicalWithString::kHandbell:
      result = "%{\\Handbell???%}";
      break;
    case msrTechnicalWithString::kOtherTechnical:
      result = "%{\\OtherTechnical???%}";
      break;
    case msrTechnicalWithString::kPluck:
      result = "%{Pluck???%}";
      break;
    case msrTechnicalWithString::kPullOff:
      break;
  } // switch

  string stringValue =
    technicalWithString->
      getTechnicalWithStringValue ();

  if (stringValue.size ()) {
    result +=
      string (" ") +
      "-\\markup {\"" + stringValue + "\"}";
  }

  return result;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateOrnament (
  S_msrOrnament ornament)
{
  S_msrNote
    ornamentNoteUpLink =
      ornament->
        getOrnamentNoteUpLink ();

  string
    noteUpLinkDuration =
      ornamentNoteUpLink->
        noteSoundingWholeNotesAsMsrString ();

  switch (ornament->getOrnamentKind ()) {
    case msrOrnament::kOrnamentTrill:
      if (! ornamentNoteUpLink->getNoteWavyLineSpannerStart ()) {
        fLilypondCodeOstream <<
          "\\trill ";
      }
      else {
        fLilypondCodeOstream <<
          "\\startTrillSpan ";
      }
      break;

/* JMI
    case msrOrnament::kOrnamentDashes:
      if (! ornamentNoteUpLink->getNoteWavyLineSpannerStart ()) {
        fLilypondCodeOstream <<
          "%{\\dashes%} ";
      }
      break;
*/

    case msrOrnament::kOrnamentTurn:
      fLilypondCodeOstream <<
          "\\turn ";
      break;

    case msrOrnament::kOrnamentInvertedTurn:
      fLilypondCodeOstream <<
          "\\reverseturn ";
      break;

    case msrOrnament::kOrnamentDelayedTurn:
      {
        // c2*2/3  s2*1/3\turn
        rational
          remainingFraction =
            rational (1, 1)
              -
            gLpsr2LilypondOah->fDelayedOrnamentsFraction;

        int
          numerator =
            remainingFraction.getNumerator (),
          denominator =
            remainingFraction.getDenominator ();

        fLilypondCodeOstream <<
          "s%{16%}" <<
          noteUpLinkDuration <<
          "*" <<
            denominator
            -
            numerator <<
          "/" <<
            denominator <<
          "\\turn ";

        // forget about the last found whole notes duration,
        // since the latter has been multipled by fDelayedOrnamentsFraction
        fLastMetWholeNotes = rational (0, 1);
      }
      break;

    case msrOrnament::kOrnamentDelayedInvertedTurn:
      {
/* JMI
        fLilypondCodeOstream <<
          "delayed inverted turn is not supported, replaced by inverted turn," <<
          endl <<
          "see http://lilypond.org/doc/v2.18/Documentation/snippets/expressive-marks";

        lpsrMusicXMLWarning (
          inputLineNumber,
          s.str ());

        result = "\\reverseturn %{ " + s.str () + " %}";
*/
        // c2*2/3 ( s2*1/3\turn

        fLilypondCodeOstream <<
          "s%{7%}" <<
          noteUpLinkDuration <<
          "*1/3" "\\reverseturn ";
      }
      break;

    case msrOrnament::kOrnamentVerticalTurn:
      fLilypondCodeOstream <<
        "^\\markup { \\rotate #90 \\musicglyph #\"scripts.turn\" } ";
          /* JMI
      {
        string message =
          "delayed vertical turn is not supported, ignored";

        lpsrMusicXMLWarning (
          inputLineNumber,
          message);

        result = "%{ " + message + " %}";
      }
        */
      break;

    case msrOrnament::kOrnamentMordent:
      fLilypondCodeOstream <<
        "\\mordent ";
      break;

    case msrOrnament::kOrnamentInvertedMordent:
      fLilypondCodeOstream <<
        "\\prall ";
      break;
      \
    case msrOrnament::kOrnamentSchleifer:
      fLilypondCodeOstream <<
        "%{\\schleifer???%} ";
      break;

    case msrOrnament::kOrnamentShake:
      fLilypondCodeOstream <<
        "%{\\shake???%} ";
      break;

    case msrOrnament::kOrnamentAccidentalMark:
      switch (ornament->getOrnamentPlacementKind ()) {
        case kPlacementNone:
          fLilypondCodeOstream << "-";
          break;
        case kPlacementAbove:
          fLilypondCodeOstream << "^";
          break;
        case kPlacementBelow:
          fLilypondCodeOstream << "_";
          break;
      } // switch

string result; // JMI
      switch (ornament->getOrnamentAccidentalMark ()) {
        case kAccidentalNone:
          fLilypondCodeOstream << "\\markup { \\accidentalNone } ";
          break;

        case kAccidentalSharp:
          fLilypondCodeOstream << "\\markup { \\aSharp } ";
          break;
        case kAccidentalNatural:
          fLilypondCodeOstream << "\\markup { \\aNatural } ";
          break;
        case kAccidentalFlat:
          fLilypondCodeOstream << "\\markup { \\aFlat } ";
          break;
        case kAccidentalDoubleSharp:
          fLilypondCodeOstream << "\\markup { \\doublesharp } ";
          fLilypondCodeOstream << "\\markup { \\doubleflat } ";
          break;
        case kAccidentalSharpSharp:
          fLilypondCodeOstream << "\\markup { \\aSharpSharp } ";
          break;
        case kAccidentalFlatFlat:
          fLilypondCodeOstream << "\\markup { \\aFlatFlat } ";
          break;
        case kAccidentalNaturalSharp:
          fLilypondCodeOstream << "\\markup { \\aNaturalSharp } ";
          break;
        case kAccidentalNaturalFlat:
          fLilypondCodeOstream << "\\markup { \\aNaturalFlat } ";
          break;
        case kAccidentalQuarterFlat:
          fLilypondCodeOstream << "\\markup { \\aQuarterFlat } ";
          break;
        case kAccidentalQuarterSharp:
          fLilypondCodeOstream << "\\markup { \\aQuarterSharp } ";
          break;
        case kAccidentalThreeQuartersFlat:
          fLilypondCodeOstream << "\\markup { \\aThreeQuartersFlat } ";
          break;
        case kAccidentalThreeQuartersSharp:
          fLilypondCodeOstream << "\\markup { \\aThreeQuartersSharp } ";
          break;

        case kAccidentalSharpDown:
          fLilypondCodeOstream << "\\markup { \\aSharpDown } ";
          break;
        case kAccidentalSharpUp:
          fLilypondCodeOstream << "\\markup { \\aSharpUp } ";
          break;
        case kAccidentalNaturalDown:
          fLilypondCodeOstream << "\\markup { \\aNaturalDown } ";
          break;
        case kAccidentalNaturalUp:
          fLilypondCodeOstream << "\\markup { \\aNaturalUp } ";
          break;
        case kAccidentalFlatDown:
          fLilypondCodeOstream << "\\markup { \\aFlatDown } ";
          break;
        case kAccidentalFlatUp:
          fLilypondCodeOstream << "\\markup { \\aFlatUp } ";
          break;
        case kAccidentalTripleSharp:
          fLilypondCodeOstream << "\\markup { \\aTripleSharp } ";
          break;
        case kAccidentalTripleFlat:
          fLilypondCodeOstream << "\\markup { \\aTripleFlat } ";
          break;
        case kAccidentalSlashQuarterSharp:
          fLilypondCodeOstream << "\\markup { \\aSlashQuarterSharp } ";
          break;
        case kAccidentalSlashSharp:
          fLilypondCodeOstream << "\\markup { \\aSlashSharp } ";
          break;
        case kAccidentalSlashFlat:
          fLilypondCodeOstream << "\\markup { \\aSlashFlat } ";
          break;
        case kAccidentalDoubleSlashFlat:
          fLilypondCodeOstream << "\\markup { \\adoubleSlashFlat } ";
          break;
        case kAccidentalSharp_1:
          fLilypondCodeOstream << "\\markup { \\aSharp_1 } ";
          break;
        case kAccidentalSharp_2:
          fLilypondCodeOstream << "\\markup { \\aSharp_2 } ";
          break;
        case kAccidentalSharp_3:
          fLilypondCodeOstream << "\\markup { \\aSharp_3 } ";
          break;
        case kAccidentalSharp_5:
          fLilypondCodeOstream << "\\markup { \\aSharp_5 } ";
          break;
        case kAccidentalFlat_1:
          fLilypondCodeOstream << "\\markup { \\aFlat_1 } ";
          break;
        case kAccidentalFlat_2:
          fLilypondCodeOstream << "\\markup { \\aFlat_2 } ";
          break;
        case kAccidentalFlat_3:
          fLilypondCodeOstream << "\\markup { \\aFlat_3 } ";
          break;
        case kAccidentalFlat_4:
          fLilypondCodeOstream << "\\markup { \\aFlat_4 } ";
          break;
        case kAccidentalSori:
          fLilypondCodeOstream << "\\markup { \\aSori } ";
          break;
        case kAccidentalKoron:
          fLilypondCodeOstream << "\\markup { \\aKoron } ";
          break;

        case kAccidentalOther:
          fLilypondCodeOstream << "\\markup { \\aOther } ";
          break;
      } // switch
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateCodeForSpannerBeforeNote (
  S_msrSpanner spanner)
{
  switch (spanner->getSpannerKind ()) {

    case msrSpanner::kSpannerDashes:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
          fLilypondCodeOstream <<
            "\\once \\override TextSpanner.style = #'dashed-line" <<
            endl;
          fOnGoingTrillSpanner = true;
          break;
        case kSpannerTypeStop:
//          fLilypondCodeOstream <<
//            "\\stopTextSpan ";
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          break;
      } // switch
      break;

    case msrSpanner::kSpannerWavyLine:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
          if (spanner->getSpannerNoteUpLink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            fOnGoingTrillSpanner = true;
          }
          else {
            fLilypondCodeOstream <<
              "\\once \\override TextSpanner.style = #'trill" <<
              endl;
          }
          break;
        case kSpannerTypeStop:
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          break;
      } // switch

      msrPlacementKind
        spannerPlacementKind =
          spanner->getSpannerPlacementKind ();

      if (spannerPlacementKind != fCurrentSpannerPlacementKind) {
        switch (spannerPlacementKind) {
          case msrPlacementKind::kPlacementNone:
            break;
          case msrPlacementKind::kPlacementAbove:
            fLilypondCodeOstream <<
              "\\textSpannerUp ";
            break;
          case msrPlacementKind::kPlacementBelow:
            fLilypondCodeOstream <<
              "\\textSpannerDown ";
            break;
          } // switch

        fCurrentSpannerPlacementKind = spannerPlacementKind;
      }
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateCodeForSpannerAfterNote (
  S_msrSpanner spanner)
{
  switch (spanner->getSpannerKind ()) {

    case msrSpanner::kSpannerDashes:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
          fLilypondCodeOstream <<
            "\\startTextSpan ";
          fOnGoingTrillSpanner = true;
          break;
        case kSpannerTypeStop:
          fLilypondCodeOstream <<
            "\\stopTextSpan ";
          fOnGoingTrillSpanner = false;
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          break;
      } // switch
      break;

    case msrSpanner::kSpannerWavyLine:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
          if (spanner->getSpannerNoteUpLink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            fOnGoingTrillSpanner = true;
          }
          else {
            fLilypondCodeOstream <<
              "\\startTextSpan " <<
              endl;
          }
          break;
        case kSpannerTypeStop:
          {
            // get spanner start end
            S_msrSpanner
              spannerStartEnd =
                spanner->
                  getSpannerOtherEndSideLink ();

            // sanity check
            msrAssert (
              spannerStartEnd != nullptr,
              "spannerStartEnd is null");

            // has the start end a trill ornament?
            if (spannerStartEnd->getSpannerNoteUpLink ()->getNoteTrillOrnament ()) {
              fLilypondCodeOstream <<
                "\\stopTrillSpan ";
            }
            else {
              fLilypondCodeOstream <<
                "\\stopTextSpan ";
            }
            fOnGoingTrillSpanner = false;
          }
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          break;
      } // switch

/* JMI
      msrPlacementKind
        spannerPlacementKind =
          spanner->getSpannerPlacementKind ();

      if (spannerPlacementKind != fCurrentSpannerPlacementKind) {
        switch (spannerPlacementKind) {
          case msrPlacementKind::kPlacementNone:
            break;
          case msrPlacementKind::kPlacementAbove:
            fLilypondCodeOstream <<
              "\\textSpannerUp ";
            break;
          case msrPlacementKind::kPlacementBelow:
            fLilypondCodeOstream <<
              "\\textSpannerDown ";
            break;
          } // switch

        fCurrentSpannerPlacementKind = spannerPlacementKind;
      }
      */

      break;
  } // switch
}

//________________________________________________________________________
string lpsr2LilypondTranslator::dynamicsAsLilypondString (
  S_msrDynamics dynamics)
{
  string result =
    "\\" + dynamics->dynamicsKindAsString ();

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::harpPedalTuningAsLilypondString (
  msrAlterationKind alterationKind)
{
  string result;

  switch (alterationKind) {
    case kTripleFlat:
      result = "%{ tripleFlat %} ";
      break;
    case kDoubleFlat:
      result = "%{ doubleFlat %} ";
      break;
    case kSesquiFlat:
      result = "%{ sesquiFlat %} ";
      break;
    case kFlat:
      result = "^";
      break;
    case kSemiFlat:
      result = "?";
      break;
    case kNatural:
      result = "-";
      break;
    case kSemiSharp:
      result = "?";
      break;
    case kSharp:
      result = "v";
      break;
    case kSesquiSharp:
      result = "%{ sesquiSharp %} ";
      break;
    case kDoubleSharp:
      result = "%{ doubleSharp %} ";
      break;
    case kTripleSharp:
      result = "%{ tripleSharp %} ";
      break;
    case k_NoAlteration:
      result = "alteration???";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::transposeDiatonicError (
  int inputLineNumber,
  int transposeDiatonic,
  int transposeChromatic)
{
  stringstream s;

  s <<
    "diatonic '" << transposeDiatonic <<
    "' is not consistent with " <<
    "chromatic '" << transposeChromatic <<
    "'";

  msrMusicXMLError (
    gOahOah->fInputSourceName,
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

//________________________________________________________________________
string lpsr2LilypondTranslator::singleTremoloDurationAsLilypondString (
  S_msrSingleTremolo singleTremolo)
{
  int
    singleTremoloMarksNumber =
      singleTremolo->
        getSingleTremoloMarksNumber ();

/* JMI
  S_msrNote
    singleTremoloNoteUpLink =
      singleTremolo->
        getSingleTremoloNoteUpLink ();
*/

  msrDurationKind
    singleTremoloNoteDurationKind =
      singleTremolo->
        getSingleTremoloGraphicDurationKind ();
    /*
      singleTremoloNoteUpLink->
        getNoteGraphicDurationKind ();
    */

  /*
  The same output can be obtained by adding :N after the note,
  where N indicates the duration of the subdivision (it must be at least 8).
  If N is 8, one beam is added to the note’s stem.
  */

  int durationToUse =
    singleTremoloMarksNumber; // JMI / singleTremoloNoteSoundingWholeNotes;

  if (singleTremoloNoteDurationKind >= kEighth) {
    durationToUse +=
      1 + (singleTremoloNoteDurationKind - kEighth);
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceTremolos) {
    fLogOutputStream <<
      "singleTremoloDurationAsLilypondString()" <<
      ", line " << singleTremolo->getInputLineNumber () <<
      ", " <<
      singularOrPlural (
        singleTremoloMarksNumber, "mark", "marks") <<
      ", singleTremoloNoteDurationKind : " <<
      singleTremoloNoteDurationKind <<
      ", durationToUse : " <<
      durationToUse <<
      endl;
  }
#endif

  stringstream s;

  s <<
    ":" <<
 // JMI   int (pow (2, durationToUse + 2)) <<
      int (1 << (durationToUse + 2)) <<
    ' ';

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::harmonyDegreeAlterationKindAsLilypondString (
  msrAlterationKind harmonyDegreeAlterationKind)
{
  string result;

  switch (harmonyDegreeAlterationKind) {
    case k_NoAlteration:
      result = "?";
      break;
    case kTripleFlat:
      result = "?";
      break;
    case kDoubleFlat:
      result = "?";
      break;
    case kSesquiFlat:
      result = "?";
      break;
    case kFlat:
      result = "-";
      break;
    case kSemiFlat:
      result = "?";
      break;
    case kNatural:
      result = "";
      break;
    case kSemiSharp:
      result = "?";
      break;
    case kSharp:
      result = "+";
      break;
    case kSesquiSharp:
      result = "?";
      break;
    case kDoubleSharp:
      result = "?";
      break;
    case kTripleSharp:
      result = "?";
      break;
  } // switch

  return result;
}

string lpsr2LilypondTranslator::harmonyAsLilypondString (
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();

  stringstream s;

  // should '\powerChords' be generated?
  switch (harmony->getHarmonyKind ()) {
    case kPowerHarmony:
      if (! fPowerChordHaveAlreadyBeenGenerated) {
        s << "\\powerChords ";
        fPowerChordHaveAlreadyBeenGenerated = true;
      }
      break;
    default:
      ;
  } // switch

  // print harmony pitch
  s <<
    msrQuarterTonesPitchKindAsString (
      gMsrOah->
        fMsrQuarterTonesPitchesLanguageKind,
      harmony->
        getHarmonyRootQuarterTonesPitchKind ());

  // print harmony duration
  msrTupletFactor
    harmonyTupletFactor =
      harmony->getHarmonyTupletFactor ();

  if (harmonyTupletFactor.isEqualToOne ()) {
    // use harmony sounding whole notes
    s <<
      durationAsLilypondString (
        inputLineNumber,
        harmony->
          getHarmonySoundingWholeNotes ());
  }
  else {
    // use harmony display whole notes and tuplet factor
    s <<
      durationAsLilypondString (
        inputLineNumber,
        harmony->
          getHarmonyDisplayWholeNotes ()) <<
      "*" <<
      rational (1, 1) / harmonyTupletFactor.asRational ();
  }

  // print harmony kind
  switch (harmony->getHarmonyKind ()) {
    case k_NoHarmony:
      s << "Harmony???";
      break;

    // MusicXML harmonies

    case kMajorHarmony:
      s << ":5.3";
      break;
    case kMinorHarmony:
      s << ":m";
      break;
    case kAugmentedHarmony:
      s << ":aug";
      break;
    case kDiminishedHarmony:
      s << ":dim";
      break;

    case kDominantHarmony:
      s << ":7";
      break;
    case kMajorSeventhHarmony:
      s << ":maj7";
      break;
    case kMinorSeventhHarmony:
      s << ":m7";
      break;
    case kDiminishedSeventhHarmony:
      s << ":dim7";
      break;
    case kAugmentedSeventhHarmony:
      s << ":aug7";
      break;
    case kHalfDiminishedHarmony:
      s << ":m7.5-";
      break;
    case kMinorMajorSeventhHarmony:
      s << ":m7+";
      break;

    case kMajorSixthHarmony:
      s << ":6";
      break;
    case kMinorSixthHarmony:
      s << ":m6";
      break;

    case kDominantNinthHarmony:
      s << ":9";
      break;
    case kMajorNinthHarmony:
      s << ":maj7.9";
      break;
    case kMinorNinthHarmony:
      s << ":m7.9";
      break;

    case kDominantEleventhHarmony:
      s << ":11";
      break;
    case kMajorEleventhHarmony:
      s << ":maj7.11";
      break;
    case kMinorEleventhHarmony:
      s << ":m7.11";
      break;

    case kDominantThirteenthHarmony:
      s << ":13";
      break;
    case kMajorThirteenthHarmony:
      s << ":maj7.13";
      break;
    case kMinorThirteenthHarmony:
      s << ":m7.13";
      break;

    case kSuspendedSecondHarmony:
      s << ":sus2";
      break;
    case kSuspendedFourthHarmony:
      s << ":sus4";
      break;

/*
 * kNeapolitan f aes des' in:
 *
 * c e g c' -> f f aes des' -> d g d b -> c e g c'

they are three different pre-dominant chords that are taught to American undergrads in a sophomore theory course.

in E major:
Italian = C E A#
French = C E F# A#
German = C E G A#
Tristan = C D# F# A#

in all of them, the C and A# in theory want to fan out to B (the dominant).  This is, of course, in theory - Wagner’s use of the Tristan chord, which he clearly named his opera after, has the A# moving down to A, or the 7th of the dominant (I’m transposing to fit w/ the example above).  Wagner obviously did not pay much attention during his sophomore music theory course…
*/
    case kNeapolitanHarmony:
      s << ":3-.6-%{:Neapolitan%}";
      break;
    case kItalianHarmony:
      s << ":3.6+%{:Italian%}";
      break;
    case kFrenchHarmony:
      s << ":3.5+.6+%{:French%}";
      break;
    case kGermanHarmony:
      s << ":3.5.6+%{:German%}";
      break;

    case kPedalHarmony:
      s << "%{:Pedal%}";
      break;
    case kPowerHarmony:
      s << ":5 %{power%}";
      break;
    case kTristanHarmony:
      s << ":2+.5+.6+%{:Tristan%}";
      break;

    // jazz-specific chords

    case kMinorMajorNinth: // -maj9, minmaj9
      s << ":m9";
      break;

    case kDominantSuspendedFourthHarmony: // 7sus4, domsus4
      s << ":7sus4";
      break;
    case kDominantAugmentedFifthHarmony: // 7#5, domaug5
      s << ":7.5+";
      break;
    case kDominantMinorNinthHarmony: // 7b9, dommin9
      s << ":7.9-";
      break;
    case kDominantAugmentedNinthDiminishedFifthHarmony: // 7#9b5, domaug9dim5
      s << ":7.9+.5-";
      break;
    case kDominantAugmentedNinthAugmentedFifthHarmony: // 7#9#5, domaug9aug5
      s << ":7.9+.5+";
      break;
    case kDominantAugmentedEleventhHarmony: // 7#11, domaug11
      s << ":7.11+";
      break;

    case kMajorSeventhAugmentedEleventhHarmony: // maj7#11, maj7aug11
      s << ":maj7.11+";
      break;

    // other

    case kOtherHarmony:
      s << "%{:Other%}";
      break;

    case kNoneHarmony:
      s << "%{:None%}";
      break;
  } // switch

  // print harmony degrees if any
  list<S_msrHarmonyDegree>
    harmonyDegreesList =
      harmony->getHarmonyDegreesList ();

  if (harmonyDegreesList.size ()) {
    bool thereAreDegreesToBeRemoved = false;

    // print degrees to be added if any first
    for (
      list<S_msrHarmonyDegree>::const_iterator i = harmonyDegreesList.begin ();
      i != harmonyDegreesList.end ();
      i++
    ) {
      S_msrHarmonyDegree harmonyDegree = (*i);

      // get harmony degree information
      int
        harmonyDegreeValue =
          harmonyDegree->getHarmonyDegreeValue ();

      msrAlterationKind
        harmonyDegreeAlterationKind =
          harmonyDegree->
            getHarmonyDegreeAlterationKind ();

      msrHarmonyDegree::msrHarmonyDegreeTypeKind
        harmonyDegreeTypeKind =
          harmonyDegree->
            getHarmonyDegreeTypeKind ();

      // print the harmony degree
      switch (harmonyDegreeTypeKind) {
        case msrHarmonyDegree::kHarmonyDegreeTypeAdd:
          s <<
      // JMI ???      "." <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsLilypondString (
              harmonyDegreeAlterationKind);
          break;

        case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
          s <<
            "." <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsLilypondString (
              harmonyDegreeAlterationKind);
          break;

        case msrHarmonyDegree::kHarmonyDegreeTypeSubstract:
          thereAreDegreesToBeRemoved = true;
          break;
      } // switch
    } // for

    // then print harmony degrees to be removed if any
    if (thereAreDegreesToBeRemoved) {
      s << "^";

      int counter = 0;

      for (
        list<S_msrHarmonyDegree>::const_iterator i = harmonyDegreesList.begin ();
        i != harmonyDegreesList.end ();
        i++
      ) {
        counter++;

        S_msrHarmonyDegree
          harmonyDegree = (*i);

        // get harmony degree information
        int
          harmonyDegreeValue =
            harmonyDegree->getHarmonyDegreeValue ();

        msrAlterationKind
          harmonyDegreeAlterationKind =
            harmonyDegree->
              getHarmonyDegreeAlterationKind ();

        msrHarmonyDegree::msrHarmonyDegreeTypeKind
          harmonyDegreeTypeKind =
            harmonyDegree->
              getHarmonyDegreeTypeKind ();

        // print the harmony degree
        switch (harmonyDegreeTypeKind) {
          case msrHarmonyDegree::kHarmonyDegreeTypeAdd:
          case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
            break;

          case msrHarmonyDegree::kHarmonyDegreeTypeSubstract:
   // JMI         if (counter > 1) {}
          s <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsLilypondString (
              harmonyDegreeAlterationKind);
            break;
        } // switch
      } // for
    }
  }

  // print the harmony bass if relevant
  msrQuarterTonesPitchKind
    harmonyBassQuarterTonesPitchKind =
      harmony->
        getHarmonyBassQuarterTonesPitchKind ();

  if (harmonyBassQuarterTonesPitchKind != k_NoQuarterTonesPitch_QTP) {
    s <<
      "/" <<
      msrQuarterTonesPitchKindAsString (
        gMsrOah->
          fMsrQuarterTonesPitchesLanguageKind,
        harmonyBassQuarterTonesPitchKind);
  }

  // print the harmony inversion if relevant // JMI ???
  int harmonyInversion =
    harmony->getHarmonyInversion ();

  if ( harmonyInversion!= K_HARMONY_NO_INVERSION) {
    s <<
      "%{ inversion: " << harmonyInversion << " %}";
  }

  return s.str ();
}

string lpsr2LilypondTranslator::figureAsLilypondString (
  S_msrFigure figure)
{
  stringstream s;

  fCurrentFiguredBassFiguresCounter++;

  // is the figured bass parenthesized?
  msrFiguredBass::msrFiguredBassParenthesesKind
    figuredBassParenthesesKind =
      fCurrentFiguredBass->
        getFiguredBassParenthesesKind ();

  // generate the figure number
  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      s << "[";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      break;
  } // switch

  s <<
    figure->getFigureNumber ();

  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      s << "]";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      break;
  } // switch

  // handle the figure prefix
  switch (figure->getFigurePrefixKind ()) {
    case msrFigure::k_NoFigurePrefix:
      break;
    case msrFigure::kDoubleFlatPrefix:
      s << "--";
      break;
    case msrFigure::kFlatPrefix:
      s << "-";
      break;
    case msrFigure::kFlatFlatPrefix:
      s << "flat flat";
      break;
    case msrFigure::kNaturalPrefix:
      s << "!";
      break;
    case msrFigure::kSharpSharpPrefix:
      s << "sharp sharp";
      break;
    case msrFigure::kSharpPrefix:
      s << "+";
      break;
    case msrFigure::kDoubleSharpPrefix:
      s << "++";
      break;
  } // switch

  // handle the figure suffix
  switch (figure->getFigureSuffixKind ()) {
    case msrFigure::k_NoFigureSuffix:
      break;
    case msrFigure::kDoubleFlatSuffix:
      s << "double flat";
      break;
    case msrFigure::kFlatSuffix:
      s << "flat";
      break;
    case msrFigure::kFlatFlatSuffix:
      s << "flat flat";
      break;
    case msrFigure::kNaturalSuffix:
      s << "natural";
      break;
    case msrFigure::kSharpSharpSuffix:
      s << "sharp sharp";
      break;
    case msrFigure::kSharpSuffix:
      s << "sharp";
      break;
    case msrFigure::kDoubleSharpSuffix:
      s << "souble sharp";
      break;
    case msrFigure::kSlashSuffix:
      s << "/";
      break;
  } // switch

  // generate a space if not last figure in figured bass
  if (
    fCurrentFiguredBassFiguresCounter
      <
    fCurrentFiguredBass->getFiguredBassFiguresList ().size ()
  ) {
    s << ' ';
  }

  return s.str ();
}

string lpsr2LilypondTranslator::figuredBassAsLilypondString (
  S_msrFiguredBass figuredBass)
{
  int inputLineNumber =
    figuredBass->getInputLineNumber ();

  stringstream s;

  s << "<";

  const list<S_msrFigure>&
    figuredBassFiguresList =
      figuredBass->getFiguredBassFiguresList ();

  if (figuredBassFiguresList.size ()) {
    list<S_msrFigure>::const_iterator
      iBegin = figuredBassFiguresList.begin (),
      iEnd   = figuredBassFiguresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << figureAsLilypondString ((*i));
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";

  // print figured bass duration
  msrTupletFactor
    figuredBassTupletFactor =
      figuredBass->getFiguredBassTupletFactor ();

  if (figuredBassTupletFactor.isEqualToOne ()) { // JMI ???
    // use figured bass sounding whole notes
    s <<
      durationAsLilypondString (
        inputLineNumber,
        figuredBass->
          getMeasureElementSoundingWholeNotes ());
  }
  else {
    // use figured bass display whole notes and tuplet factor
    s <<
      durationAsLilypondString (
        inputLineNumber,
        figuredBass->
          getFiguredBassDisplayWholeNotes ()) <<
      "*" <<
      figuredBassTupletFactor.asRational ();
  }

  s << ' ';

  return s.str ();
}

//________________________________________________________________________
bool compareFrameNotesByDecreasingStringNumber (
  const S_msrFrameNote& first,
  const S_msrFrameNote& second)
{
  return
    first->getFrameNoteStringNumber ()
      >
    second->getFrameNoteStringNumber ();
}

string lpsr2LilypondTranslator::frameAsLilypondString (
  S_msrFrame frame)
{
/* JMI
  int inputLineNumber =
    frame->getInputLineNumber ();
  */

  stringstream s;

  list<S_msrFrameNote>
    frameFrameNotesList =
      frame->getFrameFrameNotesList ();

  const list<msrBarre>&
    frameBarresList =
      frame->getFrameBarresList ();

  int frameStringsNumber =
    frame->getFrameStringsNumber ();
  int frameFretsNumber =
    frame->getFrameFretsNumber ();

  s <<
    "^\\markup {\\fret-diagram #\"";

  // are there fingerings?
  if (frame->getFrameContainsFingerings ()) {
    s <<
      "f:1;";
  }

  // strings number
  if (frameStringsNumber != 6) {
    s <<
      "w:" <<
      frameStringsNumber <<
      ";";
  }

  // frets number
  s <<
    "h:" <<
    frameFretsNumber <<
    ";";

  // frame barres
  if (frameBarresList.size ()) {
    list<msrBarre>::const_iterator
      iBegin = frameBarresList.begin (),
      iEnd   = frameBarresList.end (),
      i      = iBegin;

    for ( ; ; ) {
      msrBarre barre = (*i);

      s <<
        "c:" <<
        barre.fBarreStartString <<
        "-" <<
        barre.fBarreStopString <<
        "-" <<
        barre.fBarreFretNumber <<
        ";";

      if (++i == iEnd) break;
  // JMI    os << ";";
    } // for
  }

  // frame notes
  if (frameFrameNotesList.size ()) {
    // sort the frame notes,
    // necessary both for code generation
    // and the detection of muted (i.e. absent) frame notes
    frameFrameNotesList.sort (
      compareFrameNotesByDecreasingStringNumber);

    int currentStringNumber = frameStringsNumber;

    // generate the code
    list<S_msrFrameNote>::const_iterator
      iBegin = frameFrameNotesList.begin (),
      iEnd   = frameFrameNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrFrameNote
        frameNote = (*i);

      int frameNoteStringNumber =
        frameNote->getFrameNoteStringNumber ();
      int frameNoteFretNumber =
        frameNote->getFrameNoteFretNumber ();
      int frameNoteFingering =
        frameNote->getFrameNoteFingering ();

      while (currentStringNumber > frameNoteStringNumber) {
        // currentStringNumber is missing,
        // hence it is a muted note
        s <<
          currentStringNumber <<
          "-x;";

        currentStringNumber--;
      }

      // generate code for the frame note
      s <<
        frameNoteStringNumber <<
        "-";

      if (frameNoteFretNumber == 0) {
        s <<
          "o";
      }
      else {
        s <<
          frameNoteFretNumber;
      }

      if (frameNoteFingering != -1) {
        s <<
          "-" <<
          frameNoteFingering;
      }

      s <<
        ";";

      currentStringNumber--;

      if (++i == iEnd) break;
  // JMI    os << ";";
    } // for

    // handling low-numbered muted notes
    while (currentStringNumber != 0) {
      // currentStringNumber is missing,
      // hence it is a muted note
      s <<
        currentStringNumber <<
        "-x;";

      currentStringNumber--;
    }
  }

  s <<
    "\" } ";

  return s.str ();
}

void lpsr2LilypondTranslator::generateInputLineNumberAndOrPositionInMeasureAsAComment (
  S_msrMeasureElement measureElement)
{
  fLilypondCodeOstream <<
    "%{ ";

  if (gLpsr2LilypondOah->fInputLineNumbers) {
    // print the input line number as a comment
    fLilypondCodeOstream <<
      "line " << measureElement->getInputLineNumber () << " ";
  }

  if (gLpsr2LilypondOah->fPositionsInMeasures) {
    // print the position in measure as a comment
    fLilypondCodeOstream <<
      "pim: " <<
      measureElement->getMeasureElementPositionInMeasure () <<
      " ";
  }

  fLilypondCodeOstream <<
    "%} ";
}

//________________________________________________________________________
string lpsr2LilypondTranslator::generateAColumnForMarkup (
  string           theString,
  markupColumnKind columnKind)
{
  stringstream s;

  switch (columnKind) {
    case markupColumnKindLeftAligned:
      s << "\\column { ";
      break;
    case markupColumnKindLeftACentered:
      s << "\\center-column { ";
      break;
  } // switch
  s << endl;

  list<string> chunksList;

  splitRegularStringAtEndOfLines (
    theString,
    chunksList);

  if (chunksList.size ()) {
    // generate a markup containing the chunks
    list<string>::const_iterator
      iBegin = chunksList.begin (),
      iEnd   = chunksList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s <<
        "\\line { \"" << (*i) << "\" }";
      if (++i == iEnd) break;
    // JMI ???  s << ' ';
      s << endl;
    } // for

    s <<
      " } ";

    if (gLpsr2LilypondOah->fLilyPondComments) {
      s <<
        "% " <<
        singularOrPlural (
          chunksList.size (), "chunk", "chunks");
    }

    s << endl;
  }

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::generateMultilineMarkup (
  string           theString,
  markupColumnKind columnKind)
{
  stringstream s;

  s <<
    "\\markup { " <<
    generateAColumnForMarkup (
      theString,
      columnKind) <<
    " } " <<
    endl;

  return s.str ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrScore& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // initial empty line in LilyPond code
  // to help copy/paste it
// JMI  fLilypondCodeOstream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrScore& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // final empty line in LilyPond code
  // to help copy/paste it
  fLilypondCodeOstream << endl;
}

    // names

string lpsr2LilypondTranslator::nameAsLilypondString (
  string name)
{
  string result;

  size_t endOfLineFound = name.find ("\n");

  if (endOfLineFound != string::npos) {
    result =
      generateMultilineMarkup (
        name,
        markupColumnKindLeftACentered); // JMI ???
  }
  else {
    result = "\"" + name + "\"";
  }

  return result;
}

string lpsr2LilypondTranslator::lpsrVarValAssocKindAsLilypondString (
  lpsrVarValAssoc::lpsrVarValAssocKind
    lilyPondVarValAssocKind)
{
  string result;

  switch (lilyPondVarValAssocKind) {
    // library

    case lpsrVarValAssoc::kLibraryVersion:
      result = "version";
      break;

    // MusicXML informations

    case lpsrVarValAssoc::kMusicXMLWorkNumber:
      result = "opus";
      break;
    case lpsrVarValAssoc::kMusicXMLWorkTitle:
      result = "title";
      break;
    case lpsrVarValAssoc::kMusicXMLOpus:
      result = "opus";
      break;
    case lpsrVarValAssoc::kMusicXMLMovementNumber:
      result = "movementNumber";
      break;
    case lpsrVarValAssoc::kMusicXMLMovementTitle:
      result = "subtitle";
      break;
    case lpsrVarValAssoc::kMusicXMLEncodingDate:
      result = "encodingDate";
      break;
    case lpsrVarValAssoc::kMusicXMLScoreInstrument:
      result = "scoreInstrument";
      break;
    case lpsrVarValAssoc::kMusicXMLMiscellaneousField:
      result = "miscellaneousField";
      break;

    // LilyPond informations

    case lpsrVarValAssoc::kLilypondDedication:
      result = "dedication";
      break;

    case lpsrVarValAssoc::kLilypondPiece:
      result = "piece";
      break;
    case lpsrVarValAssoc::kLilypondOpus:
      result = "opus";
      break;

    case lpsrVarValAssoc::kLilypondTitle:
      result = "title";
      break;
    case lpsrVarValAssoc::kLilypondSubTitle:
      result = "subtitle";
      break;
    case lpsrVarValAssoc::kLilypondSubSubTitle:
      result = "subsubtitle";
      break;

    case lpsrVarValAssoc::kLilypondInstrument:
      result = "instrument";
      break;
    case lpsrVarValAssoc::kLilypondMeter:
      result = "meter";
      break;

    case lpsrVarValAssoc::kLilypondTagline:
      result = "tagline";
      break;
    case lpsrVarValAssoc::kLilypondCopyright:
      result = "copyright";
      break;

    case lpsrVarValAssoc::kLilypondMyBreak:
      result = "myBreak";
      break;
    case lpsrVarValAssoc::kLilypondMyPageBreak:
      result = "myPageBreak";
      break;
    case lpsrVarValAssoc::kLilypondGlobal:
      result = "global";
      break;
  } // switch

  return result;
}

string lpsr2LilypondTranslator::lpsrVarValAssocAsLilypondString (
  S_lpsrVarValAssoc lpsrVarValAssoc,
  int               fieldNameWidth)
{
  stringstream s;

  s << left <<
    setw (fieldNameWidth) <<
    lpsrVarValAssocKindAsLilypondString (
      lpsrVarValAssoc->
        getLilyPondVarValAssocKind ()) <<
    " = ";

  msrFontStyleKind
    varValFontStyleKind =
      lpsrVarValAssoc->
        getVarValFontStyleKind ();

  bool italicIsNeeded = false;

  switch (varValFontStyleKind) {
    case kFontStyleNone:
      break;
    case kFontStyleNormal:
      break;
    case KFontStyleItalic:
      italicIsNeeded = true;
      break;
    } // switch

  msrFontWeightKind
    varValFontWeightKind =
      lpsrVarValAssoc->
        getVarValFontWeightKind ();

  bool boldIsNeeded = false;

  switch (varValFontWeightKind) {
    case kFontWeightNone:
      break;
    case kFontWeightNormal:
      break;
    case kFontWeightBold:
      boldIsNeeded = true;
      break;
    } // switch

  bool markupIsNeeded = italicIsNeeded || boldIsNeeded;

  if (markupIsNeeded) {
    fLilypondCodeOstream << "\\markup { ";
  }

  if (italicIsNeeded) {
    fLilypondCodeOstream << "\\italic ";
  }
  if (boldIsNeeded) {
    fLilypondCodeOstream << "\\bold ";
  }

  s <<
    "\"" <<
    lpsrVarValAssoc->
      getVariableValue () <<
    "\"";

  if (markupIsNeeded) {
    s << " }";
  }

  return s.str ();
}

string lpsr2LilypondTranslator::lpsrVarValsListAssocKindAsLilypondString (
  lpsrVarValsListAssoc::lpsrVarValsListAssocKind
    lilyPondVarValsListAssocKind)
{
  string result;

  switch (lilyPondVarValsListAssocKind) {
    // MusicXML informations

    case lpsrVarValsListAssoc::kMusicXMLRights:
      result = "rights";
      break;
    case lpsrVarValsListAssoc::kMusicXMLComposer:
      result = "composer";
      break;
    case lpsrVarValsListAssoc::kMusicXMLArranger:
      result = "arranger";
      break;
    case lpsrVarValsListAssoc::kMusicXMLPoet:
      result = "poet";
      break;
    case lpsrVarValsListAssoc::kMusicXMLLyricist:
      result = "lyricist";
      break;
    case lpsrVarValsListAssoc::kMusicXMLTranslator:
      result = "translator";
      break;
    case lpsrVarValsListAssoc::kMusicXMLArtist:
      result = "artist";
      break;
    case lpsrVarValsListAssoc::kMusicXMLSoftware:
      result = "software";
      break;
    } // switch

  return result;
}

void lpsr2LilypondTranslator::generateLpsrVarValsListAssocValues (
  S_lpsrVarValsListAssoc varValsListAssoc)
{
  const list<string>&
    variableValuesList =
      varValsListAssoc->
        getVariableValuesList ();

  switch (variableValuesList.size ()) {
    case 0:
      break;

    case 1:
      // generate a single string
      fLilypondCodeOstream <<
        "\"" <<
        escapeDoubleQuotes (variableValuesList.front ()) <<
        "\"";
      break;

    default:
      // generate a markup containing the chunks
      fLilypondCodeOstream <<
        endl <<
        "\\markup {" <<
        endl;

      gIndenter++;

      fLilypondCodeOstream <<
        "\\column {" <<
        endl;

      gIndenter++;

      list<string>::const_iterator
        iBegin = variableValuesList.begin (),
        iEnd   = variableValuesList.end (),
        i      = iBegin;

      for ( ; ; ) {
        fLilypondCodeOstream <<
          "\"" << (*i) << "\"";
        if (++i == iEnd) break;
        fLilypondCodeOstream << endl;
      } // for

      fLilypondCodeOstream << endl;

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrVarValAssoc& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrVarValAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // generate the comment if needed
  string
    comment =
      elt->getComment ();

  if (comment.size ()) {
    fLilypondCodeOstream <<
      "% " << comment <<
      endl;
  }

  // generate a comment out if needed
  switch (elt->getCommentedKind ()) {
    case lpsrVarValAssoc::kCommentedYes:
      fLilypondCodeOstream << "%";
      break;
    case lpsrVarValAssoc::kCommentedNo:
      break;
  } // switch

  // generate the backslash if needed
  switch (elt->getBackSlashKind ()) {
    case lpsrVarValAssoc::kWithBackSlashYes:
      fLilypondCodeOstream << "\\";
      break;
    case lpsrVarValAssoc::kWithBackSlashNo:
      break;
  } // switch

  lpsrVarValAssoc::lpsrVarValAssocKind
    varValAssocKind =
      elt->getLilyPondVarValAssocKind ();

  string
    lilyPondVarValAssocKindAsLilypondString =
      lpsrVarValAssocKindAsLilypondString (
        varValAssocKind);

  // the largest variable name length in a header is 18 JMI
  int fieldWidth;

  if (fOnGoingHeader) {
    fieldWidth = 18;
  }
  else {
    fieldWidth =
      lilyPondVarValAssocKindAsLilypondString.size ();
  }

  // generate the field name
  fLilypondCodeOstream << left<<
    setw (fieldWidth) <<
    lilyPondVarValAssocKindAsLilypondString;

  switch (elt->getVarValSeparatorKind ()) {
    case lpsrVarValAssoc::kVarValSeparatorSpace:
      fLilypondCodeOstream << ' ';
      break;
    case lpsrVarValAssoc::kVarValSeparatorEqualSign:
      fLilypondCodeOstream << " = ";
      break;
  } // switch

  msrFontStyleKind
    varValFontStyleKind =
      elt->getVarValFontStyleKind ();

  bool italicIsNeeded = false;

  switch (varValFontStyleKind) {
    case kFontStyleNone:
      break;
    case kFontStyleNormal:
      break;
    case KFontStyleItalic:
      italicIsNeeded = true;
      break;
    } // switch

  msrFontWeightKind
    varValFontWeightKind =
      elt->getVarValFontWeightKind ();

  bool boldIsNeeded = false;

  switch (varValFontWeightKind) {
    case kFontWeightNone:
      break;
    case kFontWeightNormal:
      break;
    case kFontWeightBold:
      boldIsNeeded = true;
      break;
    } // switch

  bool markupIsNeeded = italicIsNeeded || boldIsNeeded;

  if (markupIsNeeded) {
    fLilypondCodeOstream << "\\markup { ";
  }

  if (italicIsNeeded) {
    fLilypondCodeOstream << "\\italic ";
  }
  if (boldIsNeeded) {
    fLilypondCodeOstream << "\\bold ";
  }

  // generate the quote if needed
  lpsrVarValAssoc::lpsrQuotesKind
    quotesKind =
      elt->getQuotesKind ();

  switch (quotesKind) {
    case lpsrVarValAssoc::kQuotesAroundValueYes:
      fLilypondCodeOstream << "\"";
      break;
    case lpsrVarValAssoc::kQuotesAroundValueNo:
      break;
  } // switch

  // generate the value and unit if any
  if (elt->getUnit ().size ()) {
    fLilypondCodeOstream <<
      setprecision (2) <<
      elt->getVariableValue ();

    fLilypondCodeOstream <<
      "\\" <<
      elt->getUnit ();
  }
  else {
    fLilypondCodeOstream <<
      elt->getVariableValue ();
  }

  // generate the quote if needed
  switch (quotesKind) {
    case lpsrVarValAssoc::kQuotesAroundValueYes:
      fLilypondCodeOstream << "\"";
      break;
    case lpsrVarValAssoc::kQuotesAroundValueNo:
      break;
  } // switch

  if (markupIsNeeded) {
    fLilypondCodeOstream << " }";
  }

  fLilypondCodeOstream << endl;

  // generate the end line(s) if needed
  switch (elt->getEndlKind ()) {
    case lpsrVarValAssoc::kEndlNone:
      break;
    case lpsrVarValAssoc::kEndlOnce:
      fLilypondCodeOstream << endl;
      break;
    case lpsrVarValAssoc::kEndlTwice:
      fLilypondCodeOstream <<
        endl <<
        endl;
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrVarValAssoc& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrVarValAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrVarValsListAssoc& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrVarValsListAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // the largest variable name length in a header is 18 JMI
  int fieldWidth;

  string
    lilyPondVarValsListAssocKindAsString =
      elt->lilyPondVarValsListAssocKindAsString ();

  if (fOnGoingHeader) {
    fieldWidth = 18;
  }
  else {
    fieldWidth =
      lilyPondVarValsListAssocKindAsString.size ();
  }

  fLilypondCodeOstream << left<<
    setw (fieldWidth) <<
    lpsrVarValsListAssocKindAsLilypondString (
      elt->getVarValsListAssocKind ());

  fLilypondCodeOstream << " = ";

  generateLpsrVarValsListAssocValues (elt);

  fLilypondCodeOstream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrVarValsListAssoc& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrVarValsListAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrSchemeVariable& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrSchemeVariable" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  string
    comment =
      elt->getComment ();

  if (comment.size ()) {
    fLilypondCodeOstream <<
      "% " << comment <<
      endl;
  }

  switch (elt->getCommentedKind ()) {
    case lpsrSchemeVariable::kCommentedYes:
      fLilypondCodeOstream << "% ";
      break;
    case lpsrSchemeVariable::kCommentedNo:
      break;
  } // switch

  fLilypondCodeOstream <<
    "#(" <<
    elt->getVariableName () <<
    ' ' <<
    elt->getVariableValue () <<
    ")";

  switch (elt->getEndlKind ()) {
    case lpsrSchemeVariable::kEndlNone:
      break;
    case lpsrSchemeVariable::kEndlOnce:
      fLilypondCodeOstream << endl;
      break;
    case lpsrSchemeVariable::kEndlTwice:
      fLilypondCodeOstream <<
        endl <<
        endl;
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrSchemeVariable& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrSchemeVariable" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//______________________________________________________________________________
string lpsr2LilypondTranslator::lengthUnitAsLilypondString (
  msrLengthUnitKind lengthUnitKind)
{
  string result;

  switch (lengthUnitKind) {
    case kInchUnit:
      result = "\\in";
      break;
    case kCentimeterUnit:
      result = "\\cm";
      break;
    case kMillimeterUnit:
      result = "\\mm";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrHeader& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrHeader" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\header {" <<
    endl;

  gIndenter++; // decremented in visitEnd (S_lpsrHeader&)

  // generate header elements

  int fieldNameWidth =
    elt->maxLilypondVariablesNamesLength ();

  // MusicXML informations JMI ???

  // LilyPond informations

  {
    // dedication
    S_lpsrVarValAssoc
      lilypondDedication =
        elt->getLilypondDedication ();

    if (lilypondDedication) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondDedication,
          fieldNameWidth) <<
        endl;
    }
  }

  {
    // piece
    S_lpsrVarValAssoc
      lilypondPiece =
        elt->getLilypondPiece ();

    if (lilypondPiece) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondPiece,
          fieldNameWidth) <<
        endl;
    }
  }

  {
    // opus
    S_lpsrVarValAssoc
      lilypondOpus =
        elt->getLilypondOpus ();

    if (lilypondOpus) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondOpus,
          fieldNameWidth) <<
        endl;
    }
  }

  {
    // title
    if (gMxmlTree2MsrOah->fUseFilenameAsWorkTitle) {
      fLilypondCodeOstream << left <<
        setw (fieldNameWidth) <<
        "title = \"" <<
        gOahOah->fInputSourceName <<
        "\"" <<
        endl;
    }
    else {
      S_lpsrVarValAssoc
        lilypondTitle =
          elt->getLilypondTitle ();

      if (lilypondTitle) {
        fLilypondCodeOstream <<
          lpsrVarValAssocAsLilypondString (
            lilypondTitle,
            fieldNameWidth) <<
          endl;
      }
    }

    /* JMI
    S_msrLength
      indent =
        elt->getIndent ();

    if (! indent) {
      fLilypondCodeOstream << "%";
    }
    fLilypondCodeOstream << left <<
      setw (fieldWidth) <<
      "indent" << " = ";
    if (indent) {
      fLilypondCodeOstream <<
        setprecision (3) << indent->getLengthValue () <<
        lengthUnitAsLilypondString (indent->getLengthUnitKind ());
    }
    else {
      fLilypondCodeOstream <<
        "0.0" <<
        lengthUnitAsLilypondString (defaultLengthUnit);
    }
    fLilypondCodeOstream << endl;
*/

    // subtitle
    S_lpsrVarValAssoc
      lilypondSubTitle =
        elt->getLilypondSubTitle ();

    if (lilypondSubTitle) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondSubTitle,
          fieldNameWidth) <<
        endl;
    }

    S_lpsrVarValAssoc
      lilypondSubSubTitle =
        elt->getLilypondSubSubTitle ();

    if (lilypondSubSubTitle) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondSubSubTitle,
          fieldNameWidth) <<
        endl;
    }
  }

  {
    // instrument
    S_lpsrVarValAssoc
      lilypondInstrument =
        elt->getLilypondInstrument ();

    if (lilypondInstrument) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondInstrument,
          fieldNameWidth) <<
        endl;
    }

    S_lpsrVarValAssoc
      lilypondMeter =
        elt->getLilypondMeter ();

    if (lilypondMeter) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondMeter,
          fieldNameWidth) <<
        endl;
    }
  }

  {
    // copyright
    S_lpsrVarValAssoc
      lilypondCopyright =
        elt->getLilypondCopyright ();

    if (lilypondCopyright) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondCopyright,
          fieldNameWidth) <<
        endl;
    }

    S_lpsrVarValAssoc
      lilypondTagline =
        elt->getLilypondTagline ();

    if (lilypondTagline) {
      fLilypondCodeOstream <<
        lpsrVarValAssocAsLilypondString (
          lilypondTagline,
          fieldNameWidth) <<
        endl;
    }
  }

  fOnGoingHeader = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrHeader& elt)
{
  gIndenter--; // incremented in visitStart (S_lpsrHeader&)

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrHeader" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "}";

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      " % header";
  }

  fLilypondCodeOstream <<
    endl <<
    endl;

  fOnGoingHeader = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generatePaperPageSize (
  S_msrPageLayout   pageLayout,
  msrLengthUnitKind defaultLengthUnit,
  int               fieldWidth)
{
  // paper height
  msrLength
    paperHeight;

  if (gLpsrOah->fPaperHeight.getLengthValue () >= 0) { // JMI BLARK
    paperHeight =
      gLpsrOah->fPaperHeight;
  }
  else {
    paperHeight =
      * (pageLayout->getPageHeight ()); // BLARK
  }

  if (paperHeight.getLengthValue () < 0.0) {
    fLilypondCodeOstream << "%";
  }
  fLilypondCodeOstream << left <<
    setw (fieldWidth) <<
    "paper-height" << " = ";
  if (paperHeight.getLengthValue () >= 0.0) {
    fLilypondCodeOstream <<
      setprecision (3) << paperHeight.getLengthValue () <<
      lengthUnitAsLilypondString (paperHeight.getLengthUnitKind ());
  }
  else {
    fLilypondCodeOstream <<
      "297.0" <<
      lengthUnitAsLilypondString (defaultLengthUnit);
  }
  fLilypondCodeOstream << endl;

  // paper width
  msrLength
    paperWidth;

  if (gLpsrOah->fPaperWidth.getLengthValue () >= 0) { // JMI BLARK
    paperWidth =
      gLpsrOah->fPaperWidth;
  }
  else {
    paperWidth =
      * (pageLayout->getPageWidth ()); // BLARK
  }

  if (paperWidth.getLengthValue () < 0.0) {
    fLilypondCodeOstream << "%";
  }
  fLilypondCodeOstream << left <<
    setw (fieldWidth) <<
    "paper-width" << " = ";
  if (paperWidth.getLengthValue () >= 0.0) {
    fLilypondCodeOstream <<
      setprecision (3) << paperWidth.getLengthValue () <<
      lengthUnitAsLilypondString (paperWidth.getLengthUnitKind ());
  }
  else {
    fLilypondCodeOstream <<
      "210.0" <<
      lengthUnitAsLilypondString (defaultLengthUnit);
  }
  fLilypondCodeOstream << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPaper& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrPaper" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/*
  we could generate the LPSR paper code a second time
  with an option JMI ???
*/

  if (! fOnGoingBookPartBlock) {
    // get MSR page layout
    S_msrPageLayout
      pageLayout =
        elt->getPageLayout ();

    // default length unit
    const msrLengthUnitKind
      defaultLengthUnit = kMillimeterUnit; // JMI

    fLilypondCodeOstream <<
      "\\paper" << " {" <<
      endl;

    gIndenter++;

    const int fieldWidth = 30;

    // page size
    generatePaperPageSize (
      pageLayout,
      defaultLengthUnit,
      fieldWidth);

    // separator
    fLilypondCodeOstream << endl;

    // margins
    {
      // left margin
      bool   commentOutLeftMargin = false;
      float  leftMarginValue = 0.0; // JMI
      string leftMarginUnitString =
               lengthUnitAsLilypondString (defaultLengthUnit);

      if (pageLayout) {
        S_msrMargin
          leftMargin =
            pageLayout->getSingleLeftMargin ();

        if (leftMargin) {
          leftMarginValue =
            leftMargin->getMarginLength ().getLengthValue ();

          leftMarginUnitString =
            lengthUnitAsLilypondString (
              leftMargin->getMarginLength ().getLengthUnitKind ());
        }
        else {
          commentOutLeftMargin = true;
        }
      }
      else {
        commentOutLeftMargin = true;
      }

      if (commentOutLeftMargin) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "left-margin" << " = " <<
        setprecision (3) << leftMarginValue <<
        leftMarginUnitString <<
        endl;

      // right margin
      bool   commentOutRightMargin = false;
      float  rightMarginValue = 0.0; // JMI
      string rightMarginUnitString =
               lengthUnitAsLilypondString (defaultLengthUnit);

      if (pageLayout) {
        S_msrMargin
          rightMargin =
            pageLayout->getSingleRightMargin ();

        if (rightMargin) {
          rightMarginValue =
            rightMargin->getMarginLength ().getLengthValue ();

          rightMarginUnitString =
            lengthUnitAsLilypondString (
              rightMargin->getMarginLength ().getLengthUnitKind ());
        }
        else {
          commentOutRightMargin = true;
        }
      }
      else {
        commentOutRightMargin = true;
      }

      if (commentOutRightMargin) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "right-margin" << " = " <<
        setprecision (3) << rightMarginValue <<
        rightMarginUnitString <<
        endl;

      // top margin
      bool   commentOutTopMargin = false;
      float  topMarginValue = 0.0; // JMI
      string topMarginUnitString =
               lengthUnitAsLilypondString (defaultLengthUnit);

      if (pageLayout) {
        S_msrMargin
          topMargin =
            pageLayout->getSingleTopMargin ();

        if (topMargin) {
          topMarginValue =
            topMargin->getMarginLength ().getLengthValue ();

          topMarginUnitString =
            lengthUnitAsLilypondString (
              topMargin->getMarginLength ().getLengthUnitKind ());
        }
        else {
          commentOutTopMargin = true;
        }
      }
      else {
        commentOutTopMargin = true;
      }

      if (commentOutTopMargin) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "top-margin" << " = " <<
        setprecision (3) << topMarginValue <<
        topMarginUnitString <<
        endl;

      // bottom margin
      bool   commentOutBottomMargin = false;
      float  bottomMarginValue = 0.0; // JMI
      string bottomMarginUnitString =
               lengthUnitAsLilypondString (defaultLengthUnit);

      if (pageLayout) {
        S_msrMargin
          bottomMargin =
            pageLayout->getSingleBottomMargin ();

        if (bottomMargin) {
          bottomMarginValue =
            bottomMargin->getMarginLength ().getLengthValue ();

          bottomMarginUnitString =
            lengthUnitAsLilypondString (
              bottomMargin->getMarginLength ().getLengthUnitKind ());
        }
        else {
          commentOutBottomMargin = true;
        }
      }
      else {
        commentOutBottomMargin = true;
      }

      if (commentOutBottomMargin) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "bottom-margin" << " = " <<
        setprecision (3) << bottomMarginValue <<
        bottomMarginUnitString <<
        endl;
    }

    // separator
    fLilypondCodeOstream << endl;

    // indents
    {
      // horizontal shift
      S_msrLength
        horizontalShift =
          elt->getHorizontalShift ();

      if (! horizontalShift) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "horizontal-shift" << " = ";
      if (horizontalShift) {
        fLilypondCodeOstream <<
          setprecision (3) << horizontalShift->getLengthValue () <<
          lengthUnitAsLilypondString (horizontalShift->getLengthUnitKind ());
      }
      else {
        fLilypondCodeOstream <<
          "0.0" <<
          lengthUnitAsLilypondString (defaultLengthUnit);
      }
      fLilypondCodeOstream << endl;

      // indent
      S_msrLength
        indent =
          elt->getIndent ();

      if (! indent) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "indent" << " = ";
      if (indent) {
        fLilypondCodeOstream <<
          setprecision (3) << indent->getLengthValue () <<
          lengthUnitAsLilypondString (indent->getLengthUnitKind ());
      }
      else {
        fLilypondCodeOstream <<
          "0.0" <<
          lengthUnitAsLilypondString (defaultLengthUnit);
      }
      fLilypondCodeOstream << endl;

      // short indent
      S_msrLength
        shortIndent =
          elt->getShortIndent ();

      if (! shortIndent) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "short-indent" << " = ";
      if (shortIndent) {
        fLilypondCodeOstream <<
          setprecision (3) << shortIndent->getLengthValue () <<
          lengthUnitAsLilypondString (shortIndent->getLengthUnitKind ());
      }
      else {
        fLilypondCodeOstream <<
          "0.0" <<
          lengthUnitAsLilypondString (defaultLengthUnit);
      }
      fLilypondCodeOstream << endl;
    }

    // separator
    fLilypondCodeOstream << endl;

    // spaces
    {
      // markup system spacing padding
      S_msrLength
        markupSystemPpacingPadding =
          elt->getMarkupSystemSpacingPadding ();

      if (! markupSystemPpacingPadding) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "markup-system-spacing.padding" << " = ";
      if (markupSystemPpacingPadding) {
        fLilypondCodeOstream <<
          setprecision (3) << markupSystemPpacingPadding->getLengthValue () <<
          lengthUnitAsLilypondString (markupSystemPpacingPadding->getLengthUnitKind ());
      }
      else {
        fLilypondCodeOstream <<
          "0.0" <<
          lengthUnitAsLilypondString (defaultLengthUnit);
      }
      fLilypondCodeOstream << endl;

      // between system space
      S_msrLength
        betweenSystemSpace =
          elt->getBetweenSystemSpace ();

      if (! betweenSystemSpace) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "between-system-space" << " = ";
      if (betweenSystemSpace) {
        fLilypondCodeOstream <<
          setprecision (3) << betweenSystemSpace->getLengthValue () <<
          lengthUnitAsLilypondString (betweenSystemSpace->getLengthUnitKind ());
      }
      else {
        fLilypondCodeOstream <<
          "0.0" <<
          lengthUnitAsLilypondString (defaultLengthUnit);
      }
      fLilypondCodeOstream << endl;

      // page top space
      S_msrLength
        pageTopSpace =
          elt->getPageTopSpace ();

      if (! pageTopSpace) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "page-top-space" << " = ";
      if (pageTopSpace) {
        fLilypondCodeOstream <<
          setprecision (3) << pageTopSpace->getLengthValue () <<
          lengthUnitAsLilypondString (pageTopSpace->getLengthUnitKind ());
      }
      else {
        fLilypondCodeOstream <<
          "0.0" <<
          lengthUnitAsLilypondString (defaultLengthUnit);
      }
      fLilypondCodeOstream << endl;
    }

    // separator
    fLilypondCodeOstream << endl;

    // counts
    {
      // page count
      int
        pageCount =
          elt->getPageCount ();

      if (pageCount < 0) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "page-count" << " = " <<
         pageCount <<
         endl;

      // system count
      int
        systemCount =
          elt->getSystemCount ();

      if (systemCount < 0) {
        fLilypondCodeOstream << "%";
      }
      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "system-count" << " = " <<
         systemCount <<
         endl;
    }

    // separator
    fLilypondCodeOstream << endl;

    // headers and footers
    {
      string oddHeaderMarkup =
        elt->getOddHeaderMarkup ();

      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "oddHeaderMarkup" << " = ";
      if (oddHeaderMarkup.size ()) {
        fLilypondCodeOstream <<
          oddHeaderMarkup;
      }
      else {
        fLilypondCodeOstream << "\"\"";
      }
      fLilypondCodeOstream << endl;

      string evenHeaderMarkup =
        elt->getEvenHeaderMarkup ();

      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "evenHeaderMarkup" << " = ";
      if (evenHeaderMarkup.size ()) {
        fLilypondCodeOstream <<
          evenHeaderMarkup;
      }
      else {
        fLilypondCodeOstream << "\"\"";
      }
      fLilypondCodeOstream << endl;

      string oddFooterMarkup =
        elt->getOddFooterMarkup ();

      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "oddFooterMarkup" << " = ";
      if (oddFooterMarkup.size ()) {
        fLilypondCodeOstream <<
          oddFooterMarkup;
      }
      else {
        fLilypondCodeOstream << "\"\"";
      }
      fLilypondCodeOstream << endl;

      string evenFooterMarkup =
        elt->getEvenFooterMarkup ();

      fLilypondCodeOstream << left <<
        setw (fieldWidth) <<
        "evenFooterMarkup" << " = ";
      if (evenFooterMarkup.size ()) {
        fLilypondCodeOstream <<
          evenFooterMarkup;
      }
      else {
        fLilypondCodeOstream << "\"\"";
      }
      fLilypondCodeOstream << endl;
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPaper& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrPaper" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (! fOnGoingBookPartBlock) {
    gIndenter--;

    fLilypondCodeOstream <<
      "}";

    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        " % paper";
    }

    fLilypondCodeOstream <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrLayout& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\layout {" <<
    endl;

  gIndenter++;

  // score context
  fLilypondCodeOstream <<
    "\\context {" <<
    endl <<
    gTab << "\\Score" <<
    endl <<
    gTab << "autoBeaming = ##f % to display tuplets brackets" <<
    endl <<
    "}" <<
    endl;

  // voice context
  fLilypondCodeOstream <<
    "\\context {" <<
    endl <<
    gTab << "\\Voice" <<
    endl;

  if (gLpsr2LilypondOah->fAmbitusEngraver) {
    fLilypondCodeOstream <<
        gTab << "\\consists \"Ambitus_engraver\"" <<
        endl;
  }

  if (gLpsr2LilypondOah->fCustosEngraver) {
    fLilypondCodeOstream <<
        gTab << "\\consists \"Custos_engraver\"" <<
        endl;
  }

  fLilypondCodeOstream <<
    "}" <<
    endl;

  // ChordNames context
  if (fVisitedLpsrScore->getJazzChordsDisplayIsNeeded ()) {
    fLilypondCodeOstream <<
      gIndenter.indentMultiLineString (
R"(\context {
  \ChordNames
  chordNameExceptions = \chExceptions
  slashChordSeparator = \markup {
    % the \hspace commands simulate kerning
    \hspace #-.7
    \fontsize #1 \lower #1.2 \rotate #-10 "/"
    \hspace #-.1
  }
  chordNoteNamer = #lower-extension
})") <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrLayout& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (gLpsr2LilypondOah->fRepeatBrackets) {
    fLilypondCodeOstream <<
      "\\context " "{" <<
      endl;

    gIndenter++;

    fLilypondCodeOstream <<
      "\\Score" <<
      endl <<
      "% defaultBarType = #\"!\"" <<
      endl <<
      "startRepeatType = #\"[|:\"" <<
      endl <<
      "endRepeatType = #\":|]\"" <<
      endl <<
      "doubleRepeatType = #\":|][|:\"" <<
      endl;

    gIndenter--;

    fLilypondCodeOstream <<
      "}" <<
      endl;
  }

  if (false) { // JMI XXL
    fLilypondCodeOstream <<
      "\\context {" <<
      endl;

    gIndenter++;

    fLilypondCodeOstream <<
      "\\Staff" <<
      endl <<
      "\\consists \"Span_arpeggio_engraver\"" <<
      endl;

    gIndenter--;

    fLilypondCodeOstream <<
      "}";

    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        " % layout";
    }

    fLilypondCodeOstream << endl;
  }

  gIndenter--;

  fLilypondCodeOstream <<
    "}";

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      " % layout";
  }

  fLilypondCodeOstream <<
    endl <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrBookBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrBookBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\book {" <<
    endl;

  gIndenter++;

  fOnGoingBookPartBlock = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrBookBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrBookBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  fLilypondCodeOstream <<
    "}";

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      " % book";
  }

  fLilypondCodeOstream << endl;

  fOnGoingBookPartBlock = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrScoreBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrScoreBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\score {" <<
    endl;

  gIndenter++;

  fOnGoingScoreBlock = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrScoreBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrScoreBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  fLilypondCodeOstream <<
    "}";

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      " % score";
  }

  fLilypondCodeOstream <<
    endl << // JMI
    endl;

  fOnGoingScoreBlock = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrBookPartBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrBookPartBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\bookpart {" <<
    endl;

  gIndenter++;

  fOnGoingBookPartBlock = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrBookPartBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrBookPartBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  fLilypondCodeOstream <<
    "}";

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      " % bookpart";
  }

  fLilypondCodeOstream <<
    endl <<
    endl;

  fOnGoingBookPartBlock = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrParallelMusicBLock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrParallelMusicBLock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fNumberOfPartGroupBlocks =
    elt->
      getParallelMusicBLockPartGroupBlocks ().size ();

  if (fNumberOfPartGroupBlocks) {
    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        "<<" <<
        "% parallel music";
    }

    else {
      fLilypondCodeOstream <<
        "<<";
    }

    fLilypondCodeOstream << endl;

    gIndenter++;
  }

  fCurrentParallelMusicBLock = elt;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrParallelMusicBLock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrParallelMusicBLock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream << endl;

  if (fNumberOfPartGroupBlocks) {
    gIndenter--;

    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        ">>" <<
        "% parallel music";
    }

    else {
      fLilypondCodeOstream <<
        ">>";
    }

    fLilypondCodeOstream << endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPartGroupBlock& elt)
{
  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrPartGroupBlock for '" <<
      partGroup->asShortString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fPartGroupBlocksCounter++;

  fNumberOfPartGroupBlockElements =
    elt -> getPartGroupBlockElements ().size ();

// JMI  fLilypondCodeOstream << endl << endl << partGroup << endl << endl;

  msrPartGroup::msrPartGroupImplicitKind
    partGroupImplicitKind =
      partGroup->
        getPartGroupImplicitKind ();

  msrPartGroup::msrPartGroupSymbolKind
    partGroupSymbolKind =
      partGroup->
        getPartGroupSymbolKind ();

  msrPartGroup::msrPartGroupBarlineKind
    partGroupBarlineKind =
      partGroup->
        getPartGroupBarlineKind ();

  string
    partGroupName =
      partGroup->
        getPartGroupName (),

    partGroupAbbreviation =
      partGroup->
        getPartGroupAbbreviation (),

    partGroupInstrumentName =
      partGroup->
        getPartGroupInstrumentName ();

  // LPNR, page 567 jMI ???

  switch (partGroupImplicitKind) {
    case msrPartGroup::kPartGroupImplicitYes:
      // don't generate code for an implicit top-most part group block
      break;

    case msrPartGroup::kPartGroupImplicitNo:
      if (gLpsr2LilypondOah->fLilyPondComments) {
        fLilypondCodeOstream << left <<
          setw (commentFieldWidth);
      }

      switch (partGroupSymbolKind) {
        case msrPartGroup::kPartGroupSymbolNone:
          fLilypondCodeOstream <<
            "\\new StaffGroup";
          break;

        case msrPartGroup::kPartGroupSymbolBrace: // JMI
          switch (partGroupBarlineKind) {
            case msrPartGroup::kPartGroupBarlineYes:
              fLilypondCodeOstream <<
                "\\new PianoStaff";
              break;
            case msrPartGroup::kPartGroupBarlineNo:
              fLilypondCodeOstream <<
                "\\new GrandStaff";
              break;
          } // switch
          break;

        case msrPartGroup::kPartGroupSymbolBracket:
          switch (partGroupBarlineKind) {
            case msrPartGroup::kPartGroupBarlineYes:
              fLilypondCodeOstream <<
                "\\new StaffGroup";
              break;
            case msrPartGroup::kPartGroupBarlineNo:
              fLilypondCodeOstream <<
                "\\new ChoirStaff";
              break;
          } // switch
          break;

        case msrPartGroup::kPartGroupSymbolLine:
          fLilypondCodeOstream <<
            "\\new StaffGroup";
          break;

        case msrPartGroup::kPartGroupSymbolSquare:
          fLilypondCodeOstream <<
            "\\new StaffGroup";
          break;
      } // switch

#ifdef TRACE_OAH
      if (gTraceOah->fTracePartGroups) {
         fLilypondCodeOstream <<
          " %{ " <<
          partGroup->getPartGroupCombinedName () <<
          ", abs number: " <<
          partGroup->getPartGroupAbsoluteNumber () <<
          " %} ";
      }
#endif

      // should a '\with' block be generated?
      bool doGenerateAWithBlock = false;

      // generate the '\with' block if there's
      // a part group name or abbreviation to be generated
      if (
        partGroupName.size ()
          ||
        partGroupAbbreviation.size ()
      ) {
        doGenerateAWithBlock = true;
      }

      // generate the '\with' block
      // if the part group is not implicit
      switch (partGroupImplicitKind) {
        case msrPartGroup::kPartGroupImplicitYes:
          break;
        case msrPartGroup::kPartGroupImplicitNo:
          if (partGroupName.size ()) {
            doGenerateAWithBlock = true;
          }
          break;
      } // switch

      // generate the '\with' block
      // if the part group symbol is a line or square
      switch (partGroupSymbolKind) {
        case msrPartGroup::kPartGroupSymbolNone:
          break;

        case msrPartGroup::kPartGroupSymbolBrace: // JMI
          break;

        case msrPartGroup::kPartGroupSymbolBracket:
          break;

        case msrPartGroup::kPartGroupSymbolLine:
          doGenerateAWithBlock = true;
          break;

        case msrPartGroup::kPartGroupSymbolSquare:
          doGenerateAWithBlock = true;
          break;
      } // switch

      if (doGenerateAWithBlock) {
        fLilypondCodeOstream <<
          endl <<
          "\\with {" <<
          endl;
      }

      gIndenter++;

      if (doGenerateAWithBlock) { // JMI
      /* ??? JMI
        if (partGroupName.size ()) {
          fLilypondCodeOstream <<
            "instrumentName = " <<
            nameAsLilypondString (partGroupName) <<
            endl;
        }
        if (partGroupAbbreviation.size ()) {
          fLilypondCodeOstream <<
            "shortInstrumentName = " <<
            nameAsLilypondString (partGroupAbbreviation) <<
            endl;
        }
        */
      }

      switch (partGroupSymbolKind) {
        case msrPartGroup::kPartGroupSymbolNone:
          break;

        case msrPartGroup::kPartGroupSymbolBrace: // JMI
          /*
           *
           * check whether individual part have instrument names JMI
           *
            if (partGroupInstrumentName.size ()) {
              fLilypondCodeOstream << = "\\new PianoStaff";
            }
            else {
              fLilypondCodeOstream << = "\\new GrandStaff";
            }
              */
          break;

        case msrPartGroup::kPartGroupSymbolBracket:
          break;

        case msrPartGroup::kPartGroupSymbolLine:
          fLilypondCodeOstream <<
            "systemStartDelimiter = #'SystemStartBar" <<
            endl;
          break;

        case msrPartGroup::kPartGroupSymbolSquare:
          fLilypondCodeOstream <<
            "systemStartDelimiter = #'SystemStartSquare" <<
            endl;
          break;
      } // switch

      gIndenter--;

      // generate the '\with' block ending
      // if the part group is not implicit
      if (doGenerateAWithBlock) {
        fLilypondCodeOstream <<
          "}" <<
          endl;
      }

      if (gLpsr2LilypondOah->fLilyPondComments) {
        fLilypondCodeOstream << left <<
          setw (commentFieldWidth) <<
          " <<" << "% part group " <<
          partGroup->getPartGroupCombinedNameWithoutEndOfLines ();
      }
      else {
        fLilypondCodeOstream <<
          " <<";
      }

      fLilypondCodeOstream << endl;
      break;
  } // switch

  if (partGroupInstrumentName.size ()) { // JMI
    fLilypondCodeOstream <<
      "instrumentName = \"" <<
      partGroupInstrumentName <<
      "\"" <<
      endl;
  }

  if (gLpsr2LilypondOah->fConnectArpeggios) {
    fLilypondCodeOstream <<
      "\\set PianoStaff.connectArpeggios = ##t" <<
      endl;
  }

  fLilypondCodeOstream << endl;

  if (elt->getPartGroupBlockElements ().size () > 1) {
    gIndenter++;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPartGroupBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrPartGroupBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();

  if (elt->getPartGroupBlockElements ().size () > 1) {
    gIndenter--;
  }

  switch (partGroup->getPartGroupImplicitKind ()) {
    case msrPartGroup::kPartGroupImplicitYes:
      // don't generate code for an implicit top-most part group block
      break;

    case msrPartGroup::kPartGroupImplicitNo:
      if (gLpsr2LilypondOah->fLilyPondComments) {
        fLilypondCodeOstream << left <<
          setw (commentFieldWidth) << ">>" <<
          "% part group " <<
          partGroup->getPartGroupCombinedNameWithoutEndOfLines ();
      }
      else {
        fLilypondCodeOstream <<
          ">>";
      }

      fLilypondCodeOstream << endl;

      if (fPartGroupBlocksCounter != fNumberOfPartGroupBlocks) {
        fLilypondCodeOstream << endl;
      }
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPartBlock& elt)
{
  // fetch part block's part
  S_msrPart
    part =
      elt->getPart ();

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrPartBlock for '" <<
      part->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fPartGroupBlockElementsCounter++;

  fNumberOfStaffBlocksElements =
    elt->getPartBlockElementsList ().size ();

  if (part->getPartStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave

    string
      partName =
        part->getPartName (),
      partAbbreviation =
        part->getPartAbbreviation ();
        /*
    string
      partInstrumentName =  // JMI
        part->getPartInstrumentName (),
      partInstrumentAbbreviation =  // JMI
        part->getPartInstrumentAbbreviation ();
        */

    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        "\\new PianoStaff" <<
        " % part " << part->getPartCombinedName ();
    }
    else {
      fLilypondCodeOstream <<
        "\\new PianoStaff";
    }
    fLilypondCodeOstream << endl;

    // generate the 'with' block beginning
    fLilypondCodeOstream <<
      "\\with {" <<
      endl;

    gIndenter++;

/* JMI
    if (partName.size ()) {
      fLilypondCodeOstream <<
        "instrumentName = \"" <<
        partName <<
        "\"" <<
        endl;
    }
    if (partAbbreviation.size ()) {
      fLilypondCodeOstream <<
        "shortInstrumentName = " <<
       nameAsLilypondString (partAbbreviation) <<
        endl;
    }
*/

    if (gLpsr2LilypondOah->fConnectArpeggios) {
      fLilypondCodeOstream <<
        "connectArpeggios = ##t" <<
        endl;
    }

    gIndenter--;

    // generate the 'with' block ending
    fLilypondCodeOstream <<
      "}" <<
      endl;

    fLilypondCodeOstream <<
      "<<" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPartBlock& elt)
{
  // fetch current part block's part
  S_msrPart
    part =
      elt->getPart ();

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrPartBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (part->getPartStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave
    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) << ">>" <<
        "% part " <<
        part->getPartCombinedName ();
    }
    else {
      fLilypondCodeOstream <<
        ">>";
    }

    fLilypondCodeOstream << endl;

    if (fPartGroupBlockElementsCounter != fNumberOfPartGroupBlockElements) {
      fLilypondCodeOstream << endl;
    }
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrStaffBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrStaffBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fStaffBlocksCounter++;

  S_msrStaff
    staff =
      elt->getStaff ();

  // generate the staff context
  switch (staff->getStaffKind ()) {
    case msrStaff::kStaffRegular:
      if (gLpsr2LilypondOah->fJianpu) {
        fLilypondCodeOstream << "\\new JianpuStaff";
      }
      else {
        fLilypondCodeOstream << "\\new Staff";
      }
      break;

    case msrStaff::kStaffTablature:
      fLilypondCodeOstream << "\\new TabStaff";
      break;

    case msrStaff::kStaffHarmony:
      fLilypondCodeOstream << "\\new kStaffHarmony???";
      break;

    case msrStaff::kStaffFiguredBass:
      fLilypondCodeOstream << "\\new FiguredBassStaff???";
      break;

    case msrStaff::kStaffDrum:
      fLilypondCodeOstream << "\\new DrumStaff";
      break;

    case msrStaff::kStaffRythmic:
      fLilypondCodeOstream << "\\new RhythmicStaff";
      break;
    } // switch

  fLilypondCodeOstream <<
    " = \"" <<
    staff->getStaffName () <<
    "\"";

  fLilypondCodeOstream << endl;

  // generate the 'with' block beginning
  fLilypondCodeOstream <<
    "\\with {" <<
    endl;

  gIndenter++;

  // fetch part upLink
  S_msrPart
    staffPartUpLink =
      staff->getStaffPartUpLink ();

  // don't generate instrument names in the staves
  // if the containing part contains several of them
  if (staffPartUpLink->getPartStavesMap ().size () == 1) {
    // get the part upLink name to be used
    string partName =
      staffPartUpLink->
        getPartNameDisplayText ();

    if (partName.size () == 0) {
      partName =
        staffPartUpLink->
          getPartName ();
    }

    // generate the instrument name
    //* JMI BLARKBLARK
    if (partName.size ()) {
    /* JMI
      fLilypondCodeOstream <<
        "instrumentName = ";

      // does the name contain hexadecimal end of lines?
      std::size_t found =
    // JMI    partName.find ("&#xd");
        partName.find ("\n");

      if (found == string::npos) {
        // no, escape quotes if any and generate the result
        fLilypondCodeOstream <<
          "\"" <<
          escapeDoubleQuotes (partName) <<
          "\"" <<
          endl;
      }

      else {
        // yes, split the name into a chunks list
        // and generate a \markup{} // JMI ???
        fLilypondCodeOstream <<
          endl <<
          generateMultilineMarkup (
            partName,
            markupColumnKindLeftACentered) << // JMI ???
          endl;
      }
      */
    }
    /* ??? JMI
    if (partAbbreviation.size ()) {
      fLilypondCodeOstream <<
        "shortInstrumentName = " <<
        nameAsLilypondString (partAbbreviation) <<
        endl;
    }
    */

    // get the part upLink abbreviation display text to be used
    string partAbbreviation =
      staffPartUpLink->
        getPartAbbreviationDisplayText ();

    if (partAbbreviation.size () == 0) {
      partAbbreviation =
        staffPartUpLink->
          getPartAbbreviation ();
    }

/* JMI ???
    if (partAbbreviation.size ()) {
      fLilypondCodeOstream <<
        "shortInstrumentName = ";

      // does the name contain hexadecimal end of lines?
      std::size_t found =
        partAbbreviation.find ("&#xd");

      if (found == string::npos) {
        // no, merely generate the name
        fLilypondCodeOstream <<
          nameAsLilypondString (partAbbreviation) <<
          endl;
      }

      else {
        // yes, split the name into a chunks list
        // and generate a \markup{} // JMI ???
        fLilypondCodeOstream <<
          endl <<
          generateMultilineMarkup (partAbbreviation) <<
          endl;
      }
    }
    */
  }

  gIndenter--;

  // generate the string tunings if any
  S_msrStaffDetails
    currentStaffDetails =
      staff->getCurrentStaffStaffDetails ();

  if (currentStaffDetails) {
    const list<S_msrStaffTuning>&
      staffTuningsList =
        currentStaffDetails->getStaffTuningsList ();

    if (staffTuningsList.size ()) {
      fLilypondCodeOstream <<
  // JMI      "restrainOpenStrings = ##t" <<
  // JMI      endl <<
        "stringTunings = \\stringTuning <";

      list<S_msrStaffTuning>::const_iterator
        iBegin = staffTuningsList.begin (),
        iEnd   = staffTuningsList.end (),
        i      = iBegin;

      gIndenter++;

      for ( ; ; ) {
        S_msrStaffTuning
          staffTuning = (*i);

        fLilypondCodeOstream <<
          msrQuarterTonesPitchKindAsString (
            gLpsrOah->
              fLpsrQuarterTonesPitchesLanguageKind,
            staffTuning->
              getStaffTuningQuarterTonesPitchKind ()) <<
          absoluteOctaveAsLilypondString (
            staffTuning->getStaffTuningOctave ());

        if (++i == iEnd) break;

        fLilypondCodeOstream << ' ';
      } // for

      fLilypondCodeOstream <<
        ">" <<
        endl;

      gIndenter--;

      // should letters be used for frets?
      switch (currentStaffDetails->getShowFretsKind ()) {
        case msrStaffDetails::kShowFretsNumbers:
          break;
        case msrStaffDetails::kShowFretsLetters:
          fLilypondCodeOstream <<
            "tablatureFormat = #fret-letter-tablature-format" <<
            endl;
          break;
      } // switch
    }
  }

  // generate ledger lines coloring code if needed
  if (fVisitedLpsrScore->getColoredLedgerLinesIsNeeded ()) {
    fLilypondCodeOstream <<
      gIndenter.indentMultiLineString (
R"(  \override LedgerLineSpanner.stencil = #MyLedgerLineSpannerPrint
  \override LedgerLineSpanner.after-line-breaking = #grob::display-objects)") <<
      endl;
  }

  // generate merge rests if needed
  if (fVisitedLpsrScore->getMergeRestsIsNeeded ()) {
    fLilypondCodeOstream <<
      gTab << "\\override RestCollision.positioning-done = #merge-rests-on-positioning" <<
      endl;
  }

  // generate the 'with' block ending
  fLilypondCodeOstream <<
    "}" <<
    endl;

  // generate the comment if relevant
  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        "<<" <<
        " % staff \"" << staff->getStaffName () << "\"";
  }
  else {
    fLilypondCodeOstream <<
      "<<";
  }

  fLilypondCodeOstream << endl;

  if (gLpsr2LilypondOah->fJianpu) {
    fLilypondCodeOstream <<
      " \\jianpuMusic" <<
      endl;
  }

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrStaffBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrStaffBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) << ">>" <<
      "% staff " <<
      elt->getStaff ()->getStaffName ();
  }
  else {
    fLilypondCodeOstream <<
      ">>";
  }

  fLilypondCodeOstream << endl;

  if (fStaffBlocksCounter != fNumberOfStaffBlocksElements) {
    fLilypondCodeOstream << endl;
  }
}

/*
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewStaffgroupBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrNewStaffgroupBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

   fLilypondCodeOstream <<
     "\\new StaffGroup" << ' ' << "{" <<
      endl;

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewStaffgroupBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrNewStaffgroupBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  fLilypondCodeOstream <<
    " }" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewStaffBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrNewStaffBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewStaffBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrNewStaffBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;
}
*/

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrUseVoiceCommand& elt) // JMI ???
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrUseVoiceCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  S_msrVoice
    voice = elt->getVoice ();

  S_msrStaff
    staff = voice-> getVoiceStaffUpLink ();

  msrStaff::msrStaffKind
    staffKind = staff->getStaffKind ();

  string staffContextName;
  string voiceContextName;

  switch (staffKind) {
    case msrStaff::kStaffRegular:
      staffContextName = "Staff";
      voiceContextName = "Voice";
      break;

    case msrStaff::kStaffTablature:
      staffContextName = "TabStaff";
      voiceContextName = "TabVoice";
      break;

    case msrStaff::kStaffHarmony:
      staffContextName = "ChordNames2"; // JMI
      voiceContextName = "???"; // JMI
      break;

    case msrStaff::kStaffFiguredBass:
      staffContextName = "FiguredBass";
      voiceContextName = "???"; // JMI
      break;

    case msrStaff::kStaffDrum:
      staffContextName = "DrumStaff";
      voiceContextName = "DrumVoice";
        // the "DrumVoice" alias exists, let's use it
      break;

    case msrStaff::kStaffRythmic:
      staffContextName = "RhythmicStaff";
      voiceContextName = "Voice";
        // no "RhythmicVoice" alias exists
      break;
  } // switch

 // if (voice->getStaffRelativeVoiceNumber () > 0) { JMI
    fLilypondCodeOstream <<
      "\\context " << voiceContextName << " = " "\"" <<
      voice->getVoiceName () << "\"" << " <<" <<
       endl;

    gIndenter++;

    if (gLpsr2LilypondOah->fNoAutoBeaming) {
      fLilypondCodeOstream <<
        "\\set " << staffContextName << ".autoBeaming = ##f" <<
        endl;
    }

    // should a voice indication be generated?
    switch (staffKind) {
      case msrStaff::kStaffRegular:
        {
          int staffRegularVoicesCounter =
            staff->getStaffRegularVoicesCounter ();

          if (staffRegularVoicesCounter > 1) {
            switch (voice->getRegularVoiceStaffSequentialNumber ()) {
              case 1:
                fLilypondCodeOstream << "\\voiceOne ";
                break;
              case 2:
                fLilypondCodeOstream << "\\voiceTwo ";
                break;
              case 3:
                fLilypondCodeOstream << "\\voiceThree ";
                break;
              case 4:
                fLilypondCodeOstream << "\\voiceFour ";
                break;
              default:
                {}
            } // switch

            fLilypondCodeOstream <<
              "% out of " <<
              staffRegularVoicesCounter <<
              " regular voices" <<
              endl;
          }
        }
      break;

      default:
        ;
    } // switch

    // fetch the part and part name
    S_msrPart
      part = staff-> getStaffPartUpLink ();
    string partName =
      part->getPartName ();

    // should a transposition be generated?
#ifdef TRACE_OAH
    if (gTraceOah->fTraceTranspositions) {
      fLogOutputStream <<
        endl <<
        "Considering the generation a a voice transposition for part \"" <<
        partName <<
        "\"" <<
        endl;
/* JMI
      fLilypondCodeOstream << // TEMP JMI
         "\\transposition " << "bes" <<
         endl <<
         "\\transpose " << "bes" << " " << "c'" <<
        endl;
      */
    }
#endif

    if (gLpsr2LilypondOah->fPartsTranspositionMap.size ()) {
      // should we transpose fCurrentPart?
      map<string, S_msrSemiTonesPitchAndOctave>::const_iterator
        it =
          gLpsr2LilypondOah->fPartsTranspositionMap.find (
            partName);

      if (it != gLpsr2LilypondOah->fPartsTranspositionMap.end ()) {
        // partName is present in the map,
        // fetch the semitones pitch and octave
        S_msrSemiTonesPitchAndOctave
          semiTonesPitchAndOctave =
            (*it).second;

        // generate the transposition
#ifdef TRACE_OAH
        if (gTraceOah->fTraceTranspositions) {
          fLogOutputStream <<
            endl <<
            "Generating a voice transposition for " <<
            semiTonesPitchAndOctave->asString () <<
            " in part \"" <<
            partName <<
            "\"" <<
            endl;
        }
#endif

        // fetch the LilyPond pitch and octave
        string
          semiTonesPitchAndOctaveAsLilypondString =
            msrSemiTonesPitchAndOctaveAsLilypondString (
              gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
              semiTonesPitchAndOctave);

        fLilypondCodeOstream <<
           "\\transposition " <<
           semiTonesPitchAndOctaveAsLilypondString <<
           endl <<
           "\\transpose " <<
           semiTonesPitchAndOctaveAsLilypondString << " " << "c'" <<
          endl;
      }
    }

    // generate voice name
    fLilypondCodeOstream <<
      "\\" << voice->getVoiceName () << endl;

    gIndenter--;

    fLilypondCodeOstream <<
      ">>" <<
      endl;
 // } JMI
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrUseVoiceCommand& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrUseVoiceCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewLyricsBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrNewLyricsBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (! gLpsr2LilypondOah->fNoLilypondLyrics) {
    S_msrStanza stanza = elt->getStanza ();

    fLilypondCodeOstream <<
      "\\new Lyrics" <<
      endl;

    gIndenter++;

    fLilypondCodeOstream <<
      "\\with {" <<
      endl;

    if (gLpsr2LilypondOah->fAddStanzasNumbers) {
      fLilypondCodeOstream <<
        gTab << "stanza = \"" <<
        stanza->getStanzaNumber () <<
        ".\"" <<
        endl;
    }

    fLilypondCodeOstream <<
      "}" <<
      endl;

    switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
      case kLyricsDurationsImplicit:
        fLilypondCodeOstream <<
          "\\lyricsto \"" << elt->getVoice ()->getVoiceName () << "\" {" <<
          "\\" << stanza->getStanzaName () <<
          "}" <<
          endl;
        break;
      case kLyricsDurationsExplicit:
        // no \lyricsto in that case
        fLilypondCodeOstream <<
          "\\" << stanza->getStanzaName () <<
          endl;
        break;
    } // switch

    gIndenter--;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewLyricsBlock& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrNewLyricsBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (! gLpsr2LilypondOah->fNoLilypondLyrics) {
    // JMI
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrVariableUseCommand& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrVariableUseCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrVariableUseCommand& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrVariableUseCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrChordNamesContext& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrChordNamesContext" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  string
    contextTypeKindAsString =
      elt->getContextTypeKindAsString (),
    contextName =
      elt->getContextName ();

  fLilypondCodeOstream <<
    "\\context " << contextTypeKindAsString <<
    " = \"" << contextName << "\"" <<
    endl;

/* JMI
  if (false) { //option JMI
    fLilypondCodeOstream <<
      "\\with {" <<
      endl;

    gIndenter++;

    fLilypondCodeOstream <<
      "\\override Barline.bar-extent = #'(-2 . 2)" <<
      endl <<
      "\\consists \"Bar_engraver\"" <<
      endl;

    gIndenter--;

    fLilypondCodeOstream <<
      "}" <<
      endl;
  }
      */

  fLilypondCodeOstream <<
    "\\" << contextName <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrChordNamesContext& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrChordNamesContext" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrFiguredBassContext& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrFiguredBassContext" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  string
    contextTypeKindAsString =
      elt->getContextTypeKindAsString (),
    contextName =
      elt->getContextName ();

  fLilypondCodeOstream <<
    "\\context " << contextTypeKindAsString <<
    " = \"" << contextName << "\"" <<
    endl;

/* JMI
  if (false) { //option JMI
    fLilypondCodeOstream <<
      "\\with {" <<
      endl;

    gIndenter++;

    fLilypondCodeOstream <<
      "\\override Barline.bar-extent = #'(-2 . 2)" <<
      endl <<
      "\\consists \"Bar_engraver\"" <<
      endl;

    gIndenter--;

    fLilypondCodeOstream <<
      "}" <<
      endl;
  }
  */

  fLilypondCodeOstream <<
    "\\" << contextName <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrFiguredBassContext& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrFiguredBassContext" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrBarCommand& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrBarCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrBarCommand& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrBarCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
/* JMI
void lpsr2LilypondTranslator::visitStart (S_lpsrMelismaCommand& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrMelismaCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (elt->getMelismaKind ()) {
    case lpsrMelismaCommand::kMelismaStart:
// JMI      fLilypondCodeOstream << "\\melisma ";
      break;
    case lpsrMelismaCommand::kMelismaEnd:
// JMI      fLilypondCodeOstream << "\\melismaEnd ";
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrMelismaCommand& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrMelismaCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}
*/

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrScore& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrScore& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrScaling& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrScaling& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrScaling" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPageLayout& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* JMI
    // page size
    S_msrLength           fPageHeight;
    S_msrLength           fPageWidth;

    // margins
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;
    S_msrMargin           fTopMargin;
    S_msrMargin           fBottomMargin;
  */

  S_msrLength
    pageHeight =
      elt->getPageHeight ();

  if (pageHeight) {
    fLilypondCodeOstream <<
      pageHeight->getLengthValue () <<
      lengthUnitAsLilypondString (pageHeight->getLengthUnitKind ()) <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrPageLayout& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrPageLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrCredit& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrCredit& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitStart (S_msrCreditWords& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrCreditWords& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPartGroup& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrPartGroup" <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      elt->getPartGroupCombinedName () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrPartGroup& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrPartGroup" <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPart& elt)
{
  string
    partCombinedName =
      elt->getPartCombinedName ();

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrPart" <<
      partCombinedName <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceParts) {
    fLogOutputStream <<
      endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->" <<
      endl;
  }
#endif

  // remember current part
  fCurrentPart = elt;

  fRemainingRestMeasuresNumber = 0; // JMI
  fOnGoingRestMeasures = false; // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrPart& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrPart" <<
      elt->getPartCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // forget about current part
  fCurrentPart = nullptr;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStaff& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingStaff = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrStaff& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingStaff = false;
}

void lpsr2LilypondTranslator::visitStart (S_msrStaffTuning& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> Start visiting msrStaffTuning" <<
      endl;
  }
#endif

/* JMI
  list<S_msrStaffTuning>
    staffTuningsList =
      staff->getStaffTuningsList ();

  if (staffTuningsList.size ()) {
    // \set TabStaff.stringTunings = \stringTuning <c' g' d'' a''>

    fLilypondCodeOstream <<
      "\\set TabStaff.stringTunings = \\stringTuning <";

    list<S_msrStaffTuning>::const_iterator
      iBegin = staffTuningsList.begin (),
      iEnd   = staffTuningsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      fLilypondCodeOstream <<
        msrQuarterTonesPitchAsString (
          gLpsrOah->fLpsrQuarterTonesPitchesLanguage,
 // JMI            elt->getInputLineNumber (),
          ((*i)->getStaffTuningQuarterTonesPitch ())) <<
 // JMI       char (tolower ((*i)->getStaffTuningStep ())) <<
        absoluteOctaveAsLilypondString (
          (*i)->getStaffTuningOctave ());
      if (++i == iEnd) break;
      fLilypondCodeOstream << ' ';
    } // for

    fLilypondCodeOstream <<
      ">" <<
      endl;
  }
 */
}

void lpsr2LilypondTranslator::visitStart (S_msrStaffDetails& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> Start visiting msrStaffDetails" <<
      endl;
  }
#endif

  // fetch staff lines number
  int
    staffLinesNumber =
      elt->getStaffLinesNumber ();

  fLilypondCodeOstream <<
    endl <<
    "\\stopStaff " <<
    endl <<
    "\\override Staff.StaffSymbol.line-count = " <<
    staffLinesNumber <<
    endl <<
    "\\startStaff" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrVoice& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentVoice = elt;

  fLilypondCodeOstream <<
    fCurrentVoice->getVoiceName () <<
    " = ";

  // generate the beginning of the voice definition
  switch (fCurrentVoice->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      switch (gLpsr2LilypondOah->fOctaveEntryKind) {
        case kOctaveEntryRelative:
          fLilypondCodeOstream <<
            "\\relative";
          if (
            gLpsr2LilypondOah->fRelativeOctaveEntrySemiTonesPitchAndOctave
//      JMI         !=
//            gLpsr2LilypondOah->fSemiTonesPitchAndOctaveDefaultValue
          ) {
            // option '-rel, -relative' has been used
            fLilypondCodeOstream <<
              " " <<
              msrSemiTonesPitchAndOctaveAsLilypondString (
                gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
                gLpsr2LilypondOah->fRelativeOctaveEntrySemiTonesPitchAndOctave);
          }
          break;

        case kOctaveEntryAbsolute:
          fLilypondCodeOstream <<
            "\\absolute";
          break;

        case kOctaveEntryFixed:
          fLilypondCodeOstream <<
            "\\fixed " <<
            msrSemiTonesPitchAndOctaveAsLilypondString (
              gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
              gLpsr2LilypondOah->fFixedOctaveEntrySemiTonesPitchAndOctave);
          break;
      } // switch

      fLilypondCodeOstream <<
        " {" <<
        endl;
      break;

    case msrVoice::kVoiceHarmony:
      fLilypondCodeOstream <<
        "\\chordmode {" <<
        endl;
      break;

    case msrVoice::kVoiceFiguredBass:
      fLilypondCodeOstream <<
        "\\figuremode {" <<
        endl;
      break;
  } // switch

  gIndenter++;

  // use a 'global' macro?
  if (gLpsr2LilypondOah->fGlobal) {
    fLilypondCodeOstream <<
      "\\global" <<
      endl <<
      endl;
  }

  // \displayMusic?
  if (gLpsr2LilypondOah->fDisplayMusic) {
    fLilypondCodeOstream <<
      "\\displayMusic {" <<
      endl;

    gIndenter++;
  }

  // language
  fLilypondCodeOstream <<
    "\\language \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      gLpsrOah->
        fLpsrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl;

  // chords language
  if (gLpsrOah->fLpsrChordsLanguageKind != k_IgnatzekChords) {
    fLilypondCodeOstream <<
      "\\" <<
      lpsrChordsLanguageKindAsString (
        gLpsrOah->
          fLpsrChordsLanguageKind) <<
      "Chords" <<
      endl;
  }

  // show all bar numbers?
  if (gLpsr2LilypondOah->fShowAllBarNumbers) {
    fLilypondCodeOstream <<
      "\\set Score.barNumberVisibility = #all-bar-numbers-visible" <<
      endl <<
      "\\override Score.BarNumber.break-visibility = ##(#f #t #t)" <<
      endl <<
      endl;
  }

  // center bar number?
  if (gLpsr2LilypondOah->fBoxAroundBarNumberSet.size ()) {
    // yes, center the boxed bar number
#ifdef TRACE_OAH
    if (gTraceOah->fTraceMeasuresNumbers) {
      fLogOutputStream <<
        endl <<
        "Centering boxed LilyPond measure numbers" <<
        ", line " << elt->getInputLineNumber () << " ===-->" <<
        endl;
    }
#endif

    fLilypondCodeOstream <<
      "\\override Score.BarNumber.self-alignment-X = #CENTER" <<
      endl;
  }

  // compress full measure rests?
  if (gLpsr2LilypondOah->fCompressFullMeasureRests) {
    fLilypondCodeOstream <<
      "\\compressFullBarRests" <<
      endl <<
      "\\set restNumberThreshold = 0" <<
      endl <<
      endl;
  }

// JMI   \set Score.alternativeNumberingStyle = #'numbers-with-letters

/* JMI
  if (
    fCurrentVoice->getVoiceContainsRestMeasures ()
      ||
    gLpsr2LilypondOah->fCompressFullMeasureRests
  ) {
    fLilypondCodeOstream <<
      "\\compressFullBarRests" <<
      endl;

    gIndenter++;
  }
*/

  if (gLpsr2LilypondOah->fAccidentalStyleKind != kDefault) {
    fLilypondCodeOstream <<
      "\\accidentalStyle Score." <<
      lpsrAccidentalStyleKindAsString (
        gLpsr2LilypondOah->fAccidentalStyleKind) <<
      endl <<
      endl;
  }

  // reset fCurrentVoiceClef
  fCurrentVoiceClef = nullptr;

  // reset fCurrentOctaveEntryReference if relevant
  switch (gLpsr2LilypondOah->fOctaveEntryKind) {
    case kOctaveEntryRelative:
      // forget about the current reference:
      // it should be set from the LilyPond preferences here
      setCurrentOctaveEntryReferenceFromTheLilypondOah ();
      break;
    case kOctaveEntryAbsolute:
      break;
    case kOctaveEntryFixed:
      break;
  } // switch

  fVoiceIsCurrentlySenzaMisura = false;

  fOnGoingVoice = true;

  switch (fCurrentVoice->getVoiceKind ()) { // JMI
    case msrVoice::kVoiceRegular:
      break;

    case msrVoice::kVoiceHarmony:
      fOnGoingHarmonyVoice = true;
      break;

    case msrVoice::kVoiceFiguredBass:
      fOnGoingFiguredBassVoice = true;
      break;
  } // switch

  // reset current voice measures counter
  fCurrentVoiceMeasuresCounter = 0; // none have been found

  // force durations to be displayed explicitly
  // at the beginning of the voice
  fLastMetWholeNotes = rational (0, 1);
}

void lpsr2LilypondTranslator::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  /* JMI
  if (
    fCurrentVoice->getVoiceContainsRestMeasures ()
      ||
    gLpsr2LilypondOah->fCompressFullMeasureRests
  ) {
    fLilypondCodeOstream <<
  // JMI    "}" <<
      endl;

    gIndenter--;
  */

  if (gLpsr2LilypondOah->fDisplayMusic) {
    fLilypondCodeOstream <<
      "}";

    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        " % displayMusic";
    }

    fLilypondCodeOstream << endl;

    gIndenter--;
  }

  // generate the end of the voice definition
  switch (elt->getVoiceKind ()) { // JMI putzer
    case msrVoice::kVoiceRegular:
      fLilypondCodeOstream <<
        "}";
      break;

    case msrVoice::kVoiceHarmony:
      fLilypondCodeOstream <<
        "}";
      break;

    case msrVoice::kVoiceFiguredBass:
      fLilypondCodeOstream <<
        "}";
      break;
  } // switch

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      " % voice " << elt->getVoiceName ();
  }

  fLilypondCodeOstream <<
    endl <<
    endl;

  // set relevant 'ongoing' variables
  switch (elt->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
      break;

    case msrVoice::kVoiceHarmony:
      fOnGoingHarmonyVoice = false;
      break;

    case msrVoice::kVoiceFiguredBass:
      fOnGoingFiguredBassVoice = false;
      break;
  } // switch

  fCurrentVoice = nullptr;
  fOnGoingVoice = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    endl <<
    "\\change Staff=\"" <<
    elt->getStaffToChangeTo ()->getStaffName () <<
    "\"";

  if (
    gLpsr2LilypondOah->fInputLineNumbers
      ||
    gLpsr2LilypondOah->fPositionsInMeasures
  ) {
    generateInputLineNumberAndOrPositionInMeasureAsAComment (
      elt);
  }

  fLilypondCodeOstream << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrHarmony& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", fOnGoingNotesStack.size () = " <<  fOnGoingNotesStack.size () <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNotesStack.size () > 0) {
  /* JMI
#ifdef TRACE_OAH
    if (gTraceOah->fTraceHarmonies) {
      fLilypondCodeOstream <<
        "%{ fOnGoingNotesStack.size () S_msrHarmony JMI " <<
        elt->asString () <<
        " %}" <<
        endl;
    }
#endif
*/
  }

  else if (fOnGoingChord) { // JMI
  }

  else if (fOnGoingHarmonyVoice) {
    fLilypondCodeOstream <<
      harmonyAsLilypondString (elt) <<
      ' ';

    if (
      gLpsr2LilypondOah->fInputLineNumbers
        ||
      gLpsr2LilypondOah->fPositionsInMeasures
    ) {
      generateInputLineNumberAndOrPositionInMeasureAsAComment (
        elt);
    }
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrFrame& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingNotesStack.size () > 0) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceFrames) {
      fLilypondCodeOstream <<
        "%{ " << elt->asString () << " %}" <<
        endl;
    }
#endif

    fLilypondCodeOstream <<
      frameAsLilypondString (elt) <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrFiguredBass& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", fOnGoingNotesStack.size () = " << fOnGoingNotesStack.size () <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingFiguredBassVoice = " << booleanAsString (fOnGoingFiguredBassVoice) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fCurrentFiguredBass = elt;

  if (fOnGoingNotesStack.size () > 0) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceFiguredBasses) {
      fLilypondCodeOstream <<
        "%{ fOnGoingNotesStack.size () S_msrFiguredBass JMI " << fCurrentFiguredBass->asString () << " %}" <<
        endl;
    }
#endif
  }

  else if (fOnGoingChord) { // JMI
  }

  else if (fOnGoingFiguredBassVoice) {
    fLilypondCodeOstream <<
      figuredBassAsLilypondString (elt) <<
      ' ';

    if (
      gLpsr2LilypondOah->fInputLineNumbers
        ||
      gLpsr2LilypondOah->fPositionsInMeasures
    ) {
      generateInputLineNumberAndOrPositionInMeasureAsAComment (
        fCurrentFiguredBass);
    }
  }

  fCurrentFiguredBassFiguresCounter = 0;
}

void lpsr2LilypondTranslator::visitStart (S_msrFigure& elt)
{
/* JMI
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (fOnGoingFiguredBassVoice) {
    fCurrentFiguredBassFiguresCounter++;

    // is the figured bass parenthesized?
    msrFiguredBass::msrFiguredBassParenthesesKind
      figuredBassParenthesesKind =
        fCurrentFiguredBass->
          getFiguredBassParenthesesKind ();

    // generate the figure number
    switch (figuredBassParenthesesKind) {
      case msrFiguredBass::kFiguredBassParenthesesYes:
        fLilypondCodeOstream << "[";
        break;
      case msrFiguredBass::kFiguredBassParenthesesNo:
        break;
    } // switch

    fLilypondCodeOstream <<
      elt->getFigureNumber ();

    switch (figuredBassParenthesesKind) {
      case msrFiguredBass::kFiguredBassParenthesesYes:
        fLilypondCodeOstream << "]";
        break;
      case msrFiguredBass::kFiguredBassParenthesesNo:
        break;
    } // switch

    // handle the figure prefix
    switch (elt->getFigurePrefixKind ()) {
      case msrFigure::k_NoFigurePrefix:
        break;
      case msrFigure::kDoubleFlatPrefix:
        fLilypondCodeOstream << "--";
        break;
      case msrFigure::kFlatPrefix:
        fLilypondCodeOstream << "-";
        break;
      case msrFigure::kFlatFlatPrefix:
        fLilypondCodeOstream << "flat flat";
        break;
      case msrFigure::kNaturalPrefix:
        fLilypondCodeOstream << "!";
        break;
      case msrFigure::kSharpSharpPrefix:
        fLilypondCodeOstream << "sharp sharp";
        break;
      case msrFigure::kSharpPrefix:
        fLilypondCodeOstream << "+";
        break;
      case msrFigure::kDoubleSharpPrefix:
        fLilypondCodeOstream << "++";
        break;
    } // switch

    // handle the figure suffix
    switch (elt->getFigureSuffixKind ()) {
      case msrFigure::k_NoFigureSuffix:
        break;
      case msrFigure::kDoubleFlatSuffix:
        fLilypondCodeOstream << "double flat";
        break;
      case msrFigure::kFlatSuffix:
        fLilypondCodeOstream << "flat";
        break;
      case msrFigure::kFlatFlatSuffix:
        fLilypondCodeOstream << "flat flat";
        break;
      case msrFigure::kNaturalSuffix:
        fLilypondCodeOstream << "natural";
        break;
      case msrFigure::kSharpSharpSuffix:
        fLilypondCodeOstream << "sharp sharp";
        break;
      case msrFigure::kSharpSuffix:
        fLilypondCodeOstream << "sharp";
        break;
      case msrFigure::kDoubleSharpSuffix:
        fLilypondCodeOstream << "souble sharp";
        break;
      case msrFigure::kSlashSuffix:
        fLilypondCodeOstream << "/";
        break;
    } // switch

    // generate a space if not last figure in figured bass
    if (
      fCurrentFiguredBassFiguresCounter
        <
      fCurrentFiguredBass->getFiguredBassFiguresList ().size ()
    ) {
      fLilypondCodeOstream << ' ';
    }
  }
  */
}

void lpsr2LilypondTranslator::visitEnd (S_msrFiguredBass& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/* JMI
  if (fOnGoingFiguredBassVoice) {
    fLilypondCodeOstream <<
      ">";

    // print figured bass duration
    msrTupletFactor
      figuredBassTupletFactor =
        elt->getFiguredBassTupletFactor ();

    if (figuredBassTupletFactor.isEqualToOne ()) {
      // use figured bass sounding whole notes
      fLilypondCodeOstream <<
        durationAsLilypondString (
          inputLineNumber,
          elt->
            getFiguredBassSoundingWholeNotes ());
    }
    else {
      // use figured bass display whole notes and tuplet factor
      fLilypondCodeOstream <<
        durationAsLilypondString (
          inputLineNumber,
          elt->
            getFiguredBassDisplayWholeNotes ()) <<
        "*" <<
        figuredBassTupletFactor.asRational ();
    }

    fLilypondCodeOstream <<
      ' ';
  }
  */
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSegment& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;
  }
#endif

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "% start of segment " <<
      elt->getSegmentAbsoluteNumber () <<
      ", line " <<
      elt->getInputLineNumber () <<
      endl;

    gIndenter++;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrSegment& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;
  }
#endif

  if (gLpsr2LilypondOah->fLilyPondComments) {
    gIndenter--;

    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "% end of segment" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACE_OAH
  msrMeasure::msrMeasureEndRegularKind
    measureEndRegularKind =
      elt-> getMeasureEndRegularKind ();
#endif

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrMeasure '" <<
      measureNumber <<
      "', " <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", " <<
      msrMeasure::measureEndRegularKindAsString (
        measureEndRegularKind) <<
      ", measurePuristNumber = '" <<
      measurePuristNumber <<
      "', onGoingRestMeasures = '" <<
      booleanAsString (
        fOnGoingRestMeasures) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // should we generated a box around this bar number?
  {
    set<int>::const_iterator
      it =
        gLpsr2LilypondOah->
          fBoxAroundBarNumberSet.find (measurePuristNumber);

    if (it != gLpsr2LilypondOah->fBoxAroundBarNumberSet.end ()) {
      // yes, generate a box around the bar number
  #ifdef TRACE_OAH
      if (gTraceOah->fTraceMeasuresNumbers) {
        fLogOutputStream <<
          endl <<
          "Generating a box around LilyPond measure purist number '" <<
          measurePuristNumber <<
          "', line " << inputLineNumber << " ===-->" <<
          endl;
      }
  #endif

      fLilypondCodeOstream <<
        "\\boxAroundNextBarNumber" <<
        endl;
    }
  }

  // should we reset the measure purist number?
  {
    map<string, int>::const_iterator
      it =
        gLpsr2LilypondOah->
          fResetMeasureElementMeasureNumberMap.find (measureNumber);

    if (it != gLpsr2LilypondOah->fResetMeasureElementMeasureNumberMap.end ()) {
      // yes, reset measure number
      int lilypondMeasureNumber = (*it).second;

      if (to_string (lilypondMeasureNumber) != measureNumber) {
  #ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresNumbers) {
          fLogOutputStream <<
            endl <<
            "Resetting LilyPond measure number from '" <<
            measureNumber <<
            "' to " <<
            lilypondMeasureNumber <<
            "', line " << inputLineNumber << " ===-->" <<
            endl;
        }
  #endif

        fLilypondCodeOstream <<
          "\\set Score.currentBarNumber = #" <<
          lilypondMeasureNumber <<
          endl;
      }
      else {
  #ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresNumbers) {
          fLogOutputStream <<
            endl <<
            "Cannot reset measure LilyPond number from '" <<
            measureNumber <<
            "' to " <<
            lilypondMeasureNumber <<
            ": they're one and the same" <<
            "', line " << inputLineNumber << " ===-->" <<
            endl;
        }
  #endif
      }
    }
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    fLogOutputStream <<
      endl <<
      "% <!--=== measure '" << measureNumber <<
      "' start, " <<
      msrMeasure::measureEndRegularKindAsString (
        measureEndRegularKind) <<
      ", measurePuristNumber = '" <<
      measurePuristNumber <<
      "', onGoingRestMeasures = '" <<
      booleanAsString (
        fOnGoingRestMeasures) <<
      "', line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // generate comment if relevant
  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "% start of " <<
      msrMeasure::measureKindAsString (elt->getMeasureKind ()) <<
      " measure " <<
      measureNumber <<
      ", line " << inputLineNumber <<
      endl;

    gIndenter++;
  }

  // take this measure into account for counting
  switch (elt->getMeasureKind ()) {
    case msrMeasure::kMeasureKindUnknown:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kMeasureKindRegular:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kMeasureKindAnacrusis:
      // keep fCurrentVoiceMeasuresCounter at 0
      break;
    case msrMeasure::kMeasureKindIncompleteStandalone:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kMeasureKindOvercomplete:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kMeasureKindCadenza:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kMeasureKindMusicallyEmpty:
      fCurrentVoiceMeasuresCounter++;
      break;
  } // switch

  // force durations to be displayed explicitly
  // for the notes at the beginning of the measure
  fLastMetWholeNotes = rational (0, 1);

  // is this the end of a cadenza?
  if (
    fOnGoingVoiceCadenza
      &&
    measureKind != msrMeasure::kMeasureKindOvercomplete
  ) {
  /* JMI
    fLilypondCodeOstream <<
      endl <<
      "\\cadenzaOff" <<
 // JMI     " \\undo \\omit Staff.TimeSignature" <<
      endl <<
      "\\bar \"|\" "; // JMI ???
      */

    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream <<
        " % kMeasureKindOvercomplete End";
    }

    fLilypondCodeOstream<<
      endl;

    fOnGoingVoiceCadenza = false;
  }

  switch (measureKind) {
    case msrMeasure::kMeasureKindUnknown:
      {
        stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' is of unknown kind in lpsr2LilypondTranslator";

if (false) // JMI
        msrInternalError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
else
        msrInternalWarning (
          gOahOah->fInputSourceName,
          inputLineNumber,
          s.str ());
      }
      break;

    case msrMeasure::kMeasureKindRegular:
      break;

    case msrMeasure::kMeasureKindAnacrusis:
      if (elt->getMeasureFirstInVoice ()) {
        // only generate '\partial' at the beginning of a voice
        string
          upbeatDuration =
            wholeNotesAsLilypondString (
              inputLineNumber,
              elt->getCurrentMeasureWholeNotesDuration ());

        fLilypondCodeOstream <<
          "\\partial " << upbeatDuration <<
          endl;
      }
      break;

    case msrMeasure::kMeasureKindIncompleteStandalone:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatCommonPart:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHookedEnding:
    case msrMeasure::kMeasureKindIncompleteLastInRepeatHooklessEnding:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterCommonPart:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
    case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
      {
        rational
          currentMeasureWholeNotesDuration =
            elt->getCurrentMeasureWholeNotesDuration ();

        rational
          fullMeasureWholeNotesDuration =
            elt->getFullMeasureWholeNotesDuration ();

        // we should set the score current measure whole notes in this case
        rational
          ratioToFullMeasureWholeNotesDuration =
            currentMeasureWholeNotesDuration / fullMeasureWholeNotesDuration;
        ratioToFullMeasureWholeNotesDuration.rationalise ();

#ifdef TRACE_OAH
        if (gTraceOah->fTraceMeasuresDetails) {
          const int fieldWidth = 27;

          fLilypondCodeOstream << left <<
            "% Setting the current measure whole notes for measure " <<
            setw (fieldWidth) <<
            measureNumber <<
            ", line = " << inputLineNumber <<
            endl <<
            setw (fieldWidth) <<
            "% currentMeasureWholeNotesDuration" << " = " <<
            currentMeasureWholeNotesDuration <<
            endl <<
            setw (fieldWidth) <<
            "% fullMeasureWholeNotesDuration" << " = " <<
            fullMeasureWholeNotesDuration <<
            endl <<
            setw (fieldWidth) <<
            "% ratioToFullMeasureWholeNotesDuration" << " = " <<
            ratioToFullMeasureWholeNotesDuration <<
            endl <<
            endl;
        }
#endif

        if (ratioToFullMeasureWholeNotesDuration == rational (1, 1)) {
          stringstream s;

          s <<
            "underfull measure '" << measureNumber <<
            "' has actually the full measure whole notes";

     // JMI       msrInternalError (
          msrInternalWarning (
            gOahOah->fInputSourceName,
            inputLineNumber,
    // JMI        __FILE__, __LINE__,
            s.str ());
        }

        else {
          /* JMI
          fLilypondCodeOstream <<
            "\\set Score.measureLength = #(ly:make-moment " <<
            currentMeasureWholeNotesDuration.toString () <<
            ")" <<
            endl;
    */

          // should we generate a line break?
          if (gLpsr2LilypondOah->fBreakLinesAtIncompleteRightMeasures) {
            fLilypondCodeOstream <<
              "\\break" <<
              endl;
          }
        }
      }
      break;

    case msrMeasure::kMeasureKindOvercomplete:
      if (! fOnGoingVoiceCadenza) {
        fLilypondCodeOstream <<
          endl <<
          "\\cadenzaOn" <<
          " \\omit Staff.TimeSignature";

        if (gLpsr2LilypondOah->fLilyPondComments) {
          fLilypondCodeOstream << " % kMeasureKindOvercomplete Start";
        }

        fLilypondCodeOstream << endl;

        fOnGoingVoiceCadenza = true;
      }
      break;

    case msrMeasure::kMeasureKindCadenza:
      if (! fOnGoingVoiceCadenza) {
        fLilypondCodeOstream <<
          endl <<
          "\\cadenzaOn";

        if (gLpsr2LilypondOah->fLilyPondComments) {
          fLilypondCodeOstream << " % kMeasureKindCadenza Start";
        }

        fLilypondCodeOstream << endl;

        fLilypondCodeOstream <<
          "\\once\\omit Staff.TimeSignature" <<
          endl;

        fOnGoingVoiceCadenza = true;
      }
      break;

    case msrMeasure::kMeasureKindMusicallyEmpty:
      {
        // fetch measure's voice
        S_msrVoice
          measureVoice =
            elt->
              getMeasureSegmentUpLink ()->
                getSegmentVoiceUpLink ();

        // fetch measure's part
        S_msrPart
          measurePart =
            measureVoice->
              getVoiceStaffUpLink ()->
                getStaffPartUpLink ();

        rational
          measureWholeNotesDuration =
            measurePart->
              getPartMeasuresWholeNotesDurationsVector () [
                 elt->getMeasureOrdinalNumberInVoice () - 1 ];

        // generate the skip name
        // take voice kind into account may be useful for debug
        switch (measureVoice->getVoiceKind ()) {
          case msrVoice::kVoiceRegular:
            fLilypondCodeOstream <<
              "s%{21%}";
            break;

          case msrVoice::kVoiceHarmony:
          case msrVoice::kVoiceFiguredBass:
            fLilypondCodeOstream <<
              "s%{21%}";
            break;
        } // switch

        // generate the duration of the skip
        fLilypondCodeOstream <<
          wholeNotesAsLilypondString (
            inputLineNumber,
            measureWholeNotesDuration);

        // generate a bar check if this makes the measure full
        if (
          measureWholeNotesDuration
            ==
          elt->getFullMeasureWholeNotesDuration ()
        ) {
          fLilypondCodeOstream << " | ";
        }
      }
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureElementMeasureNumber ();

  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();

  int
    measurePuristNumber =
      elt->getMeasurePuristNumber ();

#ifdef TRACE_OAH
  msrMeasure::msrMeasureEndRegularKind
    measureEndRegularKind =
      elt-> getMeasureEndRegularKind ();
#endif

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrMeasure '" <<
      measureNumber <<
      "', " <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", " <<
      msrMeasure::measureEndRegularKindAsString (
        measureEndRegularKind) <<
      ", measurePuristNumber = '" <<
      measurePuristNumber <<
      "', onGoingRestMeasures = '" <<
      booleanAsString (
        fOnGoingRestMeasures) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasures) {
    fLogOutputStream <<
      endl <<
      "% <!--=== measure '" << measureNumber <<
      "' end, " <<
      msrMeasure::measureEndRegularKindAsString (
        measureEndRegularKind) <<
     "' end, measurePuristNumber = '" << measurePuristNumber << "'" <<
      ", onGoingRestMeasures = '" <<
      booleanAsString (
        fOnGoingRestMeasures) <<
      "', line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  if (! fOnGoingRestMeasures) {
    // handle the measure
    switch (measureKind) {
      case msrMeasure::kMeasureKindUnknown: // should not occur
        fLilypondCodeOstream <<
          "%{ measureKindUnknown, " <<
          measurePuristNumber + 1 <<
          " %}" <<
          endl;
        break;

      case msrMeasure::kMeasureKindRegular:
        {
        }
        break;

      case msrMeasure::kMeasureKindAnacrusis:
        break;

      case msrMeasure::kMeasureKindIncompleteStandalone:
        break;

      case msrMeasure::kMeasureKindIncompleteLastInRepeatCommonPart:
      case msrMeasure::kMeasureKindIncompleteLastInRepeatHookedEnding:
      case msrMeasure::kMeasureKindIncompleteLastInRepeatHooklessEnding:
      case msrMeasure::kMeasureKindIncompleteNextMeasureAfterCommonPart:
      case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHookedEnding:
      case msrMeasure::kMeasureKindIncompleteNextMeasureAfterHooklessEnding:
        break;

      case msrMeasure::kMeasureKindOvercomplete:
        fLilypondCodeOstream <<
          endl <<
          "\\cadenzaOff" <<
          " \\undo \\omit Staff.TimeSignature |" <<
          endl;

        fOnGoingVoiceCadenza = false;
        break;

      case msrMeasure::kMeasureKindCadenza:
        fLilypondCodeOstream <<
          endl <<
          "\\cadenzaOff" <<
          endl <<
          "\\bar \"|\"" << // JMI ???
          endl;

        fOnGoingVoiceCadenza = false;
        break;

      case msrMeasure::kMeasureKindMusicallyEmpty:
        if (gLpsr2LilypondOah->fLilyPondComments) {
          fLilypondCodeOstream <<
            "%{ emptyMeasureKind" <<
            ", line " << inputLineNumber <<
            " %} % " <<
            measurePuristNumber + 1 <<
            endl;
        }
        break;
    } // switch

    if (gLpsr2LilypondOah->fLilyPondComments) {
      gIndenter--;

      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        "% end of " <<
        msrMeasure::measureKindAsString (elt->getMeasureKind ()) <<
        " measure " <<
        measureNumber <<
        ", line " << inputLineNumber <<
        endl <<
        endl;
    }

    if (gLpsr2LilypondOah->fSeparatorLineEveryNMeasures > 0) {
      if (
        fCurrentVoiceMeasuresCounter
          %
        gLpsr2LilypondOah->fSeparatorLineEveryNMeasures
          ==
        0)
        fLilypondCodeOstream <<
          endl <<
          "% ============================= " <<
          endl <<
          endl;
    }
  }

  // should we generate a line break?
  {
    set<string>::const_iterator
      it =
        gLpsr2LilypondOah->
          fBreakLineAfterMeasureNumberSet.find (measureNumber);

    if (it != gLpsr2LilypondOah->fBreakLineAfterMeasureNumberSet.end ()) {
      // yes, generate a line break command
  #ifdef TRACE_OAH
        if (gTraceOah->fTraceLineBreaks) {
          fLogOutputStream <<
            endl <<
            "Adding a LilyPond line break after measure number '" <<
            measureNumber <<
            "', line " << inputLineNumber << " ===-->" <<
            endl;
        }
  #endif

        fLilypondCodeOstream <<
          "\\break" <<
          endl;
    }

  /* JMI
    else {
  #ifdef TRACE_OAH
     if (gTraceOah->fTraceLineBreaks) { // JMI
        fLogOutputStream <<
          endl <<
          "Measure number '" <<
          measureNumber <<
          "' not found in gLpsr2LilypondOah->fBreakLineAfterMeasureNumberSet" <<
          ", line " << inputLineNumber <<
          endl;
      }
  #endif
    }
    */
  }

  // should we generate a page break?
  {
    set<string>::const_iterator
      it =
        gLpsr2LilypondOah->
          fBreakPageAfterMeasureNumberSet.find (measureNumber);

    if (it != gLpsr2LilypondOah->fBreakPageAfterMeasureNumberSet.end ()) {
      // yes, generate a page break command
  #ifdef TRACE_OAH
        if (gTraceOah->fTracePageBreaks) {
          fLogOutputStream <<
            endl <<
            "Adding a LilyPond page break after measure number '" <<
            measureNumber <<
            "', line " << inputLineNumber << " ===-->" <<
            endl;
        }
  #endif

        fLilypondCodeOstream <<
          "\\pageBreak" <<
          endl;
    }

  /* JMI
    else {
  #ifdef TRACE_OAH
     if (gTraceOah->fTracePageBreaks) { // JMI
        fLogOutputStream <<
          endl <<
          "Measure number '" <<
          measureNumber <<
          "' not found in gLpsr2LilypondOah->fBreakPageAfterMeasureNumberSet" <<
          ", line " << inputLineNumber <<
          endl;
      }
  #endif
    }
 */
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStanza& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (! gLpsr2LilypondOah->fNoLilypondLyrics) {
    // don't generate code for the stanza inside the code for the voice
    fGenerateCodeForOngoingNonEmptyStanza =
      ! fOnGoingVoice
        &&
      elt->getStanzaTextPresent ();

    if (fGenerateCodeForOngoingNonEmptyStanza) {
      fLilypondCodeOstream <<
        elt->getStanzaName () << " = " << "\\lyricmode {" <<
        endl;

      gIndenter++;

      fLilypondCodeOstream <<
        // set associatedVoice so that
        // both double hyphens and double underscores can be used
        // to draw hyphenated lines and extenders under melismata correctly
        "\\set associatedVoice = #\"" <<
        elt->getStanzaVoiceUpLink ()->getVoiceName () <<
        "\"" <<
        endl <<
        "\\set ignoreMelismata = ##t" <<
        endl;
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrStanza& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (! gLpsr2LilypondOah->fNoLilypondLyrics) {
    if (fGenerateCodeForOngoingNonEmptyStanza) {
      gIndenter--;

      fLilypondCodeOstream <<
        endl <<
        "}" <<
        endl <<
        endl;
    }

    fGenerateCodeForOngoingNonEmptyStanza = false;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSyllable& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrSyllable '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (! gLpsr2LilypondOah->fNoLilypondLyrics) {
    if (fGenerateCodeForOngoingNonEmptyStanza) {
      switch (elt->getSyllableKind ()) {
        case msrSyllable::kSyllableNone: // JMI
          break;

        case msrSyllable::kSyllableSingle:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeOstream);

          switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
            case kLyricsDurationsImplicit:
              // don't generate a duration for automatic lyrics durations
              break;
            case kLyricsDurationsExplicit:
              fLilypondCodeOstream <<
                elt->syllableWholeNotesAsMsrString ();
              break;
          } // switch

          fLilypondCodeOstream <<
            ' ';

#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableSingle %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableBegin:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeOstream);

          switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
            case kLyricsDurationsImplicit:
              // don't generate a duration for automatic lyrics durations
              break;
            case kLyricsDurationsExplicit:
              fLilypondCodeOstream <<
                elt->syllableWholeNotesAsMsrString ();
              break;
          } // switch

          fLilypondCodeOstream <<
            " -- ";

#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableBegin %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableMiddle:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeOstream);

          switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
            case kLyricsDurationsImplicit:
              // don't generate a duration for automatic lyrics durations
              break;
            case kLyricsDurationsExplicit:
              fLilypondCodeOstream <<
                elt->syllableWholeNotesAsMsrString ();
              break;
          } // switch

          fLilypondCodeOstream <<
            " -- ";

#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableMiddle %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableEnd:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeOstream);

          switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
            case kLyricsDurationsImplicit:
              // don't generate a duration for automatic lyrics durations
              break;
            case kLyricsDurationsExplicit:
              fLilypondCodeOstream <<
                elt->syllableWholeNotesAsMsrString ();
              break;
          } // switch

          fLilypondCodeOstream <<
            ' ';

#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableEnd %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableOnRestNote:
          // generate the syllable in lyrics for rests with syllables
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeOstream);
          fLilypondCodeOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            ' ';
#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
            " %{ syllableOnRestNote %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableSkipRestNote:
          switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
            case kLyricsDurationsImplicit:
              // LilyPond ignores the skip durations when \lyricsto is used
#ifdef TRACE_OAH
              if (gTraceOah->fTraceLyrics) {
                fLilypondCodeOstream <<
                  " %{ NOTHING for kSyllableSkipRestNote " <<
                  elt->syllableWholeNotesAsMsrString () <<
                  " %} ";
              }
#endif
              break;
            case kLyricsDurationsExplicit:
              fLilypondCodeOstream <<
                "\\skip" <<
                elt->syllableWholeNotesAsMsrString () <<
                ' ';
#ifdef TRACE_OAH
              if (gTraceOah->fTraceLyrics) {
                fLilypondCodeOstream <<
                " %{ kLyricsDurationsExplicit %} ";
              }
#endif
              break;
          } // switch
          break;

        case msrSyllable::kSyllableSkipNonRestNote:
          switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
            case kLyricsDurationsImplicit:
              // LilyPond ignores the skip durations when \lyricsto is used
              fLilypondCodeOstream <<
                "\\skip" <<
                elt->syllableWholeNotesAsMsrString () <<
                ' ';
#ifdef TRACE_OAH
              if (gTraceOah->fTraceLyrics) {
                fLilypondCodeOstream <<
                  " %{ syllableSkipNonRestNote lyricsDurationsImplicit %} ";
              }
#endif
              break;
            case kLyricsDurationsExplicit:
              fLilypondCodeOstream <<
                "\\skip" <<
                elt->syllableWholeNotesAsMsrString () <<
                ' ';
#ifdef TRACE_OAH
              if (gTraceOah->fTraceLyrics) {
                fLilypondCodeOstream <<
                " %{ syllableSkipNonRestNote lyricsDurationsExplicit %} ";
              }
#endif
              break;
          } // switch
          break;

        case msrSyllable::kSyllableMeasureEnd:
      // JMI      "| " <<
          if (gLpsr2LilypondOah->fInputLineNumbers) {
            // print the measure end line number as a comment
            fLilypondCodeOstream <<
              "%{ syllableMeasureEnd, line " <<
              elt->getInputLineNumber () <<
              " %}";
          }

      // JMI    fLilypondCodeOstream << endl;
          break;

        case msrSyllable::kSyllableLineBreak:
          // print the measure end line number as a comment
          fLilypondCodeOstream <<
            "%{ syllableLineBreak, line " <<
            elt->getInputLineNumber () <<
            " %} " <<
            "%| % " << // JMI BLARK TO BE FIXED
            elt->getSyllableNextMeasurePuristNumber () <<
            endl;
          break;

        case msrSyllable::kSyllablePageBreak:
          // print the measure end line number as a comment
          fLilypondCodeOstream <<
            "%{ syllablePageBreak, line " <<
            elt->getInputLineNumber () <<
            " %}" <<
            endl;
          break;
      } // switch

      switch (elt->getSyllableExtendKind ()) {
        case msrSyllable::kSyllableExtendNone:
#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableExtendNone %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableExtendEmpty:
#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableExtendEmpty %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableExtendSingle:
          switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
            case kLyricsDurationsImplicit:
              // generate a lyric extender, i.e. a melisma, after this syllable
              fLilypondCodeOstream <<
                "__ ";
              break;
            case kLyricsDurationsExplicit:
              // generate a lyric extender, i.e. a melisma, after this syllable ??? JMI
              fLilypondCodeOstream <<
                "__ ";
              break;
          } // switch
#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableExtendSingle %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableExtendStart:
          switch (gLpsr2LilypondOah->fLyricsDurationsKind) {
            case kLyricsDurationsImplicit:
              // generate a lyric extender, i.e. a melisma, after this syllable
              fLilypondCodeOstream <<
                "__ ";
              break;
            case kLyricsDurationsExplicit:
              // generate a lyric extender, i.e. a melisma, after this syllable ??? JMI
              fLilypondCodeOstream <<
                "__ ";
              break;
          } // switch
#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableExtendStart %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableExtendContinue:
#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableExtendContinue %} ";
          }
#endif
          break;

        case msrSyllable::kSyllableExtendStop:
#ifdef TRACE_OAH
          if (gTraceOah->fTraceLyrics) {
            fLilypondCodeOstream <<
              "%{ syllableExtendStop %} ";
          }
#endif
          break;
      } // switch

    if (
      gLpsr2LilypondOah->fInputLineNumbers
        ||
      gLpsr2LilypondOah->fPositionsInMeasures
    ) {
      generateInputLineNumberAndOrPositionInMeasureAsAComment (
        elt);
      }
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrSyllable& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrSyllable '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrClef& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream << endl;

  // get the clef kind
  msrClefKind
    clefKind =
      elt->getClefKind ();

  if (gLpsr2LilypondOah->fCommentClefChanges) {
  /* JMI
    S_msrClef
      currentVoiceCurrentClef =
        fCurrentVoice->getVoiceCurrentClef ();
*/

    if (
      fCurrentVoiceClef
        &&
      fCurrentVoiceClef->getClefKind () != clefKind
    ) {
      // this is a clef change, comment it

#ifdef TRACE_OAH
      if (gTraceOah->fTraceClefs) {
        gLogOstream <<
          "Commenting clef change from " <<
          fCurrentVoiceClef->asShortString () <<
          " to " <<
          elt->asShortString () <<
          endl;
      }
#endif

      fLilypondCodeOstream << "% ";
    }
  }

  if (clefKind != k_NoClef) {
    fLilypondCodeOstream <<
      "\\clef ";

    switch (clefKind) {
      case k_NoClef:
        break;
      case kTrebleClef:
        fLilypondCodeOstream << "\"treble\"";
        break;
      case kSopranoClef:
        fLilypondCodeOstream << "\"soprano\"";
        break;
      case kMezzoSopranoClef:
        fLilypondCodeOstream << "\"mezzosoprano\"";
        break;
      case kAltoClef:
        fLilypondCodeOstream << "\"alto\"";
        break;
      case kTenorClef:
        fLilypondCodeOstream << "\"tenor\"";
        break;
      case kBaritoneClef:
        fLilypondCodeOstream << "\"baritone\"";
        break;
      case kBassClef:
        fLilypondCodeOstream << "\"bass\"";
        break;
      case kTrebleLine1Clef:
        fLilypondCodeOstream << "\"french\"";
        break;
      case kTrebleMinus15Clef:
        fLilypondCodeOstream << "\"treble_15\"";
        break;
      case kTrebleMinus8Clef:
        fLilypondCodeOstream << "\"treble_8\"";
        break;
      case kTreblePlus8Clef:
        fLilypondCodeOstream << "\"treble^8\"";
        break;
      case kTreblePlus15Clef:
        fLilypondCodeOstream << "\"treble^15\"";
        break;
      case kBassMinus15Clef:
        fLilypondCodeOstream << "\"bass_15\"";
        break;
      case kBassMinus8Clef:
        fLilypondCodeOstream << "\"bass_8\"";
        break;
      case kBassPlus8Clef:
        fLilypondCodeOstream << "\"bass^8\"";
        break;
      case kBassPlus15Clef:
        fLilypondCodeOstream << "\"bass^15\"";
        break;
      case kVarbaritoneClef:
        fLilypondCodeOstream << "\"varbaritone\"";
        break;

      case kTablature4Clef:
      case kTablature5Clef:
      case kTablature6Clef:
      case kTablature7Clef:
        if (gLpsr2LilypondOah->fModernTab) {
          fLilypondCodeOstream <<
            "\"moderntab\"" <<
            endl;
        }
        else {
          fLilypondCodeOstream <<
            "\"tab\"" <<
            endl;
        }
        if (gLpsr2LilypondOah->fTabFullNotation) {
          fLilypondCodeOstream <<
            "\\tabFullNotation" <<
            endl;
        }
        break;

      case kPercussionClef:
        fLilypondCodeOstream << "\"percussion\"";
        break;
      case kJianpuClef:
        fLilypondCodeOstream << "\"%{jianpuClef???%}\"";
        break;
    } // switch

  fLilypondCodeOstream <<
    endl;
  }

  // register current voice clef
  fCurrentVoiceClef = elt;
}

void lpsr2LilypondTranslator::visitEnd (S_msrClef& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrKey& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  switch (fCurrentVoice->getVoiceKind ()) {
    case msrVoice::kVoiceRegular:
    case msrVoice::kVoiceHarmony:
      switch (elt->getKeyKind ()) {
        case msrKey::kTraditionalKind:
          fLilypondCodeOstream <<
            "\\key " <<
            msrQuarterTonesPitchKindAsString (
              gLpsrOah->fLpsrQuarterTonesPitchesLanguageKind,
              elt->getKeyTonicQuarterTonesPitchKind ()) <<
            " \\" <<
            msrKey::keyModeKindAsString (
              elt->getKeyModeKind ()) <<
            endl;
          break;

        case msrKey::kHumdrumScotKind:
          {
            const vector<S_msrHumdrumScotKeyItem>&
              humdrumScotKeyItemsVector =
                elt->getHumdrumScotKeyItemsVector ();

            if (humdrumScotKeyItemsVector.size ()) {
              fLilypondCodeOstream <<
                endl <<
                "\\set Staff.keyAlterations = #`(";

              vector<S_msrHumdrumScotKeyItem>::const_iterator
                iBegin = humdrumScotKeyItemsVector.begin (),
                iEnd   = humdrumScotKeyItemsVector.end (),
                i      = iBegin;

              for ( ; ; ) {
                S_msrHumdrumScotKeyItem item = (*i);

                if (elt->getKeyItemsOctavesAreSpecified ()) {
                  //   JMI   "((octave . step) . alter) ((octave . step) . alter) ...)";
                  //\set Staff.keyAlterations = #`(((3 . 3) . 7) ((3 . 5) . 3) ((3 . 6) . 3))"  \time 2/4


                    fLilypondCodeOstream <<
                      "(" <<
                        "(" <<
                        item->getKeyItemOctave () - 3 <<
                          // in MusicXML, octave number is 4 for the octave
                          // starting with middle C,
                          // and the latter is c' in LilyPond
                        " . " <<
                        item->getKeyItemDiatonicPitchKind () <<
                        ")" <<
                      " . ," <<
                      alterationKindAsLilypondString (
                        item->getKeyItemAlterationKind ()) <<
                      ")";
                }

                else {
                  // Alternatively, for each item in the list, using the more concise format (step . alter) specifies that the same alteration should hold in all octaves.

                    fLilypondCodeOstream <<
                      "(" <<
                      item->getKeyItemDiatonicPitchKind () <<
                      " . ," <<
                      alterationKindAsLilypondString (
                        item->getKeyItemAlterationKind ()) <<
                      ")";
                }

                if (++i == iEnd) break;

                fLilypondCodeOstream << ' ';
              } // for

              fLilypondCodeOstream <<
                ")";
            }

            else {
                msrInternalError (
                  gOahOah->fInputSourceName,
                  elt->getInputLineNumber (),
                  __FILE__, __LINE__,
                  "Humdrum/Scot key items vector is empty");
            }
          }
          break;
      } // switch
      break;

    case msrVoice::kVoiceFiguredBass:
      // not \key should be generated in \figuremode
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrKey& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrKey '" <<
      elt->asString () <<
      "'"  <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTime& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  msrTime::msrTimeSymbolKind
    timeSymbolKind =
      elt->getTimeSymbolKind ();

  const vector<S_msrTimeItem>&
    timeItemsVector =
      elt->getTimeItemsVector ();

/* JMI
  // is this the end of a senza misura fragment?
  if (
    fVoiceIsCurrentlySenzaMisura
      &&
    timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
    fLilypondCodeOstream <<
      "\\undo\\omit Staff.TimeSignature" <<
      endl;

    fVoiceIsCurrentlySenzaMisura = false;
  }
*/

  // handle the time
  if (timeSymbolKind == msrTime::kTimeSymbolSenzaMisura) {
    // senza misura time

    /* JMI
    fLilypondCodeOstream <<
      "\\omit Staff.TimeSignature" <<
      endl;
*/

    fVoiceIsCurrentlySenzaMisura = true;
  }

  else {
    // con misura time

    int timesItemsNumber =
      timeItemsVector.size ();

    if (timesItemsNumber) {
      // should there be a single number?
      switch (timeSymbolKind) {
        case msrTime::kTimeSymbolCommon:
          break;
        case msrTime::kTimeSymbolCut:
          break;
        case msrTime::kTimeSymbolNote:
          break;
        case msrTime::kTimeSymbolDottedNote:
          break;
        case msrTime::kTimeSymbolSingleNumber:
          fLilypondCodeOstream <<
            "\\once\\override Staff.TimeSignature.style = #'single-digit" <<
            endl;
          break;
        case msrTime::kTimeSymbolSenzaMisura:
          break;
        case msrTime::kTimeSymbolNone:
          break;
      } // switch

      if (! elt->getTimeIsCompound ()) {
        // simple time
        // \time "3/4" for 3/4
        // or senza misura

        S_msrTimeItem
          timeItem =
            timeItemsVector [0]; // the only element;

        // fetch the time item beat numbers vector
        const vector<int>&
          beatsNumbersVector =
            timeItem->
              getTimeBeatsNumbersVector ();

        // should the time be numeric?
        if (
          timeSymbolKind == msrTime::kTimeSymbolNone
            ||
          gLpsr2LilypondOah->fNumericalTime) {
          fLilypondCodeOstream <<
            "\\numericTimeSignature ";
        }

        fLilypondCodeOstream <<
          "\\time " <<
          beatsNumbersVector [0] << // the only element
          "/" <<
          timeItem->getTimeBeatValue () <<
          endl;
      }

      else {
        // compound time
        // \compoundMeter #'(3 2 8) for 3+2/8
        // \compoundMeter #'((3 8) (2 8) (3 4)) for 3/8+2/8+3/4
        // \compoundMeter #'((3 2 8) (3 4)) for 3+2/8+3/4

        fLilypondCodeOstream <<
          "\\compoundMeter #`(";

        // handle all the time items in the vector
        for (int i = 0; i < timesItemsNumber; i++) {
          S_msrTimeItem
            timeItem =
              timeItemsVector [i];

          // fetch the time item beat numbers vector
          const vector<int>&
            beatsNumbersVector =
              timeItem->
                getTimeBeatsNumbersVector ();

          int beatsNumbersNumber =
            beatsNumbersVector.size ();

          // first generate the opening parenthesis
          fLilypondCodeOstream <<
            "(";

          // then generate all beats numbers in the vector
          for (int j = 0; j < beatsNumbersNumber; j++) {
            fLilypondCodeOstream <<
              beatsNumbersVector [j] <<
              ' ';
          } // for

          // then generate the beat type
          fLilypondCodeOstream <<
            timeItem->getTimeBeatValue ();

          // and finally generate the closing parenthesis
          fLilypondCodeOstream <<
            ")";

          if (i != timesItemsNumber - 1) {
            fLilypondCodeOstream <<
              ' ';
          }
        } // for

      fLilypondCodeOstream <<
        ")" <<
        endl;
      }
    }

    else {
      // there are no time items
      if (timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
        msrInternalError (
          gOahOah->fInputSourceName,
          elt->getInputLineNumber (),
          __FILE__, __LINE__,
          "time items vector is empty");
      }
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrTime& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTranspose& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  int  transposeDiatonic  = elt->getTransposeDiatonic ();
  int  transposeChromatic = elt->getTransposeChromatic ();
  int  transposeOctaveChange = elt->getTransposeOctaveChange ();
  bool transposeDouble = elt->getTransposeDouble ();

/*
  // transposition in LilyPond is relative to c',
  // i.e. the C in the middle of the piano keyboard

The diatonic element specifies the number of pitch steps needed to go from written to sounding pitch. This allows for correct spelling of enharmonic transpositions.

The chromatic element represents the number of semitones needed to get from written to sounding pitch. This value does not include octave-change values; the values for both elements need to be added to the written pitch to get the correct sounding pitch.

The octave-change element indicates how many octaves to add to get from written pitch to sounding pitch.

If the double element is present, it indicates that the music is doubled one octave down from what is currently written (as is the case for mixed cello / bass parts in orchestral literature).
*/

  // determine transposition pitch
  msrQuarterTonesPitchKind
    transpositionPitchKind = k_NoQuarterTonesPitch_QTP;

  switch (transposeChromatic) {
    case -11:
      switch (transposeDiatonic) {
        case -7:
          transpositionPitchKind = kC_Sharp_QTP;
          break;

        case -6:
          transpositionPitchKind = kD_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -10:
      switch (transposeDiatonic) {
        case -6:
          transpositionPitchKind = kD_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -9:
      switch (transposeDiatonic) {
        case -6:
          transpositionPitchKind = kD_Sharp_QTP;
          break;

        case -5:
          transpositionPitchKind = kE_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -8:
      switch (transposeDiatonic) {
        case -5:
          transpositionPitchKind = kE_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -7:
      switch (transposeDiatonic) {
        case -4:
          transpositionPitchKind = kF_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -6:
      switch (transposeDiatonic) {
        case -4:
          transpositionPitchKind = kF_Sharp_QTP;
          break;

        case -3:
          transpositionPitchKind = kG_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -5:
      switch (transposeDiatonic) {
        case -3:
          transpositionPitchKind = kG_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -4:
      switch (transposeDiatonic) {
        case -3:
          transpositionPitchKind = kG_Sharp_QTP;
          break;

        case -2:
          transpositionPitchKind = kA_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -3:
      switch (transposeDiatonic) {
        case -2:
          transpositionPitchKind = kA_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -2:
      switch (transposeDiatonic) {
        case -2:
          transpositionPitchKind = kA_Sharp_QTP;
          break;

        case -1:
          transpositionPitchKind = kB_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case -1:
      switch (transposeDiatonic) {
        case -1:
          transpositionPitchKind = kB_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 0:
      switch (transposeDiatonic) {
        case 0:
          transpositionPitchKind = kC_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 1:
      switch (transposeDiatonic) {
        case 0:
          transpositionPitchKind = kC_Sharp_QTP;
          break;

        case 1:
          transpositionPitchKind = kD_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 2:
      switch (transposeDiatonic) {
        case 1:
          transpositionPitchKind = kD_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 3:
      switch (transposeDiatonic) {
        case 1:
          transpositionPitchKind = kD_Sharp_QTP;
          break;

        case 2:
          transpositionPitchKind = kE_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 4:
      switch (transposeDiatonic) {
        case 2:
          transpositionPitchKind = kE_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 5:
      switch (transposeDiatonic) {
        case 3:
          transpositionPitchKind = kF_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 6:
      switch (transposeDiatonic) {
        case 3:
          transpositionPitchKind = kF_Sharp_QTP;
          break;

        case 4:
          transpositionPitchKind = kG_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 7:
      switch (transposeDiatonic) {
        case 4:
          transpositionPitchKind = kG_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 8:
      switch (transposeDiatonic) {
        case 4:
          transpositionPitchKind = kG_Sharp_QTP;
          break;

        case 5:
          transpositionPitchKind = kA_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 9:
      switch (transposeDiatonic) {
        case 5:
          transpositionPitchKind = kA_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 10:
      switch (transposeDiatonic) {
        case 5:
          transpositionPitchKind = kA_Sharp_QTP;
          break;

        case 6:
          transpositionPitchKind = kB_Flat_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 11:
      switch (transposeDiatonic) {
        case 6:
          transpositionPitchKind = kB_Natural_QTP;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    default:
      {
        stringstream s;

        s <<
          "transpose chromatic '" << transposeChromatic <<
          "' is not between -12 and 12, ignored";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          elt->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
  } // switch

  // 4 is the octave starting with middle C
  int transpositionOctave;

  if (transposeChromatic < 0) {
    transpositionOctave = 3;
  }
  else {
    transpositionOctave = 4;
  }

  // take the transpose octave change into account
  transpositionOctave += transposeOctaveChange;

  // take the transpose double if any into account
  if (transposeDouble) {
    transpositionOctave--;
  }

  string
    transpositionPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOah->
          fLpsrQuarterTonesPitchesLanguageKind,
        transpositionPitchKind);

  string
    transpositionOctaveAsString =
      absoluteOctaveAsLilypondString (
        transpositionOctave);

/* JMI
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTranspositions) {
    fLilypondCodeOstream << // JMI
      "Handlling transpose '" <<
      elt->transposeAsString () <<
      "' ignored because it is already present in voice \"" <<
      fCurrentVoice->getVoiceName () <<
      "\"" <<
      / * JMI
      getStaffName () <<
      "\" in part " <<
      fStaffPartUpLink->getPartCombinedName () <<
      * /
      endl <<
      ", transpositionPitch: " <<
      transpositionPitchAsString <<
      ", transpositionOctave: " <<
      transpositionOctaveAsString <<
      "(" << transpositionOctave << ")" <<
      endl;
    }
#endif
*/

  // now we can generate the transpostion command
  fLilypondCodeOstream <<
    "\\transposition " <<
    transpositionPitchKindAsString <<
    transpositionOctaveAsString <<
    ' ' <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTranspose& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPartNameDisplay& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrPartNameDisplay" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  string partNameDisplayValue =
    elt->getPartNameDisplayValue ();

  fLilypondCodeOstream <<
    "\\set Staff.instrumentName = \"" <<
    partNameDisplayValue <<
    "\"" <<
    endl <<
 // JMI ???   "\\tweak break-visibility #end-of-line-visible" <<
 //   endl <<
    "\\tweak RehearsalMark.self-alignment-X #LEFT" <<
    endl <<
    "\\mark\\markup{\"" <<
    partNameDisplayValue <<
    "\"}" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPartAbbreviationDisplay& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrPartAbbreviationDisplay" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  string partAbbreviationDisplayValue =
    elt->getPartAbbreviationDisplayValue ();

  fLilypondCodeOstream <<
    "\\set Staff.shortInstrumentName = " <<
    nameAsLilypondString (partAbbreviationDisplayValue) <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTempo& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  const list<S_msrWords>&
    tempoWordsList =
      elt->getTempoWordsList ();

  int tempoWordsListSize = tempoWordsList.size ();

  msrDottedDuration tempoBeatUnit  = elt->getTempoBeatUnit ();
  string            tempoPerMinute = elt->getTempoPerMinute ();

  msrTempo::msrTempoParenthesizedKind
    tempoParenthesizedKind =
      elt->getTempoParenthesizedKind ();

  switch (elt->getTempoPlacementKind ()) {
    case msrPlacementKind::kPlacementNone:
      break;
    case msrPlacementKind::kPlacementAbove:
      // by default, so nothing to do
      break;
    case msrPlacementKind::kPlacementBelow:
      fLilypondCodeOstream <<
        "\\once\\override Score.MetronomeMark.direction = #DOWN" <<
        endl;
      break;
    } // switch

  switch (elt->getTempoKind ()) {
    case msrTempo::k_NoTempoKind:
      break;

    case msrTempo::kTempoBeatUnitsWordsOnly:
        fLilypondCodeOstream <<
          "\\tempo ";

        if (tempoWordsListSize) {
          list<S_msrWords>::const_iterator
            iBegin = tempoWordsList.begin (),
            iEnd   = tempoWordsList.end (),
            i      = iBegin;

          for ( ; ; ) {
            S_msrWords words = (*i);

            fLilypondCodeOstream <<
              "\"" << words->getWordsContents () << "\"";

            if (++i == iEnd) break;

            fLilypondCodeOstream <<
              ' ';
          } // for

          fLilypondCodeOstream <<
            endl;
        }
      break;

    case msrTempo::kTempoBeatUnitsPerMinute:
      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeOstream <<
            "\\tempo " <<
            "\\markup {" <<
            endl;

          gIndenter++;

          fLilypondCodeOstream <<
            "\\concat {" <<
            endl;

          gIndenter++;

          gIndenter++;

          if (tempoWordsListSize) {
            list<S_msrWords>::const_iterator
              iBegin = tempoWordsList.begin (),
              iEnd   = tempoWordsList.end (),
              i      = iBegin;

            for ( ; ; ) {
              S_msrWords words = (*i);

              fLilypondCodeOstream <<
                "\"" << words->getWordsContents () << "\"";

              if (++i == iEnd) break;

              fLilypondCodeOstream <<
                ' ';
            } // for
          }

          fLilypondCodeOstream <<
            "(";
          if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
            fLilypondCodeOstream <<
              " \\smaller \\general-align #Y #DOWN \\note {";
          }
          else {
            fLilypondCodeOstream <<
              "\\smaller \\general-align #Y #DOWN \\note #\"";
          }

          fLilypondCodeOstream <<
            dottedDurationAsLilypondStringWithoutBackSlash (
              inputLineNumber,
              tempoBeatUnit);

          if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
            fLilypondCodeOstream <<
              "} #UP";
          }
          else {
            fLilypondCodeOstream <<
              "\" #UP";
          }

          fLilypondCodeOstream <<
            endl <<
            "\" = \"" <<
            endl <<
            tempoPerMinute <<
            ")" <<
            endl;

          gIndenter--;

          fLilypondCodeOstream << endl;

          gIndenter--;

          fLilypondCodeOstream <<
            "}" <<
            endl;

          gIndenter--;

          fLilypondCodeOstream <<
            "}" <<
            endl;
        break;

      case msrTempo::kTempoParenthesizedNo:
        fLilypondCodeOstream <<
          "\\tempo " <<
          "\\markup {" <<
          endl;

        gIndenter++;

        if (tempoWordsListSize) {
          list<S_msrWords>::const_iterator
            iBegin = tempoWordsList.begin (),
            iEnd   = tempoWordsList.end (),
            i      = iBegin;

          for ( ; ; ) {
            S_msrWords words = (*i);

            fLilypondCodeOstream <<
              "\"" << words->getWordsContents () << "\"";

            if (++i == iEnd) break;

            fLilypondCodeOstream <<
              ' ';
          } // for
        }

        fLilypondCodeOstream <<
          "\\concat {" <<
          endl;

        gIndenter++;

        if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
          fLilypondCodeOstream <<
            " \\smaller \\general-align #Y #DOWN \\note {";
        }
        else {
          fLilypondCodeOstream <<
            " \\smaller \\general-align #Y #DOWN \\note #\"";
        }

        fLilypondCodeOstream <<
          dottedDurationAsLilypondStringWithoutBackSlash (
            inputLineNumber,
            tempoBeatUnit);

        if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
          fLilypondCodeOstream <<
            "} #UP";
        }
        else {
          fLilypondCodeOstream <<
            "\" #UP";
        }

        fLilypondCodeOstream <<
          endl <<
          "\" = \"" <<
          endl <<
          tempoPerMinute <<
        endl;

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;
      } // switch
      break;

    case msrTempo::kTempoBeatUnitsEquivalence:
      fLilypondCodeOstream <<
        "\\tempo ";

      if (tempoWordsListSize) {
        list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fLilypondCodeOstream <<
            "\"" << words->getWordsContents () << "\"";

          if (++i == iEnd) break;

          fLilypondCodeOstream <<
            ' ';
        } // for
      }

      fLilypondCodeOstream <<
        ' ' <<
        "\\markup {" <<
        endl;

      gIndenter++;

      fLilypondCodeOstream <<
        "\\concat {" <<
        endl;

      gIndenter++;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeOstream <<
            "(" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch

      gIndenter++;

      if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fLilypondCodeOstream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fLilypondCodeOstream <<
          " \\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fLilypondCodeOstream <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          tempoBeatUnit);

      if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fLilypondCodeOstream <<
          "} #UP";
      }
      else {
        fLilypondCodeOstream <<
          "\" #UP";
      }

      fLilypondCodeOstream <<
        endl <<
        "\" = \"" <<
        endl;

      fLilypondCodeOstream <<
        "(";
      if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fLilypondCodeOstream <<
          " \\smaller \\general-align #Y #DOWN \\note {";
      }
      else {
        fLilypondCodeOstream <<
          " \\smaller \\general-align #Y #DOWN \\note #\"";
      }

      fLilypondCodeOstream <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          elt->getTempoEquivalentBeatUnit ());

      if (gLpsrOah->versionNumberGreaterThanOrEqualTo ("2.20")) {
        fLilypondCodeOstream <<
          "} #UP";
      }
      else {
        fLilypondCodeOstream <<
          "\" #UP";
      }

      fLilypondCodeOstream << endl;

      gIndenter--;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeOstream <<
            ")" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;
      break;

    case msrTempo::kTempoNotesRelationShip:
      fLilypondCodeOstream <<
        "\\tempoRelationship #\"";

      if (tempoWordsListSize) {
        list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;

        for ( ; ; ) {
          S_msrWords words = (*i);

          fLilypondCodeOstream <<
     // JMI       "\"" <<
            words->getWordsContents (); // JMI <<
      // JMI      "\"";

          if (++i == iEnd) break;

          fLilypondCodeOstream <<
            ' ';
        } // for
      }

      fLilypondCodeOstream <<
        "\"";

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeOstream <<
            " ##t";
          break;
        case msrTempo::kTempoParenthesizedNo:
          fLilypondCodeOstream <<
            " ##f";
          break;
      } // switch

      fLilypondCodeOstream << endl;
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitStart (S_msrTempoRelationshipElements& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTempoRelationshipElements" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\fixed b' {" <<
    endl;

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTempoRelationshipElements& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTempoRelationshipElements" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  fLilypondCodeOstream <<
    endl <<
    "}" <<
    endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrTempoNote& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTempoNote" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "b" <<
    wholeNotesAsLilypondString (
      elt->getInputLineNumber (),
      elt->getTempoNoteWholeNotes ()) <<
      ' ';
}

void lpsr2LilypondTranslator::visitStart (S_msrTempoTuplet& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTempoTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  const msrTupletFactor&
    tempoTupletFactor =
      elt->getTempoTupletFactor ();

  fLilypondCodeOstream <<
    "\\tuplet " <<
    tempoTupletFactor.getTupletActualNotes () <<
    "/" <<
    tempoTupletFactor.getTupletNormalNotes () << " { ";
}

void lpsr2LilypondTranslator::visitEnd (S_msrTempoTuplet& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTempoTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "}";
}

void lpsr2LilypondTranslator::visitEnd (S_msrTempo& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrArticulation& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrArticulation& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrFermata& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/*
Articulations can be attached to rests as well as notes but they cannot be attached to multi-measure rests. A special predefined command, \fermataMarkup, is available for at- taching a fermata to a multi-measure rest (and only a multi-measure rest). This creates a MultiMeasureRestText object.
*/

/* JMI
  switch (elt->getArticulationPlacement ()) {
    case kPlacementNone:
      // nothing needed
      break;
    case kPlacementAbove:
      fLilypondCodeOstream << "^";
      break;
    case kPlacementBelow:
      fLilypondCodeOstream << "_";
      break;
  } // switch

  // don't generate fermatas for chord member notes
  if (false && fOnGoingNotesStack.size () > 0) { // JMI
    switch (elt->getFermataTypeKind ()) {
      case msrFermata::kFermataTypeNone:
        // no placement needed
        break;
      case msrFermata::kFermataTypeUpright:
        // no placement needed
        break;
      case msrFermata::kFermataTypeInverted:
        fLilypondCodeOstream << "_";
        break;
    } // switch

    switch (elt->getFermataKind ()) {
      case msrFermata::kNormalFermataKind:
        fLilypondCodeOstream << "\\fermata ";
        break;
      case msrFermata::kAngledFermataKind:
        fLilypondCodeOstream << "\\shortfermata ";
        break;
      case msrFermata::kSquareFermataKind:
        fLilypondCodeOstream << "\\longfermata ";
        break;
    } // switch
  }
*/
}

void lpsr2LilypondTranslator::visitEnd (S_msrFermata& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrArpeggiato& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* VIRER JMI
  */
}

void lpsr2LilypondTranslator::visitEnd (S_msrArpeggiato& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrNonArpeggiato& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* VIRER JMI
  */
}

void lpsr2LilypondTranslator::visitEnd (S_msrNonArpeggiato& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnical& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the technical here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnical& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTechnicalWithInteger" <<
      ", fOnGoingChord = " <<
      booleanAsString (fOnGoingChord) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the technicalWithInteger here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTechnicalWithFloat" <<
      ", fOnGoingChord = " <<
      booleanAsString (fOnGoingChord) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the technicalWithFloat here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithString& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the technicalWithString here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOrnament& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the ornament here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrOrnament& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrGlissando& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the glissando here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrGlissando& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSlide& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the slide here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrSlide& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSingleTremolo& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // don't generate the singleTremolo here,
  // the note or chord will do it in its visitEnd () method
}

void lpsr2LilypondTranslator::visitEnd (S_msrSingleTremolo& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrDoubleTremolo& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // get double tremolo number of repeats
  int numberOfRepeats =
    elt->getDoubleTremoloNumberOfRepeats ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceTremolos) {
    fLilypondCodeOstream <<
      "% visitStart (S_msrDoubleTremolo&)" <<
      endl;

    gIndenter++;

    fLilypondCodeOstream <<
      "% doubleTremoloSoundingWholeNotes = " <<
      elt->getDoubleTremoloSoundingWholeNotes () <<
      endl <<

      "% gdoubleTremoloElementsDuration = " <<
      elt->getDoubleTremoloElementsDuration () <<
      endl <<

      "% doubleTremoloMarksNumber = " <<
      elt->getDoubleTremoloMarksNumber () <<
      endl <<

      "% numberOfRepeats = " <<
      numberOfRepeats <<
      endl;

    gIndenter++;
  }
#endif

  fLilypondCodeOstream <<
    "\\repeat tremolo " << numberOfRepeats << " {";

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  fLilypondCodeOstream <<
    "}" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrDynamics& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrDynamics& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOtherDynamics& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrOtherDynamics& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrWords& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrWords& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSlur& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrSlur& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrSlur " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

/*
void lpsr2LilypondTranslator::visitStart (S_msrChordSlurLink& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrChordSlurLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrChordSlurLink& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrChordSlurLink " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}
*/

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrLigature& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrLigature& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrWedge& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrWedge& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateNoteBeams (S_msrNote note)
{
  const list<S_msrBeam>&
    noteBeams =
      note->getNoteBeams ();

  if (noteBeams.size ()) {
    list<S_msrBeam>::const_iterator i;
    for (
      i=noteBeams.begin ();
      i!=noteBeams.end ();
      i++
    ) {
      S_msrBeam beam = (*i);

      // LilyPond will take care of multiple beams automatically,
      // so we need only generate code for the first number (level)
      switch (beam->getBeamKind ()) {

        case msrBeam::kBeginBeam:
          if (beam->getBeamNumber () == 1)
            if (! gLpsr2LilypondOah->fNoBeams) {
              fLilypondCodeOstream << "[ ";
            }
          break;

        case msrBeam::kContinueBeam:
          break;

        case msrBeam::kEndBeam:
          if (beam->getBeamNumber () == 1)
            if (! gLpsr2LilypondOah->fNoBeams) {
              fLilypondCodeOstream << "] ";
            }
          break;

        case msrBeam::kForwardHookBeam:
          break;

        case msrBeam::kBackwardHookBeam:
          break;

        case msrBeam::k_NoBeam:
          break;
      } // switch
    } // for
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateNoteSlurs (S_msrNote note)
{
  const list<S_msrSlur>&
    noteSlurs =
      note->getNoteSlurs ();

  if (noteSlurs.size ()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=noteSlurs.begin ();
      i!=noteSlurs.end ();
      i++
    ) {
      S_msrSlur slur = (*i);

      /* JMI ???
        \slurDashed, \slurDotted, \slurHalfDashed,
        \slurHalfSolid, \slurDashPattern, \slurSolid
      */

      switch (slur->getSlurTypeKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kRegularSlurStart:
          fLilypondCodeOstream << "( ";
          break;
        case msrSlur::kPhrasingSlurStart:
          fLilypondCodeOstream << "\\( ";
          break;
        case msrSlur::kSlurContinue:
          break;
        case msrSlur::kRegularSlurStop:
          fLilypondCodeOstream << ") ";
          break;
        case msrSlur::kPhrasingSlurStop:
          fLilypondCodeOstream << "\\) ";
          break;
      } // switch
    } // for
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateGraceNotesGroup (
  S_msrGraceNotesGroup graceNotesGroup)
{
  /*
    1. no slash, no slur: \grace
    2. slash and slur: \acciaccatura, LilyPond will slur it JMI
    3. slash but no slur: \slashedGrace
    4. no slash but slur: \appoggiatura, LilyPond will slur it JMI
  */

#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    fLilypondCodeOstream <<
      "% --> generating code for grace notes group " <<
      graceNotesGroup->asString () <<
      ", line " << graceNotesGroup->getInputLineNumber () <<
      endl;
  }
#endif

  bool doGenerateASlurIfPresent = true;

  switch (graceNotesGroup->getGraceNotesGroupKind ()) {
    case msrGraceNotesGroup::kGraceNotesGroupBefore:
      if (graceNotesGroup->getGraceNotesGroupIsSlashed ()) {
        if (graceNotesGroup->getGraceNotesGroupIsTied ()) {
          fLilypondCodeOstream <<
            "\\acciaccatura";
          doGenerateASlurIfPresent = false;
        }
        else {
          fLilypondCodeOstream <<
            "\\slashedGrace";
        }
      }

      else {
        if (graceNotesGroup->getGraceNotesGroupIsTied ()) {
          fLilypondCodeOstream <<
            "\\appoggiatura";
          doGenerateASlurIfPresent = false;
        }
        else {
          fLilypondCodeOstream <<
            "\\grace";
        }
      }
      break;

    case msrGraceNotesGroup::kGraceNotesGroupAfter:
      // don't generate anything here
      break;
  } // switch

  fLilypondCodeOstream <<
    " { ";

  // force durations to be displayed explicitly
  // at the beginning of the grace notes
  fLastMetWholeNotes = rational (0, 1);

  // generate the notes in the grace notes group
  list<S_msrMeasureElement>&
    graceNotesGroupElementsList =
      graceNotesGroup->
        getGraceNotesGroupElementsList ();

  int
    graceNotesGroupElementsListSize =
      graceNotesGroupElementsList.size ();

  if (graceNotesGroupElementsListSize) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = graceNotesGroupElementsList.begin (),
      iEnd   = graceNotesGroupElementsList.end (),
      i      = iBegin;

    int elementNumber = 0;

    for ( ; ; ) {
      S_msrElement element = (*i);

      elementNumber += 1;
      if (
        // note?
        S_msrNote
          note =
            dynamic_cast<msrNote*>(&(*element))
        ) {
          // generate things before the note
          generateCodeRightBeforeNote (note);

          // generate the note itself
          generateCodeForNote (note);

          // generate things after the note
          generateCodeRightAfterNote (note);

          // generate the note beams if any,
          // unless the note is chord member
          if (! note->getNoteBelongsToAChord ()) {
            generateNoteBeams (note);
          }

          if (graceNotesGroup->getGraceNotesGroupIsBeamed ()) {
            if (elementNumber == 1) {
              fLilypondCodeOstream << "[ ";
            }
            else if (elementNumber == graceNotesGroupElementsListSize) {
              fLilypondCodeOstream << "] ";
            }
          }

          // generate the note slurs if any,
          // unless the note is chord member
          // or LilyPond will take care of that
          if (
            ! note->getNoteBelongsToAChord ()
              &&
            doGenerateASlurIfPresent
          ) {
            generateNoteSlurs (note);
          }
        }

      else if (
        // chord?
        S_msrChord
          chord =
            dynamic_cast<msrChord*>(&(*element))
        ) {
          generateChordInGraceNotesGroup (chord);
        }

      else {
        stringstream s;

        fLilypondCodeOstream <<
          "grace notes group elements list in '" <<
          graceNotesGroup->asString () <<
          "' is empty" <<
          ", line " << graceNotesGroup->getInputLineNumber ();

        msrInternalError (
          gOahOah->fInputSourceName,
          graceNotesGroup->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }

      if (++i == iEnd) break;
      fLilypondCodeOstream <<
        ' ';
    } // for

    fLilypondCodeOstream << "} ";
  }

  else {
    stringstream s;

    fLilypondCodeOstream <<
      "grace notes group elements list in '" <<
      graceNotesGroup->asString () <<
      "' is empty" <<
      ", line " << graceNotesGroup->getInputLineNumber ();

    msrInternalError (
      gOahOah->fInputSourceName,
      graceNotesGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // force durations to be displayed explicitly
  // at the end of the grace notes
  fLastMetWholeNotes = rational (0, 1);
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrGraceNotesGroup " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingGraceNotesGroup = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrGraceNotesGroup " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fOnGoingGraceNotesGroup = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrAfterGraceNotesGroup& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrAfterGraceNotesGroup " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

 // JMI exists? if (elt->getGraceNotesGroupIsSlashed ()) {}
  fLilypondCodeOstream <<
    "\\afterGrace { ";
}

void lpsr2LilypondTranslator::visitStart (S_msrAfterGraceNotesGroupContents& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrAfterGraceNotesGroupContents " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  // write a first closing right bracket right now
  fLilypondCodeOstream <<
    "} { ";

  // force durations to be displayed explicitly
  // at the beginning of the after grace notes contents
  fLastMetWholeNotes = rational (0, 1);
}

void lpsr2LilypondTranslator::visitEnd (S_msrAfterGraceNotesGroupContents& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrAfterGraceNotesGroupContents " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "} ";
}

void lpsr2LilypondTranslator::visitEnd (S_msrAfterGraceNotesGroup& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrAfterGraceNotesGroup " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateBeforeNoteSpannersIfAny (
  S_msrNote note)
{
  const list<S_msrSpanner>&
    noteSpanners =
      note->getNoteSpanners ();

  if (noteSpanners.size ()) {
    list<S_msrSpanner>::const_iterator i;
    for (
      i=noteSpanners.begin ();
      i!=noteSpanners.end ();
      i++
    ) {
      S_msrSpanner
        spanner = (*i);

      bool doGenerateSpannerCode = true;

      switch (spanner->getSpannerKind ()) {
        case msrSpanner::kSpannerDashes:
          break;
        case msrSpanner::kSpannerWavyLine:
          if (spanner->getSpannerNoteUpLink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            doGenerateSpannerCode = false;
          }
          break;
      } // switch

      if (doGenerateSpannerCode) {
        switch (spanner->getSpannerKind ()) {
          case msrSpanner::kSpannerDashes:
          case msrSpanner::kSpannerWavyLine:
            break;
        } // switch

        generateCodeForSpannerBeforeNote (spanner);
      }
    } // for
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateAfterNoteSpannersIfAny (
  S_msrNote note)
{
  const list<S_msrSpanner>&
    noteSpanners =
      note->getNoteSpanners ();

  if (noteSpanners.size ()) {
    list<S_msrSpanner>::const_iterator i;
    for (
      i=noteSpanners.begin ();
      i!=noteSpanners.end ();
      i++
    ) {
      S_msrSpanner
        spanner = (*i);

      bool doGenerateSpannerCode = true;

      switch (spanner->getSpannerKind ()) {
        case msrSpanner::kSpannerDashes:
          break;
        case msrSpanner::kSpannerWavyLine:
          if (spanner->getSpannerNoteUpLink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            doGenerateSpannerCode = false;
          }
          break;
      } // switch

      if (doGenerateSpannerCode) {
        switch (spanner->getSpannerKind ()) {
          case msrSpanner::kSpannerDashes:
          case msrSpanner::kSpannerWavyLine:
            break;
        } // switch

        generateCodeForSpannerAfterNote (spanner);
      }
    } // for
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrNote& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting note " <<
      elt->asShortString () <<
      ", fOnGoingChord: " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingGraceNotesGroup: " << booleanAsString (fOnGoingGraceNotesGroup) <<
      ", fOnGoingChordGraceNotesGroupLink: " << booleanAsString (fOnGoingChordGraceNotesGroupLink) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  bool noteIsToBeIgnored = false;

  // is this note to be ignored?
  if (fOnGoingChord && fOnGoingGraceNotesGroup) { // JMI
    noteIsToBeIgnored = true;
  }

  if (fOnGoingRestMeasures) {
    switch (elt->getNoteKind ()) {
      case msrNote::kRestNote:
        // don't handle rest measures, that's done in visitEnd (S_msrRestMeasures&)
          /*
          if (elt->getNoteOccupiesAFullMeasure ()) {
            bool inhibitRestMeasuresBrowsing =
              fVisitedLpsrScore->
                getMsrScore ()->
                  getInhibitRestMeasuresBrowsing ();

            if (inhibitRestMeasuresBrowsing) {
              if (
                gMsrOah->fTraceMsrVisitors
                  ||
                gTraceOah->ffTraceRestMeasures) {
                gLogOstream <<
                  "% ==> visiting rest measures is ignored" <<
                  endl;
              }

  #ifdef TRACE_OAH
    if (gTraceOah->fTraceNotesDetails) {
      gLogOstream <<
        "% ==> returning from visitStart (S_msrNote&)" <<
      endl;
    }
  #endif

            noteIsToBeIgnored = true;
            }
          }
          */

#ifdef TRACE_OAH
          if (
            gMsrOah->fTraceMsrVisitors
              ||
            gTraceOah->fTraceRestMeasures
          ) {
            gLogOstream <<
              "% ==> start visiting rest notes is ignored upon note " <<
              elt->asString () <<
              endl;
          }
#endif

          noteIsToBeIgnored = true;
        break;

      case msrNote::kSkipNote:
        if (elt->getNoteGraceNotesGroupUpLink ()) {
#ifdef TRACE_OAH
          if (
            gMsrOah->fTraceMsrVisitors
              ||
            gTraceOah->fTraceNotes
          ) {
            gLogOstream <<
              "% ==> start visiting skip notes is ignored upon note " <<
              elt->asString () <<
              endl;
          }
#endif

          noteIsToBeIgnored = true;
        }
        break;

      case msrNote::kGraceNote:
#ifdef TRACE_OAH
          if (
            gMsrOah->fTraceMsrVisitors
              ||
            gTraceOah->fTraceGraceNotes
          ) {
            gLogOstream <<
              "% ==> start visiting grace notes is ignored upon note " <<
              elt->asString () <<
              endl;
          }
#endif

          noteIsToBeIgnored = true;
        break;

      case msrNote::kGraceChordMemberNote:
#ifdef TRACE_OAH
          if (
            gMsrOah->fTraceMsrVisitors
              ||
            gTraceOah->fTraceGraceNotes
          ) {
            gLogOstream <<
              "% ==> start visiting chord grace notes is ignored upon note " <<
              elt->asString () <<
              endl;
          }
#endif

          noteIsToBeIgnored = true;
        break;

      default:
        ;
    } // switch
  }

  if (noteIsToBeIgnored) {
    return;
  }

  // get the note's grace notes group after ??? JMI
  S_msrGraceNotesGroup
    noteGraceNotesGroupAfter =
      elt->getNoteGraceNotesGroupAfter ();

  // print the note's grace notes group after opener if any
  if (noteGraceNotesGroupAfter) {
    fLilypondCodeOstream <<
      "\\afterGrace { ";
  }

// * JMI mal placé???
  // print the note's grace notes group before if any,
  // unless the note belongs to a chord
  bool doGenerateNoteGraceNotesGroupBefore = true;

  if (fOnGoingChord && fOnGoingGraceNotesGroup) {
    doGenerateNoteGraceNotesGroupBefore = false;
  }

  if (doGenerateNoteGraceNotesGroupBefore) {
    S_msrGraceNotesGroup
      noteGraceNotesGroupBefore =
        elt->getNoteGraceNotesGroupBefore ();

    if (noteGraceNotesGroupBefore) {
      generateGraceNotesGroup (
        noteGraceNotesGroupBefore);
    }
  }
//*/

  // print the note scordaturas if any
  const list<S_msrScordatura>&
    noteScordaturas =
      elt->getNoteScordaturas ();

  if (noteScordaturas.size ()) {
    fLilypondCodeOstream <<
      "<<" <<
      endl;

    gIndenter++;

    list<S_msrScordatura>::const_iterator
      iBegin = noteScordaturas.begin (),
      iEnd   = noteScordaturas.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrScordatura
        scordatura = (*i);

      const list<S_msrStringTuning>&
        scordaturaStringTuningsList =
          scordatura->
            getScordaturaStringTuningsList ();

      gIndenter++;

      fLilypondCodeOstream <<
        "\\new Staff \\with { alignAboveContext = \"" <<
        elt->getNoteMeasureUpLink ()->
          getMeasureSegmentUpLink ()->
            getSegmentVoiceUpLink ()->
              getVoiceStaffUpLink ()->
                getStaffName () <<
        "\" } {" <<
        endl;

      gIndenter++;

      fLilypondCodeOstream <<
        "\\hide Staff.Stem" <<
        endl <<
        "\\hide Staff.TimeSignature" <<
        endl <<
        "\\small" <<
        endl <<
        "\\once \\override Score.RehearsalMark.self-alignment-X = #LEFT" <<
        endl <<
        "\\mark\\markup {\\small\\bold \"Scordatura\"}" <<
        endl <<
        "<";

      if (scordaturaStringTuningsList.size ()) {
        list<S_msrStringTuning>::const_iterator
          iBegin = scordaturaStringTuningsList.begin (),
          iEnd   = scordaturaStringTuningsList.end (),
          i      = iBegin;
        for ( ; ; ) {
          S_msrStringTuning
            stringTuning = (*i);

          fLilypondCodeOstream <<
            stringTuningAsLilypondString (
              elt->getInputLineNumber (),
              stringTuning);

          if (++i == iEnd) break;

          fLilypondCodeOstream << ' ';
        } // for
      }

      fLilypondCodeOstream <<
        ">4" <<
        endl;

      gIndenter--;

      fLilypondCodeOstream <<
        "}" <<
        endl;

      gIndenter--;

      fLilypondCodeOstream <<
        "{" <<
        endl;

      gIndenter++;

      if (++i == iEnd) break;
    } // for
  }

  // should the note actually be printed?
  msrPrintObjectKind
    notePrintObjectKind =
      elt->getNotePrintObjectKind ();

  if (notePrintObjectKind != fCurrentNotePrinObjectKind) {
    switch (notePrintObjectKind) {
      case kPrintObjectNone:
        // JMI
        break;
      case kPrintObjectYes:
      /* JMI ???
        fLilypondCodeOstream <<
          endl <<
          "\\revert NoteHead.color" <<
          endl;
          */
        break;
      case kPrintObjectNo:
        fLilypondCodeOstream <<
          endl <<
          "\\temporary\\override NoteHead.color = #(rgb-color 0.5 0.5 0.5)" <<
          endl;
        break;
    } // switch

    fCurrentNotePrinObjectKind = notePrintObjectKind;
  }

  // print the note slashes if any
  const list<S_msrSlash>&
    noteSlashes =
      elt->getNoteSlashes ();

  if (noteSlashes.size ()) {
    list<S_msrSlash>::const_iterator i;
    for (
      i=noteSlashes.begin ();
      i!=noteSlashes.end ();
      i++
    ) {
      S_msrSlash
        slash =
          (*i);

      switch (slash->getSlashTypeKind ()) {
        case k_NoSlashType:
          break;

        case kSlashTypeStart:
          fLilypondCodeOstream <<
            endl <<
            "\\override Staff.NoteHead.style = #'slash " <<
            endl;
          break;

        case kSlashTypeStop:
          fLilypondCodeOstream <<
            endl <<
            "\\revert Staff.NoteHead.style " <<
            endl;
          break;
      } // switch

      switch (slash->getUseDotsKind ()) {
        case k_NoUseDots:
          break;

        case kUseDotsYes:
          fLilypondCodeOstream <<
            endl <<
            "\\override Staff.NoteHead.style = #'slash " <<
            endl;
          break;

        case kUseDotsNo:
          fLilypondCodeOstream <<
            endl <<
            "\\revert Staff.NoteHead.style " <<
            endl;
          break;
      } // switch

      switch (slash->getSlashUseStemsKind ()) {
        case k_NoSlashUseStems:
          break;

        case kSlashUseStemsYes:
          fLilypondCodeOstream <<
            endl <<
            "\\undo \\hide Staff.Stem " <<
            endl;
          break;

        case kSlashUseStemsNo:
          fLilypondCodeOstream <<
            endl <<
            "\\hide Staff.Stem " <<
            endl;
          break;
      } // switch
    } // for
  }

  // print the note wedges circled tips if any
  const list<S_msrWedge>&
    noteWedges =
      elt->getNoteWedges ();

  if (noteWedges.size ()) {
    list<S_msrWedge>::const_iterator i;
    for (
      i=noteWedges.begin ();
      i!=noteWedges.end ();
      i++
    ) {
      S_msrWedge wedge = (*i);

      switch (wedge->getWedgeKind ()) {
        case msrWedge::kWedgeKindNone:
          break;

        case msrWedge::kWedgeCrescendo:
          switch (wedge->getWedgeNienteKind ()) {
            case msrWedge::kWedgeNienteYes:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Hairpin.circled-tip = ##t " <<
                endl;
              break;
            case msrWedge::kWedgeNienteNo:
              break;
            } // switch
          break;

        case msrWedge::kWedgeDecrescendo:
          switch (wedge->getWedgeNienteKind ()) {
            case msrWedge::kWedgeNienteYes:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Hairpin.circled-tip = ##t " <<
                endl;
              break;
            case msrWedge::kWedgeNienteNo:
              break;
            } // switch
          break;

        case msrWedge::kWedgeStop:
        /* JMI
          fLilypondCodeOstream <<
            "\\! ";
            */
          break;
      } // switch
    } // for
  }

  // print the note slurs line types if any,
  // unless the note is chord member
  if (! elt->getNoteBelongsToAChord ()) {
    const list<S_msrSlur>&
      noteSlurs =
        elt->getNoteSlurs ();

    if (noteSlurs.size ()) {
      list<S_msrSlur>::const_iterator i;
      for (
        i=noteSlurs.begin ();
        i!=noteSlurs.end ();
        i++
      ) {
        S_msrSlur slur = (*i);

        /*
        \slurDashed, \slurDotted, \slurHalfDashed,
        \slurHalfSolid, \slurDashPattern, \slurSolid
        */

        switch (slur->getSlurTypeKind ()) {
          case msrSlur::kRegularSlurStart:
          case msrSlur::kPhrasingSlurStart:
            switch (slur->getSlurLineTypeKind ()) {
              case kLineTypeSolid:
                /* JMI ???
                fLilypondCodeOstream <<
                  "\\once\\slurSolid ";
                */
                break;
              case kLineTypeDashed:
                fLilypondCodeOstream <<
                  "\\once\\slurDashed ";
                break;
              case kLineTypeDotted:
                fLilypondCodeOstream <<
                  "\\once\\slurDotted ";
                break;
              case kLineTypeWavy:
                fLilypondCodeOstream <<
                  "\\once\\slurWavy "; // JMI
                break;
            } // switch
            break;
          default:
            ;
        } // switch
      } // for
    }
  }

  // print the note glissandos styles if any
  const list<S_msrGlissando>&
    noteGlissandos =
      elt->getNoteGlissandos ();

  if (noteGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (
      i=noteGlissandos.begin ();
      i!=noteGlissandos.end ();
      i++
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
          break;

        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando style
          switch (glissando->getGlissandoLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Glissando.style = #'zigzag" <<
                endl;
              break;
          } // switch
          break;

        case msrGlissando::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // print the note slides styles if any, implemented as glissandos
  const list<S_msrSlide>&
    noteSlides =
      elt->getNoteSlides ();

  if (noteSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (
      i=noteSlides.begin ();
      i!=noteSlides.end ();
      i++
    ) {
      S_msrSlide slide = (*i);

      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
          break;

        case msrSlide::kSlideTypeStart:
          // generate the glissando style
          switch (slide->getSlideLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Glissando.style = #'zigzag" <<
                endl;
              break;
          } // switch
          break;

        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

  // print the note glissandos with text if any
  if (noteGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (
      i=noteGlissandos.begin ();
      i!=noteGlissandos.end ();
      i++
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
          break;

        case msrGlissando::kGlissandoTypeStart:
          {
            string
              glissandoTextValue =
                glissando->getGlissandoTextValue ();

            if (glissandoTextValue.size ()) {
              // generate the glissando text on itself
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Glissando.details.glissando-text = \"" <<
                glissandoTextValue <<
                "\"" <<
                endl <<
                "\\glissandoTextOn" <<
                endl;
            }
          }
          break;

        case msrGlissando::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // print the note slides with text if any
  if (noteSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (
      i=noteSlides.begin ();
      i!=noteSlides.end ();
      i++
    ) {
      S_msrSlide slide = (*i);

      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
          break;

        case msrSlide::kSlideTypeStart:
          {
            string
              slideTextValue =
                slide->getSlideTextValue ();

            if (slideTextValue.size ()) {
              // generate the slide text on itself
              fLilypondCodeOstream <<
                endl <<
                "\\once\\override Glissando.details.glissando-text = \"" <<
                slideTextValue <<
                "\"" <<
                endl <<
                "\\glissandoTextOn" <<
                endl;
            }
          }
          break;

        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

  // print the 'before note' spanners if any
//  generateBeforeNoteSpannersIfAny (elt);

  // should the note be parenthesized?
  msrNote::msrNoteHeadParenthesesKind
    noteHeadParenthesesKind =
      elt->getNoteHeadParenthesesKind ();

  switch (noteHeadParenthesesKind) {
    case msrNote::kNoteHeadParenthesesYes:
      fLilypondCodeOstream << "\\parenthesize ";
      break;
    case msrNote::kNoteHeadParenthesesNo:
      break;
  } // switch

  // print the note technicals with string if any
  const list<S_msrTechnicalWithString>&
    noteTechnicalWithStrings =
      elt->getNoteTechnicalWithStrings ();

  if (noteTechnicalWithStrings.size ()) {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=noteTechnicalWithStrings.begin ();
      i!=noteTechnicalWithStrings.end ();
      i++
    ) {
      S_msrTechnicalWithString technicalWithString = (*i);

      switch (technicalWithString->getTechnicalWithStringKind ()) {
        case msrTechnicalWithString::kHammerOn:
          switch (technicalWithString->getTechnicalWithStringTypeKind ()) {
            case kTechnicalTypeStart:
              {
                rational
                  noteSoundingWholeNotes =
                    elt->getNoteSoundingWholeNotes ();

                rational
                  halfWholeNotes =
                    noteSoundingWholeNotes /2;

                fLilypondCodeOstream <<
                  "\\after " <<
                  wholeNotesAsLilypondString (
                    elt->getInputLineNumber (),
                    halfWholeNotes) <<
                  " ^\"H\" ";
              }
              break;
            case kTechnicalTypeStop:
              break;
            case k_NoTechnicalType:
              break;
          } // switch
          break;
        case msrTechnicalWithString::kHandbell: // JMI
          break;
        case msrTechnicalWithString::kOtherTechnical: // JMI
          break;
        case msrTechnicalWithString::kPluck: // JMI
          break;
        case msrTechnicalWithString::kPullOff:
          switch (technicalWithString->getTechnicalWithStringTypeKind ()) {
            case kTechnicalTypeStart:
              {
                rational
                  noteSoundingWholeNotes =
                    elt->getNoteSoundingWholeNotes ();

                rational
                  halfWholeNotes =
                    noteSoundingWholeNotes /2;

                fLilypondCodeOstream <<
                  "\\after " <<
                  wholeNotesAsLilypondString (
                    elt->getInputLineNumber (),
                    halfWholeNotes) <<
                  " ^\"P\" ";
              }
              break;
            case kTechnicalTypeStop:
              break;
            case k_NoTechnicalType:
              break;
          } // switch
          break;
      } // switch
    } // for
  }

  // is the note a cue note?
  if (elt->getNoteIsACueNote ()) {
    fLilypondCodeOstream <<
      "\\once \\override NoteHead.font-size = -3 ";
  }

  // has the note an octave shift up or down?
  if (! fOnGoingChord) {
    // the octave shift for the chords has already been generated
    S_msrOctaveShift
      noteOctaveShift =
        elt->
          getNoteOctaveShift ();

    if (noteOctaveShift) {
      generateCodeForOctaveShift (
        noteOctaveShift);
    }
  }

  // generate things before the note
  generateBeforeNoteSpannersIfAny (elt);
  generateCodeRightBeforeNote (elt);

  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // generate the note itself as a LilyPond string
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  generateCodeForNote (elt);

  // generate things after the note
  generateCodeRightAfterNote (elt);
  generateAfterNoteSpannersIfAny (elt);

  if (
    gLpsr2LilypondOah->fInputLineNumbers
      ||
    gLpsr2LilypondOah->fPositionsInMeasures
  ) {
    generateInputLineNumberAndOrPositionInMeasureAsAComment (
      elt);
  }

  fOnGoingNotesStack.push (elt);
}

void lpsr2LilypondTranslator::visitEnd (S_msrNote& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting note " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  bool noteIsToBeIgnored = false;

  // is this note to be ignored?
  if (fOnGoingChord && fOnGoingGraceNotesGroup) { // JMI
    noteIsToBeIgnored = true;
  }
  if (fOnGoingRestMeasures) {
    switch (elt->getNoteKind ()) {
      case msrNote::kRestNote:
        // don't handle rest measuress, that's done in visitEnd (S_msrRestMeasures&)
          if (elt->getNoteOccupiesAFullMeasure ()) {
            bool inhibitRestMeasuresBrowsing =
              fVisitedLpsrScore->
                getMsrScore ()->
                  getInhibitRestMeasuresBrowsing ();

            if (inhibitRestMeasuresBrowsing) {
#ifdef TRACE_OAH
              if (
                gTraceOah->fTraceNotes
                  ||
                gTraceOah->fTraceRestMeasures
              ) {
                gLogOstream <<
                  "% ==> end visiting rest measures is ignored" <<
                  endl;
              }
#endif

#ifdef TRACE_OAH
              if (gTraceOah->fTraceNotesDetails) {
                gLogOstream <<
                  "% ==> returning from visitEnd (S_msrNote&)" <<
                  endl;
              }
#endif

              noteIsToBeIgnored = true;
            }
          }
        break;

      case msrNote::kSkipNote:
        if (elt->getNoteGraceNotesGroupUpLink ()) {
#ifdef TRACE_OAH
          if (
            gMsrOah->fTraceMsrVisitors
              ||
            gTraceOah->fTraceNotes
          ) {
            gLogOstream <<
              "% ==> end visiting skip notes is ignored" <<
              endl;
          }
#endif

          noteIsToBeIgnored = true;
        }
        break;

      case msrNote::kGraceNote:
#ifdef TRACE_OAH
          if (
            gMsrOah->fTraceMsrVisitors
              ||
            gTraceOah->fTraceGraceNotes) {
            gLogOstream <<
              "% ==> end visiting grace notes is ignored" <<
              endl;
          }
#endif

          noteIsToBeIgnored = true;
        break;

      default:
        ;
    } // switch
  }

  if (noteIsToBeIgnored) {
    return;
  }

  // fetch the note single tremolo
  S_msrSingleTremolo
    noteSingleTremolo =
      elt->getNoteSingleTremolo ();

  if (noteSingleTremolo) {
    // generate code for the single tremolo only
    // if note doesn't belong to a chord,
    // otherwise it will be generated for the chord itself
    if (! elt->getNoteBelongsToAChord ()) {
      fLilypondCodeOstream <<
        singleTremoloDurationAsLilypondString (
          noteSingleTremolo);
    }
  }

  // print the note words if any,
  // which should precede the articulations in LilyPond
  // generate code for the words only
  // if note doesn't belong to a chord,
  // otherwise it will be generated for the chord itself
  if (! elt->getNoteBelongsToAChord ()) {
    const list<S_msrWords>&
      noteWords =
        elt->getNoteWords ();

    if (noteWords.size ()) {
      list<S_msrWords>::const_iterator i;
      for (
        i=noteWords.begin ();
        i!=noteWords.end ();
        i++
      ) {
        msrPlacementKind
          wordsPlacementKind =
            (*i)->getWordsPlacementKind ();

        string wordsContents =
          (*i)->getWordsContents ();

        msrFontStyleKind
          wordsFontStyleKind =
            (*i)->getWordsFontStyleKind ();

        S_msrFontSize
          wordsFontSize =
            (*i)->getWordsFontSize ();

        msrFontWeightKind
          wordsFontWeightKind =
            (*i)->getWordsFontWeightKind ();

        string markup;

        {
          // create markup apart to have its length available
          stringstream s;

          switch (wordsPlacementKind) {
            case kPlacementNone:
              s << "^";
              break;
            case kPlacementAbove:
              s << "^";
              break;
            case kPlacementBelow:
              s << "_";
              break;
          } // switch

          s <<
            "\\markup" << " { ";

          switch (wordsFontStyleKind) {
            case kFontStyleNone:
              break;
            case kFontStyleNormal:
              // LilyPond produces 'normal style' text by default
              break;
            case KFontStyleItalic:
              s <<
                "\\italic ";
              break;
          } // switch

          switch (wordsFontWeightKind) {
            case kFontWeightNone:
              break;
            case kFontWeightNormal:
              // LilyPond produces 'normal weight' text by default
              break;
            case kFontWeightBold:
              s <<
                "\\bold ";
              break;
          } // switch

          switch (wordsFontSize->getFontSizeKind ()) {
            case msrFontSize::kFontSizeNone:
              break;
            case msrFontSize::kFontSizeXXSmall:
              s <<
                "\\tiny ";
              break;
            case msrFontSize::kFontSizeXSmall:
              s <<
                "\\smaller ";
              break;
            case msrFontSize::kFontSizeSmall:
              s <<
                "\\small ";
              break;
            case msrFontSize::kFontSizeMedium:
              s <<
                "\\normalsize ";
              break;
            case msrFontSize::kFontSizeLarge:
              s <<
                "\\large ";
              break;
            case msrFontSize::kFontSizeXLarge:
              s <<
                "\\larger ";
              break;
            case msrFontSize::kFontSizeXXLarge:
              s <<
                "\\huge ";
              break;
            case msrFontSize::kFontSizeNumeric:
            /* JMI
              s <<
                "%{ " <<
                wordsFontSize->getFontNumericSize () <<
                " points %} ";
                */
              break;
          } // switch

          // does wordsContents contains end of lines?
          size_t endOfLineFound = wordsContents.find ("\n");

          if (endOfLineFound == string::npos) {
            s <<
     // JMI         doubleQuoteStringIfNonAlpha (wordsContents) <<
              "\"" << wordsContents << "\"";
            }
          else {
            s <<
              gIndenter.indentMultiLineString (
                generateAColumnForMarkup (
                  wordsContents,
                  markupColumnKindLeftAligned));
          }

          s <<
            " } ";

          markup = s.str ();
        }

        fLilypondCodeOstream <<
          markup;
      } // for
    }
  }

/* TOO EARLY FOR ALL OF THEM??? JMI
  // print the note articulations if any
  if (! fOnGoingChord) {
    const list<S_msrArticulation>&
      noteArticulations =
        elt->getNoteArticulations ();

    if (noteArticulations.size ()) {
      list<S_msrArticulation>::const_iterator i;
      for (
        i=noteArticulations.begin ();
        i!=noteArticulations.end ();
        i++
      ) {
        S_msrArticulation articulation = (*i);
        switch (articulation->getArticulationKind ()) {
          case msrArticulation::kFermata: // handle this better JMI
            if (
              // fermata?
              S_msrFermata
                fermata =
                  dynamic_cast<msrFermata*>(&(*articulation))
              ) {
              switch (fermata->getFermataTypeKind ()) {
                case msrFermata::kFermataTypeNone:
                  // no placement needed
                  break;
                case msrFermata::kFermataTypeUpright:
                  // no placement needed
                  break;
                case msrFermata::kFermataTypeInverted:
                  fLilypondCodeOstream << "_";
                  break;
              } // switch

              switch (fermata->getFermataKind ()) {
                case msrFermata::kNormalFermataKind:
                  fLilypondCodeOstream << "\\fermata ";
                  break;
                case msrFermata::kAngledFermataKind:
                  fLilypondCodeOstream << "\\shortfermata ";
                  break;
                case msrFermata::kSquareFermataKind:
                  fLilypondCodeOstream << "\\longfermata ";
                  break;
              } // switch
            }
            else {
              stringstream s;

              s <<
                "note articulation '" <<
                articulation->asString () <<
                "' has 'fermata' kind, but is not of type S_msrFermata" <<
                ", line " << articulation->getInputLineNumber ();

              msrInternalError (
                gOahOah->fInputSourceName,
                articulation->getInputLineNumber (),
                __FILE__, __LINE__,
                s.str ());
            }
            break;

          default:
            generateNoteArticulation ((*i));
            fLilypondCodeOstream <<
              ' ';
        } // switch
      } // for
    }
  }
  */

  // print the note technicals if any
  const list<S_msrTechnical>&
    noteTechnicals =
      elt->getNoteTechnicals ();

  if (noteTechnicals.size ()) {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=noteTechnicals.begin ();
      i!=noteTechnicals.end ();
      i++
    ) {

      fLilypondCodeOstream <<
        technicalAsLilypondString ((*i));

      switch ((*i)->getTechnicalPlacementKind ()) {
        case kPlacementNone:
          break;
        case kPlacementAbove:
          fLilypondCodeOstream << "^";
          break;
        case kPlacementBelow:
          fLilypondCodeOstream << "_";
          break;
      } // switch

      fLilypondCodeOstream << ' ';
    } // for
  }

  // print the note technicals with integer if any,
  // but not for chord member notes strings:
  // they should appear after the chord itself
  switch (elt->getNoteKind ()) {
    case msrNote::kChordMemberNote:
       break;

    default:
      {
        const list<S_msrTechnicalWithInteger>&
          noteTechnicalWithIntegers =
            elt->getNoteTechnicalWithIntegers ();

        if (noteTechnicalWithIntegers.size ()) {
          list<S_msrTechnicalWithInteger>::const_iterator i;
          for (
            i=noteTechnicalWithIntegers.begin ();
            i!=noteTechnicalWithIntegers.end ();
            i++
          ) {

            S_msrTechnicalWithInteger
                technicalWithInteger = (*i);

            fLilypondCodeOstream <<
              technicalWithIntegerAsLilypondString (
                technicalWithInteger);

            switch (technicalWithInteger->getTechnicalWithIntegerPlacementKind ()) {
              case kPlacementNone:
                break;
              case kPlacementAbove:
                fLilypondCodeOstream << "^";
                break;
              case kPlacementBelow:
                fLilypondCodeOstream << "_";
                break;
            } // switch

            fLilypondCodeOstream << ' ';
          } // for
        }
      }
  } // switch

  // print the note technicals with float if any,
  // but not for chord member notes strings:
  // they should appear after the chord itself
  switch (elt->getNoteKind ()) {
    case msrNote::kChordMemberNote:
       break;

    default:
      {
        const list<S_msrTechnicalWithFloat>&
          noteTechnicalWithFloats =
            elt->getNoteTechnicalWithFloats ();

        if (noteTechnicalWithFloats.size ()) {
          list<S_msrTechnicalWithFloat>::const_iterator i;
          for (
            i=noteTechnicalWithFloats.begin ();
            i!=noteTechnicalWithFloats.end ();
            i++
          ) {

            S_msrTechnicalWithFloat
                technicalWithFloat = (*i);

            fLilypondCodeOstream <<
              technicalWithFloatAsLilypondString (
                technicalWithFloat);

            switch (technicalWithFloat->getTechnicalWithFloatPlacementKind ()) {
              case kPlacementNone:
                break;
              case kPlacementAbove:
                fLilypondCodeOstream << "^";
                break;
              case kPlacementBelow:
                fLilypondCodeOstream << "_";
                break;
            } // switch

            fLilypondCodeOstream << ' ';
          } // for
        }
      }
  } // switch

  // print the note technicals with string if any
  const list<S_msrTechnicalWithString>&
    noteTechnicalWithStrings =
      elt->getNoteTechnicalWithStrings ();

  if (noteTechnicalWithStrings.size ()) {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=noteTechnicalWithStrings.begin ();
      i!=noteTechnicalWithStrings.end ();
      i++
    ) {

      fLilypondCodeOstream <<
        technicalWithStringAsLilypondString ((*i));

      switch ((*i)->getTechnicalWithStringPlacementKind ()) {
        case kPlacementNone:
          break;
        case kPlacementAbove:
          fLilypondCodeOstream << "^";
          break;
        case kPlacementBelow:
          fLilypondCodeOstream << "_";
          break;
      } // switch

      fLilypondCodeOstream << ' ';
    } // for
  }

  // print the note ornaments if any
  list<S_msrOrnament>
    noteOrnaments =
      elt->getNoteOrnaments ();

  if (noteOrnaments.size ()) {
    list<S_msrOrnament>::const_iterator i;
    for (
      i=noteOrnaments.begin ();
      i!=noteOrnaments.end ();
      i++
    ) {
      S_msrOrnament
        ornament = (*i);

      generateOrnament (ornament); // some ornaments are not yet supported
    } // for
  }

  // print the note dynamics if any
  if (! fOnGoingChord) {
    const list<S_msrDynamics>&
      noteDynamics =
        elt->getNoteDynamics ();

    if (noteDynamics.size ()) {
      list<S_msrDynamics>::const_iterator i;
      for (
        i=noteDynamics.begin ();
        i!=noteDynamics.end ();
        i++
      ) {
        S_msrDynamics
          dynamics = (*i);

        switch (dynamics->getDynamicsPlacementKind ()) {
          case kPlacementNone:
     // JMI       fLilypondCodeOstream << "-3";
            break;
          case kPlacementAbove:
            fLilypondCodeOstream << "^";
            break;
          case kPlacementBelow:
            // this is done by LilyPond by default
            break;
        } // switch

        fLilypondCodeOstream <<
          dynamicsAsLilypondString (dynamics) << ' ';
      } // for
    }
  }

  // print the note other dynamics if any
  if (! fOnGoingChord) {
    const list<S_msrOtherDynamics>&
      noteOtherDynamics =
        elt->getNoteOtherDynamics ();

    if (noteOtherDynamics.size ()) {
      list<S_msrOtherDynamics>::const_iterator i;
      for (
        i=noteOtherDynamics.begin ();
        i!=noteOtherDynamics.end ();
        i++
      ) {
        S_msrOtherDynamics
          otherDynamics = (*i);

        switch (otherDynamics->getOtherDynamicsPlacementKind ()) {
          case kPlacementNone:
            fLilypondCodeOstream << "-";
            break;
          case kPlacementAbove:
            fLilypondCodeOstream << "^";
            break;
          case kPlacementBelow:
            // this is done by LilyPond by default
            break;
        } // switch

        fLilypondCodeOstream <<
          "\\otherDynamic \"" <<
          otherDynamics->getOtherDynamicsString () <<
          "\" ";
      } // for
    }
  }

  // print the note beams if any,
  // unless the note is chord member
  if (! elt->getNoteBelongsToAChord ()) {
    generateNoteBeams (elt);
  }

  // print the note slurs if any,
  // unless the note is chord member
  if (! elt->getNoteBelongsToAChord ()) {
    generateNoteSlurs (elt);
  }

  // print the note ligatures if any
  const list<S_msrLigature>&
    noteLigatures =
      elt->getNoteLigatures ();

  if (noteLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin ();
      i!=noteLigatures.end ();
      i++
    ) {

      switch ((*i)->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
        case msrLigature::kLigatureStart:
   // JMI       fLilypondCodeOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
          fLilypondCodeOstream << "\\] ";
          break;
      } // switch
    } // for
  }

  // print the note wedges if any
  const list<S_msrWedge>&
    noteWedges =
      elt->getNoteWedges ();

  if (noteWedges.size ()) {
    list<S_msrWedge>::const_iterator i;
    for (
      i=noteWedges.begin ();
      i!=noteWedges.end ();
      i++
    ) {
      S_msrWedge wedge = (*i);

      switch (wedge->getWedgeKind ()) {
        case msrWedge::kWedgeKindNone:
          break;

        case msrWedge::kWedgeCrescendo:
          switch (wedge->getWedgePlacementKind ()) {
            case kPlacementNone:
              break;
            case kPlacementAbove:
              fLilypondCodeOstream <<
                "^";
              break;
            case kPlacementBelow:
              fLilypondCodeOstream <<
                "_";
              break;
            } // switch
          fLilypondCodeOstream <<
            "\\< ";
          break;

        case msrWedge::kWedgeDecrescendo:
          switch (wedge->getWedgePlacementKind ()) {
            case kPlacementNone:
              break;
            case kPlacementAbove:
              fLilypondCodeOstream <<
                "^";
              break;
            case kPlacementBelow:
              fLilypondCodeOstream <<
                "_";
              break;
            } // switch
          fLilypondCodeOstream <<
            "\\> ";
          break;

        case msrWedge::kWedgeStop:
          fLilypondCodeOstream <<
            "\\! ";
          break;
      } // switch
    } // for
  }

  // print the note articulations if any,
  // which should follo the dynamics and wedges in LilyPond
  if (! fOnGoingChord) {
    const list<S_msrArticulation>&
      noteArticulations =
        elt->getNoteArticulations ();

    if (noteArticulations.size ()) {
      list<S_msrArticulation>::const_iterator i;
      for (
        i=noteArticulations.begin ();
        i!=noteArticulations.end ();
        i++
      ) {
        S_msrArticulation articulation = (*i);

        switch (articulation->getArticulationKind ()) {
          case msrArticulation::kFermata: // handle this better JMI
            if (
              // fermata?
              S_msrFermata
                fermata =
                  dynamic_cast<msrFermata*>(&(*articulation))
              ) {
              switch (fermata->getFermataTypeKind ()) {
                case msrFermata::kFermataTypeNone:
                  // no placement needed
                  break;
                case msrFermata::kFermataTypeUpright:
                  // no placement needed
                  break;
                case msrFermata::kFermataTypeInverted:
                  fLilypondCodeOstream << "_";
                  break;
              } // switch

              switch (fermata->getFermataKind ()) {
                case msrFermata::kNormalFermataKind:
                  if (
                    elt->getNoteOccupiesAFullMeasure ()
                      &&
                    elt->getNoteIsARest ()
                  ) {
                    fLilypondCodeOstream << "\\fermataMarkup ";
                  }
                  else {
                    fLilypondCodeOstream << "\\fermata ";
                  }
                  break;
                case msrFermata::kAngledFermataKind:
                  fLilypondCodeOstream << "\\shortfermata ";
                  break;
                case msrFermata::kSquareFermataKind:
                  fLilypondCodeOstream << "\\longfermata ";
                  break;
              } // switch
            }
            else {
              stringstream s;

              s <<
                "note articulation '" <<
                articulation->asString () <<
                "' has 'fermata' kind, but is not of type S_msrFermata" <<
                ", line " << articulation->getInputLineNumber ();

              msrInternalError (
                gOahOah->fInputSourceName,
                articulation->getInputLineNumber (),
                __FILE__, __LINE__,
                s.str ());
            }
            break;

          default:
            generateNoteArticulation ((*i));
            fLilypondCodeOstream <<
              " ";
        } // switch
      } // for
    }
  }

  // print the note glissandos if any
  const list<S_msrGlissando>&
    noteGlissandos =
      elt->getNoteGlissandos ();

  if (noteGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (
      i=noteGlissandos.begin ();
      i!=noteGlissandos.end ();
      i++
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
          break;

        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando itself
          fLilypondCodeOstream <<
            "\\glissando " <<
            "\\glissandoTextOff ";
          break;

        case msrGlissando::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // print the note slides if any, implemented as glissandos
  const list<S_msrSlide>&
    noteSlides =
      elt->getNoteSlides ();

  if (noteSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (
      i=noteSlides.begin ();
      i!=noteSlides.end ();
      i++
    ) {
      S_msrSlide slide = (*i);

      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
          break;

        case msrSlide::kSlideTypeStart:
          // generate the glissando itself
          fLilypondCodeOstream <<
            "\\glissando " <<
            "\\glissandoTextOff ";
          break;

        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

  // print the note spanners if any
  const list<S_msrSpanner>&
    noteSpanners =
      elt->getNoteSpanners ();

  if (noteSpanners.size ()) {
    list<S_msrSpanner>::const_iterator i;
    for (
      i=noteSpanners.begin ();
      i!=noteSpanners.end ();
      i++
    ) {
      S_msrSpanner
        spanner = (*i);

      bool doGenerateSpannerCode = true;

      switch (spanner->getSpannerKind ()) {
        case msrSpanner::kSpannerDashes:
          break;
        case msrSpanner::kSpannerWavyLine:
          if (spanner->getSpannerNoteUpLink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            doGenerateSpannerCode = false;
          }
          break;
      } // switch

      if (doGenerateSpannerCode) {
        switch (spanner->getSpannerPlacementKind ()) {
          case kPlacementNone:
     // JMI       fLilypondCodeOstream << "-3";
            break;
          case kPlacementAbove:
            fLilypondCodeOstream << "^";
            break;
          case kPlacementBelow:
            // this is done by LilyPond by default
            break;
        } // switch

        generateCodeForSpannerAfterNote (spanner);
      }
    } // for
  }

  // are there note scordaturas?
  const list<S_msrScordatura>&
    noteScordaturas =
      elt->getNoteScordaturas ();

  if (noteScordaturas.size ()) {
    gIndenter--;

    fLilypondCodeOstream <<
      endl <<
      "}" <<
      endl;

    gIndenter--;

    fLilypondCodeOstream <<
      ">>" <<
      endl;
  }

  if (elt->getNoteIsFollowedByGraceNotesGroup ()) { // JMI
    if (! elt->getNoteIsARest ()) {
      fLilypondCodeOstream <<
       " % NoteIsFollowedByGraceNotesGroup" <<
        endl; // JMI ???
    }
  }

  // get the note's grace notes now,
  // unless the note belongs to a grace notes group link
  if (! fOnGoingChordGraceNotesGroupLink) {
    S_msrGraceNotesGroup
      noteGraceNotesGroupAfter =
        elt->getNoteGraceNotesGroupAfter ();

    // print the note's grace notes after group closer if any
    if (noteGraceNotesGroupAfter) {
      fLilypondCodeOstream <<
        "} ";
      generateGraceNotesGroup (
        noteGraceNotesGroupAfter);
    }
  }

  fOnGoingNotesStack.pop ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOctaveShift& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrOctaveShift& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrAccordionRegistration& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  int highDotsNumber =
    elt->getHighDotsNumber ();
  int middleDotsNumber =
    elt->getMiddleDotsNumber ();
  int lowDotsNumber =
    elt->getLowDotsNumber ();

  string numbersToBeUsed;
  bool   nonZeroNumberHasBeenIssued = false;

  // the numbers should be written in the order 'high, middle, low'
  // and 0 ahead of the specification is forbidden

  if (highDotsNumber > 0) {
    numbersToBeUsed +=
      to_string (highDotsNumber);
    nonZeroNumberHasBeenIssued = true;
  }

  if (middleDotsNumber > 0) {
    numbersToBeUsed +=
      to_string (middleDotsNumber);
    nonZeroNumberHasBeenIssued = true;
  }
  else {
    if (nonZeroNumberHasBeenIssued) {
      numbersToBeUsed +=
        to_string (middleDotsNumber);
    }
  }

  numbersToBeUsed +=
    to_string (lowDotsNumber);

  fLilypondCodeOstream <<
    "\\discant \"" <<
    numbersToBeUsed <<
    "\" ";
}

void lpsr2LilypondTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  map<msrDiatonicPitchKind, msrAlterationKind>
    harpPedalsAlterationKindsMap =
      elt->getHarpPedalsAlterationKindsMap ();

  if (harpPedalsAlterationKindsMap.size ()) {
    gIndenter++;

    fLilypondCodeOstream <<
      "_\\markup { \\harp-pedal #\"" <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [kD]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [kC]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [kB]) <<
      "|" <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [kE]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [kF]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [kG]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationKindsMap [kA]) <<
      "\" } " <<
      endl;

    gIndenter--;
  }
  else {
    fLilypondCodeOstream <<
      "%{empty harp pedals tuning???%} "; // JMI
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStem& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrStem& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBeam& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrBeam& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateCodeForOctaveShift (
  S_msrOctaveShift octaveShift)
{
  msrOctaveShift::msrOctaveShiftKind
    octaveShiftKind =
      octaveShift->
        getOctaveShiftKind ();

  int
    octaveShiftSize =
      octaveShift->
        getOctaveShiftSize ();

  switch (octaveShiftKind) {
    case msrOctaveShift::kOctaveShiftNone:
      break;
    case msrOctaveShift::kOctaveShiftUp:
      fLilypondCodeOstream <<
      "\\ottava #" <<
        "-" << (octaveShiftSize - 1) / 7 << // 1 or 2
        ' ';
      break;
    case msrOctaveShift::kOctaveShiftDown:
      fLilypondCodeOstream <<
        "\\ottava #" <<
        (octaveShiftSize - 1) / 7 << // 1 or 2
        ' ';
      break;
    case msrOctaveShift::kOctaveShiftStop:
          fLilypondCodeOstream <<
            "\\ottava #0 ";
      break;
    case msrOctaveShift::kOctaveShiftContinue:
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateCodeAHeadOfChordContents (
  S_msrChord chord)
{
  // generate the chord codas if any
  const list<S_msrCoda>&
    chordCodas =
      chord->getChordCodas ();

  if (chordCodas.size ()) {
    list<S_msrCoda>::const_iterator i;
    for (i=chordCodas.begin (); i!=chordCodas.end (); i++) {
      // generate the coda
      S_msrCoda coda = (*i);

      generateCoda (coda);
    } // for
  }

  // generate the chord segnos if any
  const list<S_msrSegno>&
    chordSegnos =
      chord->getChordSegnos ();

  if (chordSegnos.size ()) {
    list<S_msrSegno>::const_iterator i;
    for (i=chordSegnos.begin (); i!=chordSegnos.end (); i++) {
      // generate the segno
      generateSegno ((*i));
    } // for
  }
}

void lpsr2LilypondTranslator::generateCodeRightBeforeChordContents (
  S_msrChord chord)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    fLilypondCodeOstream <<
      "% --> generateCodeRightBeforeChordContents() for chord " <<
      chord->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      endl;
  }
#endif

/* JMI
  gLogOstream <<
    "% chordGraceNotesGroupBefore = ";
  if (chordGraceNotesGroupBefore) {
    gLogOstream <<
      chordGraceNotesGroupBefore;
  }
  else {
    gLogOstream <<
      "nullptr";
  }
  gLogOstream << endl;
*/

  // print the chord's grace notes before if any,
  S_msrChordGraceNotesGroupLink
    chordGraceNotesGroupLinkBefore =
      chord->getChordGraceNotesGroupLinkBefore ();

  if (chordGraceNotesGroupLinkBefore) {
    generateGraceNotesGroup (
      chordGraceNotesGroupLinkBefore->
        getOriginalGraceNotesGroup ());
  }

  // get the chord glissandos
  const list<S_msrGlissando>&
    chordGlissandos =
      chord->getChordGlissandos ();

  // print the chord glissandos styles if any
  if (chordGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (
      i=chordGlissandos.begin ();
      i!=chordGlissandos.end ();
      i++
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
          break;

        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando style
          switch (glissando->getGlissandoLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeOstream <<
                "\\once\\override Glissando.style = #'dashed-line ";
              break;
            case kLineTypeDotted:
              fLilypondCodeOstream <<
                "\\once\\override Glissando.style = #'dotted-line ";
              break;
            case kLineTypeWavy:
              fLilypondCodeOstream <<
                "\\once\\override Glissando.style = #'zigzag ";
              break;
          } // switch
          break;

        case msrGlissando::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // get the chord slides
  const list<S_msrSlide>&
    chordSlides =
      chord->getChordSlides ();

  // print the chord slides styles if any, implemented as glissandos
  if (chordSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (
      i=chordSlides.begin ();
      i!=chordSlides.end ();
      i++
    ) {
      S_msrSlide slide = (*i);

      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
          break;

        case msrSlide::kSlideTypeStart:
          // generate the glissando style
          switch (slide->getSlideLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeOstream <<
                "\\once\\override Glissando.style = #'dashed-line ";
              break;
            case kLineTypeDotted:
              fLilypondCodeOstream <<
                "\\once\\override Glissando.style = #'dotted-line ";
              break;
            case kLineTypeWavy:
              fLilypondCodeOstream <<
                "\\once\\override Glissando.style = #'zigzag ";
              break;
          } // switch
          break;

        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

  // get the chord ligatures
  list<S_msrLigature>
    chordLigatures =
      chord->getChordLigatures ();

  // print the chord ligatures if any
  if (chordLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin ();
      i!=chordLigatures.end ();
      i++
    ) {

      switch ((*i)->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
        case msrLigature::kLigatureStart:
          fLilypondCodeOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
  // JMI        fLilypondCodeOstream << "\\] ";
          break;
      } // switch
    } // for
  }

  // don't take the chord into account for line breaking ??? JMI

  // get the chord articulations
  list<S_msrArticulation>
    chordArticulations =
      chord->getChordArticulations ();

  // print the chord arpeggios directions if any
  if (chordArticulations.size ()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin ();
      i!=chordArticulations.end ();
      i++
    ) {
      S_msrArticulation articulation = (*i);

      if (
        // arpeggiato?
        S_msrArpeggiato
          arpeggiato =
            dynamic_cast<msrArpeggiato*>(&(*articulation))
        ) {
        msrDirectionKind
          directionKind =
            arpeggiato->getArpeggiatoDirectionKind ();

        switch (directionKind) {
          case kDirectionNone:
            fLilypondCodeOstream <<
              endl <<
              "\\arpeggioNormal";
            break;
          case kDirectionUp:
            fLilypondCodeOstream <<
              endl <<
              "\\arpeggioArrowUp";
            break;
          case kDirectionDown:
            fLilypondCodeOstream <<
              endl <<
              "\\arpeggioArrowDown";
            break;
        } // switch

        fLilypondCodeOstream << ' ';

        fCurrentArpeggioDirectionKind = directionKind;
      }

      else if (
        // non arpeggiato?
        S_msrNonArpeggiato
          nonArpeggiato =
            dynamic_cast<msrNonArpeggiato*>(&(*articulation))
        ) {
        fLilypondCodeOstream <<
          endl <<
          "\\arpeggioBracket";

        switch (nonArpeggiato->getNonArpeggiatoTypeKind ()) {
          case msrNonArpeggiato::kNonArpeggiatoTypeNone:
            fLilypondCodeOstream << " %{\\kNonArpeggiatoTypeNone???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeTop:
            fLilypondCodeOstream << " %{\\kNonArpeggiatoTypeTop???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeBottom:
            fLilypondCodeOstream << " %{\\kNonArpeggiatoTypeBottom???%}";
            break;
        } // switch

        fLilypondCodeOstream << endl;
      }
   } // for
  }

  // should a stem direction command be generated?
  const list<S_msrStem>&
    chordStems =
      chord->getChordStems ();

  if (chordStems.size ()) {
    list<S_msrStem>::const_iterator
      iBegin = chordStems.begin ();

    // only the first stem is used, since they all have the same kind
    S_msrStem firstChordStem = (*iBegin);

    generateNoteStemIfNeeded (firstChordStem);

    fLilypondCodeOstream <<
      ' ';
  }

  // should an octave shift be generated?
  S_msrOctaveShift
    chordOctaveShift =
      chord->getChordOctaveShift ();

  if (chordOctaveShift) {
    generateCodeForOctaveShift (
      chordOctaveShift);
  }
}

void lpsr2LilypondTranslator::generateCodeForChordInGraceNotesGroupContents (
  S_msrChord chord)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    fLilypondCodeOstream <<
      "% --> generateCodeForChordInGraceNotesGroupContents() for chord " <<
      chord->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      endl;
  }
#endif

  // get the chord notes vector
  const vector<S_msrNote>&
    chordNotesVector =
      chord->getChordNotesVector ();

  // generate the chord notes KOF JMI
  if (chordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = chordNotesVector.begin (),
      iEnd   = chordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        chordNote = (*i);

      // print things before the chordNote
      generateCodeRightBeforeNote (chordNote);

      // print the chordNote itself
      generateCodeForNote (chordNote);

      // print things after the chordNote
      generateCodeRightAfterNote (chordNote);

      if (++i == iEnd) break;
      fLilypondCodeOstream <<
        ' ';
    } // for
  }
}

void lpsr2LilypondTranslator::generateCodeRightAfterChordContents (
  S_msrChord chord)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    fLilypondCodeOstream <<
      endl <<
      "% --> generateCodeRightAfterChordContents() for chord " <<
      chord->asShortString () <<
      ", line " << chord->getInputLineNumber () <<
      endl;
  }
#endif

  // get the chord notes vector
  const vector<S_msrNote>&
    chordNotesVector =
      chord->getChordNotesVector ();

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  switch (gLpsr2LilypondOah->fOctaveEntryKind) {
    case kOctaveEntryRelative:
      fCurrentOctaveEntryReference =
        chordNotesVector [0];
      break;
    case kOctaveEntryAbsolute:
      break;
    case kOctaveEntryFixed:
      break;
  } // switch

  // generate the chord duration if relevant
  if (
    chord->getChordIsFirstChordInADoubleTremolo ()
      ||
    chord->getChordIsSecondChordInADoubleTremolo ()) {
      // print chord duration
      fLilypondCodeOstream <<
        chord->getChordSoundingWholeNotes ();
  }

  else {
    int chordInputLineNumber =
      chord->getInputLineNumber ();

    // print the chord duration
    fLilypondCodeOstream <<
      durationAsLilypondString (
        chordInputLineNumber,
        chord->
          getChordDisplayWholeNotes ()); // JMI test wether chord is in a tuplet?
  }

/* JMI
  gLogOstream <<
    "% chordGraceNotesGroupAfter = ";
  if (chordGraceNotesGroupAfter) {
    gLogOstream <<
      chordGraceNotesGroupAfter;
  }
  else {
    gLogOstream <<
      "nullptr";
  }
  gLogOstream << endl;

  if (chordGraceNotesGroupAfter) {
    generateGraceNotesGroup (
      chordGraceNotesGroupAfter);
  }
*/

  // print the chord's grace notes after if any
  S_msrChordGraceNotesGroupLink
    chordGraceNotesGroupLinkAfter =
      chord->getChordGraceNotesGroupLinkAfter ();

  if (chordGraceNotesGroupLinkAfter) {
    generateGraceNotesGroup (
      chordGraceNotesGroupLinkAfter->
        getOriginalGraceNotesGroup ());
  }

  // are there pending chord member notes string numbers?
  if (fPendingChordMemberNotesStringNumbers.size ()) {
    list<int>::const_iterator
      iBegin = fPendingChordMemberNotesStringNumbers.begin (),
      iEnd   = fPendingChordMemberNotesStringNumbers.end (),
      i      = iBegin;

    for ( ; ; ) {
      int stringNumber = (*i);

      fLilypondCodeOstream <<
        "\\" <<
        stringNumber;

      if (++i == iEnd) break;
      fLilypondCodeOstream <<
        ' ';
    } // for
    fLilypondCodeOstream <<
      ' ';

    // forget about the pending string numbers
    fPendingChordMemberNotesStringNumbers.clear ();
  }

  // fetch the chord single tremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      chord->getChordSingleTremolo ();

  if (chordSingleTremolo) {
    // generate code for the chord single tremolo
    fLilypondCodeOstream <<
      singleTremoloDurationAsLilypondString (
        chordSingleTremolo);
  }

  fLilypondCodeOstream <<
    ' ';

  // get the chord articulations
  list<S_msrArticulation>
    chordArticulations =
      chord->getChordArticulations ();

  // print the chord articulations if any
  if (chordArticulations.size ()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin ();
      i!=chordArticulations.end ();
      i++
    ) {
      generateChordArticulation ((*i));

      fLilypondCodeOstream <<
        ' ';
    } // for
  }

  // print the chord technicals if any
  list<S_msrTechnical>
    chordTechnicals =
      chord->getChordTechnicals ();

  if (chordTechnicals.size ()) {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=chordTechnicals.begin ();
      i!=chordTechnicals.end ();
      i++
    ) {
      fLilypondCodeOstream <<
        technicalAsLilypondString ((*i)) <<
        ' '; // JMI
    } // for
  }

  // print the chord technicals with integer if any
  list<S_msrTechnicalWithInteger>
    chordTechnicalWithIntegers =
      chord->getChordTechnicalWithIntegers ();

  if (chordTechnicalWithIntegers.size ()) {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=chordTechnicalWithIntegers.begin ();
      i!=chordTechnicalWithIntegers.end ();
      i++
    ) {
      fLilypondCodeOstream <<
        technicalWithIntegerAsLilypondString ((*i)) <<
        ' '; // JMI
    } // for
  }

  // print the chord technicals with float if any
  list<S_msrTechnicalWithFloat>
    chordTechnicalWithFloats =
      chord->getChordTechnicalWithFloats ();

  if (chordTechnicalWithFloats.size ()) {
    list<S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i=chordTechnicalWithFloats.begin ();
      i!=chordTechnicalWithFloats.end ();
      i++
    ) {
      fLilypondCodeOstream <<
        technicalWithFloatAsLilypondString ((*i)) <<
        ' '; // JMI
    } // for
  }

  // print the chord technicals with string if any
  list<S_msrTechnicalWithString>
    chordTechnicalWithStrings =
      chord->getChordTechnicalWithStrings ();

  if (chordTechnicalWithStrings.size ()) {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=chordTechnicalWithStrings.begin ();
      i!=chordTechnicalWithStrings.end ();
      i++
    ) {
      fLilypondCodeOstream <<
        technicalWithStringAsLilypondString ((*i)) <<
        ' '; // JMI
    } // for
  }

  // print the chord ornaments if any
  list<S_msrOrnament>
    chordOrnaments =
      chord->getChordOrnaments ();

  if (chordOrnaments.size ()) {
    list<S_msrOrnament>::const_iterator i;
    for (
      i=chordOrnaments.begin ();
      i!=chordOrnaments.end ();
      i++
    ) {
      S_msrOrnament
        ornament = (*i);

      switch (ornament->getOrnamentPlacementKind ()) {
        case kPlacementNone:
          fLilypondCodeOstream << "-";
          break;
        case kPlacementAbove:
          fLilypondCodeOstream << "^";
          break;
        case kPlacementBelow:
          fLilypondCodeOstream << "-";
          break;
      } // switch

      generateOrnament (ornament); // some ornaments are not yet supported
    } // for
  }

  // print the chord dynamics if any
  list<S_msrDynamics>
    chordDynamics =
      chord->getChordDynamics ();

  if (chordDynamics.size ()) {
    list<S_msrDynamics>::const_iterator i;
    for (
      i=chordDynamics.begin ();
      i!=chordDynamics.end ();
      i++
    ) {
      S_msrDynamics
        dynamics = (*i);

      switch (dynamics->getDynamicsPlacementKind ()) {
        case kPlacementNone:
          fLilypondCodeOstream << "-";
          break;
        case kPlacementAbove:
          fLilypondCodeOstream << "^";
          break;
        case kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fLilypondCodeOstream <<
        dynamicsAsLilypondString (dynamics) << ' ';
    } // for
  }

  // print the chord other dynamics if any
  list<S_msrOtherDynamics>
    chordOtherDynamics =
      chord->getChordOtherDynamics ();

  if (chordOtherDynamics.size ()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (
      i=chordOtherDynamics.begin ();
      i!=chordOtherDynamics.end ();
      i++
    ) {
      S_msrOtherDynamics
        otherDynamics = (*i);

      switch (otherDynamics->getOtherDynamicsPlacementKind ()) {
        case kPlacementNone:
          fLilypondCodeOstream << "-";
          break;
        case kPlacementAbove:
          fLilypondCodeOstream << "^";
          break;
        case kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fLilypondCodeOstream <<
        "\\otherDynamic \"" <<
        otherDynamics->getOtherDynamicsString () <<
        "\" ";
    } // for
  }

  // print the chord words if any
  list<S_msrWords>
    chordWords =
      chord->getChordWords ();

  if (chordWords.size ()) {
    list<S_msrWords>::const_iterator i;
    for (
      i=chordWords.begin ();
      i!=chordWords.end ();
      i++
    ) {

      msrPlacementKind
        wordsPlacementKind =
          (*i)->getWordsPlacementKind ();

      string wordsContents =
        (*i)->getWordsContents ();

      switch (wordsPlacementKind) {
        case kPlacementNone:
          // should not occur
          break;
        case kPlacementAbove:
          fLilypondCodeOstream << "^";
          break;
        case kPlacementBelow:
          fLilypondCodeOstream << "_";
          break;
      } // switch

      fLilypondCodeOstream <<
        "\\markup" << " { " <<
        doubleQuoteStringIfNonAlpha (wordsContents) <<
        " } ";
    } // for
  }

  // print the chord beams if any
  list<S_msrBeam>
    chordBeams =
      chord->getChordBeams ();

  if (chordBeams.size ()) {
    list<S_msrBeam>::const_iterator i;
    for (
      i=chordBeams.begin ();
      i!=chordBeams.end ();
      i++
    ) {

      S_msrBeam beam = (*i);

      // LilyPond will take care of multiple beams automatically,
      // so we need only generate code for the first number (level)
      switch (beam->getBeamKind ()) {

        case msrBeam::kBeginBeam:
          if (beam->getBeamNumber () == 1)
            fLilypondCodeOstream << "[ ";
          break;

        case msrBeam::kContinueBeam:
          break;

        case msrBeam::kEndBeam:
          if (beam->getBeamNumber () == 1)
            fLilypondCodeOstream << "] ";
          break;

        case msrBeam::kForwardHookBeam:
          break;

        case msrBeam::kBackwardHookBeam:
          break;

        case msrBeam::k_NoBeam:
          break;
      } // switch
    } // for
  }

/*
  // print the chord slurs if any
  list<S_msrSlur>
    chordSlurs =
      chord->getChordSlurs ();

  if (false && chordSlurs.size ()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=chordSlurs.begin ();
      i!=chordSlurs.end ();
      i++
    ) {

      switch ((*i)->getSlurTypeKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kRegularSlurStart:
          fLilypondCodeOstream << "( ";
          break;
        case msrSlur::kPhrasingSlurStart:
          fLilypondCodeOstream << "\\( ";
          break;
        case msrSlur::kSlurContinue:
          break;
        case msrSlur::kRegularSlurStop:
          fLilypondCodeOstream << ") ";
          break;
        case msrSlur::kPhrasingSlurStop:
          fLilypondCodeOstream << "\\) ";
          break;
      } // switch
    } // for
  }
*/

  // print the chord slur links if any
  list<S_msrChordSlurLink>
    chordSlurLinks =
      chord->getChordSlurLinks ();

  if (chordSlurLinks.size ()) {
    list<S_msrChordSlurLink>::const_iterator i;
    for (
      i=chordSlurLinks.begin ();
      i!=chordSlurLinks.end ();
      i++
    ) {
      S_msrChordSlurLink ChordSlurLink = (*i);

      S_msrSlur originalSlur = ChordSlurLink->getOriginalSlur ();

      switch (originalSlur->getSlurTypeKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kRegularSlurStart:
          fLilypondCodeOstream << "( ";
          break;
        case msrSlur::kPhrasingSlurStart:
          fLilypondCodeOstream << "\\( ";
          break;
        case msrSlur::kSlurContinue:
          break;
        case msrSlur::kRegularSlurStop:
          fLilypondCodeOstream << ") ";
          break;
        case msrSlur::kPhrasingSlurStop:
          fLilypondCodeOstream << "\\) ";
          break;
      } // switch
    } // for
  }

/* Don't print the chord ties, rely only on its notes's ties // JMI
  // thus using LilyPond's partially tied chords // JMI
  // print the chord ties if any
  list<S_msrTie>
    chordTies =
      chord->getChordTies ();

  if (chordTies.size ()) {
    list<S_msrTie>::const_iterator i;
    for (
      i=chordTies.begin ();
      i!=chordTies.end ();
      i++
    ) {
      fLilypondCodeOstream <<
        "%{line " << inputLineNumber << "%}" <<
        "~ %{S_msrChord}"; // JMI spaces???
    } // for
  }
*/

/* JMI
  // print the tie if any
  {
    S_msrTie chordTie = chord->getChordTie ();

    if (chordTie) {
      if (chordTie->getTieKind () == msrTie::kTieStart) {
        fLilypondCodeOstream <<
          "%{line " << inputLineNumber << "%}" <<
          "~ ";
      }
    }
  }
*/

  // get the chord ligatures
  list<S_msrLigature>
    chordLigatures =
      chord->getChordLigatures ();

  // print the chord ligatures if any
  if (chordLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin ();
      i!=chordLigatures.end ();
      i++
    ) {
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
        case msrLigature::kLigatureStart:
          fLilypondCodeOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
          fLilypondCodeOstream << "\\] ";
          break;
      } // switch
    } // for
  }

  // print the chord wedges if any
  list<S_msrWedge>
    chordWedges =
      chord->getChordWedges ();

  if (chordWedges.size ()) {
    list<S_msrWedge>::const_iterator i;
    for (
      i=chordWedges.begin ();
      i!=chordWedges.end ();
      i++
      ) {
      switch ((*i)->getWedgeKind ()) {
        case msrWedge::kWedgeKindNone:
          break;
        case msrWedge::kWedgeCrescendo:
          fLilypondCodeOstream << "\\< ";
          break;
        case msrWedge::kWedgeDecrescendo:
          fLilypondCodeOstream << "\\> ";
          break;
        case msrWedge::kWedgeStop:
          fLilypondCodeOstream << "\\! ";
          break;
      } // switch
    } // for
  }

  // get the chord glissandos
  const list<S_msrGlissando>&
    chordGlissandos =
      chord->getChordGlissandos ();

  // print the chord glissandos if any
  if (chordGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (
      i=chordGlissandos.begin ();
      i!=chordGlissandos.end ();
      i++
    ) {
      S_msrGlissando glissando = (*i);

      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
          break;

        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando itself
          fLilypondCodeOstream <<
            "\\glissando ";
          break;

        case msrGlissando::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // get the chord slides
  const list<S_msrSlide>&
    chordSlides =
      chord->getChordSlides ();

  // print the chord slides if any, implemented as glissandos
  if (chordSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (
      i=chordSlides.begin ();
      i!=chordSlides.end ();
      i++
    ) {
      S_msrSlide slide = (*i);

      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
          break;

        case msrSlide::kSlideTypeStart:
          // generate the glissando itself
          fLilypondCodeOstream <<
            "\\glissando ";
          break;

        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

  // generate the chord dal segnos if any
  const list<S_msrDalSegno>&
    chordDalSegnos =
      chord->getChordDalSegnos ();

  if (chordDalSegnos.size ()) {
    list<S_msrDalSegno>::const_iterator i;
    for (i=chordDalSegnos.begin (); i!=chordDalSegnos.end (); i++) {
      // generate the dal segno
      fLilypondCodeOstream <<
        endl <<
        "\\override Score.RehearsalMark.break-visibility = #begin-of-line-invisible" <<
        endl <<
        "\\tweak self-alignment-X #RIGHT" <<
        endl <<
        "\\mark \\markup { " <<
        (*i)->getDalSegnoString () <<
        " }" <<
        endl;
    } // for
  }
}

void lpsr2LilypondTranslator::generateChordInGraceNotesGroup (S_msrChord chord)
{
  generateCodeRightBeforeChordContents (chord);

  // generate the start of the chord
  fLilypondCodeOstream <<
    "<";

  generateCodeForChordInGraceNotesGroupContents (chord);

  // generate the end of the chord
  fLilypondCodeOstream <<
    ">";

  generateCodeRightAfterChordContents (chord);
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrChord& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrChord " <<
      elt->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (fOnGoingGraceNotesGroup) {
    int chordInputLineNumber =
      elt->getInputLineNumber ();

    stringstream s;

    s <<
      "% ==> Start visiting grace chords is ignored inside grace notes groups " <<
      elt->asShortString ();

    msrInternalWarning (
      gOahOah->fInputSourceName,
      chordInputLineNumber,
      s.str ());

    return;
  }
#endif

  fOnGoingChord = true;
  fCurrentChord = elt;

  generateCodeAHeadOfChordContents (elt);

  generateCodeRightBeforeChordContents (elt);

  // generate the start of the chord
  fLilypondCodeOstream <<
    "<";
}

void lpsr2LilypondTranslator::visitEnd (S_msrChord& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrChord " <<
      elt->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (fOnGoingGraceNotesGroup) {
    msrInternalWarning (
      gOahOah->fInputSourceName,
      inputLineNumber,
      "% ==> End visiting grace chords is ignored inside grace notes groups");

    return;
  }
#endif

  // generate the end of the chord
  fLilypondCodeOstream <<
    ">";

  generateCodeRightAfterChordContents (elt);

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  switch (gLpsr2LilypondOah->fOctaveEntryKind) {
    case kOctaveEntryRelative:
      fCurrentOctaveEntryReference =
        elt->getChordNotesVector () [0];
      break;
    case kOctaveEntryAbsolute:
      break;
    case kOctaveEntryFixed:
      break;
  } // switch

  fOnGoingChord = false;
  fCurrentChord = nullptr;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTuplet& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTuplet" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (fTupletsStack.size ()) {
    // elt is a nested tuplet

    S_msrTuplet
      containingTuplet =
        fTupletsStack.top ();

    // unapply containing tuplet factor,
    // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
    elt->
      unapplySoundingFactorToTupletMembers (
        containingTuplet->
          getTupletFactor ());
  }

  if (gLpsr2LilypondOah->fIndentTuplets) {
    fLilypondCodeOstream << endl;
  }

  // get bracket kind
  msrTuplet::msrTupletBracketKind
    tupletBracketKind =
      elt->getTupletBracketKind ();

  switch (tupletBracketKind) {
    case msrTuplet::kTupletBracketYes:
    /* JMI
      fLilypondCodeOstream <<
        "%{kTupletBracketYes%}" <<
        endl;
        */
      break;
    case msrTuplet::kTupletBracketNo:
      fLilypondCodeOstream <<
        "\\once \\omit TupletBracket" <<
        endl;
      break;
  } // switch

  // get line shape kind
  msrTuplet::msrTupletLineShapeKind
    tupletLineShapeKind =
      elt->getTupletLineShapeKind ();

  switch (tupletLineShapeKind) {
    case msrTuplet::kTupletLineShapeStraight:
      break;
    case msrTuplet::kTupletLineShapeCurved:
      fLilypondCodeOstream <<
        "\\temporary \\tupletsCurvedBrackets" <<
        endl;
      break;
  } // switch

  // get show number kind
  msrTuplet::msrTupletShowNumberKind
    tupletShowNumberKind =
      elt->getTupletShowNumberKind ();

  switch (tupletShowNumberKind) {
    case msrTuplet::kTupletShowNumberActual:
    /* JMI
      fLilypondCodeOstream <<
        "%{tupletShowNumberActual%}" <<
        endl;
        */
      break;
    case msrTuplet::kTupletShowNumberBoth:
      fLilypondCodeOstream <<
        "\\once \\override TupletNumber.text = #tuplet-number::calc-fraction-text" <<
        endl;
      break;
    case msrTuplet::kTupletShowNumberNone:
      fLilypondCodeOstream <<
        "\\once \\omit TupletNumber" <<
        endl;
      break;
  } // switch

  // get show type kind
  msrTuplet::msrTupletShowTypeKind
    tupletShowTypeKind =
      elt->getTupletShowTypeKind ();

  rational
    memberNoteDisplayWholeNotes =
      elt->getMemberNotesDisplayWholeNotes ();

  switch (tupletShowTypeKind) {
    case msrTuplet::kTupletShowTypeActual:
      fLilypondCodeOstream <<
     // JMI ???   "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-fraction-text \"" <<
        "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-denominator-text \"" <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        "\")" <<
        endl;
      break;
    case msrTuplet::kTupletShowTypeBoth:
      fLilypondCodeOstream <<
        "\\once \\override TupletNumber.text = #(tuplet-number::fraction-with-notes" <<
        " #{ " <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        " #} #{ " <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        " #})" <<
        endl;
      break;
    case msrTuplet::kTupletShowTypeNone:
    /* JMI
      fLilypondCodeOstream <<
        "%{tupletShowTypeNone%}" <<
        endl;
        */
      break;
  } // switch

  fLilypondCodeOstream <<
    "\\tuplet " <<
    elt->getTupletFactor ().asRational () <<
    " {" <<
    endl;

  fTupletsStack.push (elt);

  gIndenter++;

  // force durations to be displayed explicitly
  // at the beginning of the tuplet
  fLastMetWholeNotes = rational (0, 1);
}

void lpsr2LilypondTranslator::visitEnd (S_msrTuplet& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  if (gLpsr2LilypondOah->fIndentTuplets) {
    fLilypondCodeOstream << endl;
  }

  fLilypondCodeOstream <<
    "}" <<
    endl;

  // get line shape kind
  msrTuplet::msrTupletLineShapeKind
    tupletLineShapeKind =
      elt->getTupletLineShapeKind ();

  switch (tupletLineShapeKind) {
    case msrTuplet::kTupletLineShapeStraight:
      break;
    case msrTuplet::kTupletLineShapeCurved:
      fLilypondCodeOstream <<
        "\\undo \\tupletsCurvedBrackets" <<
        endl;
      break;
  } // switch

  fTupletsStack.pop ();

/* JMI
 ?????? fCurrentOctaveEntryReference = nullptr;
  // the first note after the tuplet will become the new reference
  */
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTie& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrTie" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  switch (elt->getTieKind ()) {
    case msrTie::kTieNone:
      break;
    case msrTie::kTieStart:
      if (fOnGoingNotesStack.size () > 0) {
        // this precludes generating for the chords' ties,
        // since the last of its notes sets fOnGoingNotesStack.size > 0 to false
        // after code has been generated for it
        fLilypondCodeOstream <<
  // JMI        "%{line " << inputLineNumber << "%}" <<
          " ~ ";
      }
      break;
    case msrTie::kTieContinue:
      break;
    case msrTie::kTieStop:
      break;
   } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrTie& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSegno& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitStart (S_msrHiddenMeasureAndBarline& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrHiddenMeasureAndBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\HiddenMeasureAndBarLine " <<
    "\\time " <<
    "4/4 ";
}

void lpsr2LilypondTranslator::visitStart (S_msrCoda& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrEyeGlasses& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "^\\markup {\\eyeglasses} ";
}

void lpsr2LilypondTranslator::visitStart (S_msrScordatura& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

/* JMI
  const list<S_msrStringTuning>&
    scordaturaStringTuningsList =
      elt->getScordaturaStringTuningsList ();

  gIndenter++;

  fLilypondCodeOstream <<
    "\\new Staff {" <<
    endl;

  gIndenter++;

  fLilypondCodeOstream <<
    "\\hide Staff.Stem" <<
    endl <<
    "\\hide Staff.TimeSignature" <<
    endl <<
    "\\small" <<
    endl <<
    "<";

  if (scordaturaStringTuningsList.size ()) {
    list<S_msrStringTuning>::const_iterator
      iBegin = scordaturaStringTuningsList.begin (),
      iEnd   = scordaturaStringTuningsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrStringTuning
        stringTuning = (*i);

      fLilypondCodeOstream <<
        stringTuningAsLilypondString (
          elt->getInputLineNumber (),
          stringTuning);

      if (++i == iEnd) break;

      fLilypondCodeOstream << ' ';
    } // for
  }

  fLilypondCodeOstream <<
    ">4" <<
    endl <<
    "}" <<
    endl;

  gIndenter--;

  fLilypondCodeOstream <<
    "{ c'4 }" <<
    endl <<

  gIndenter--;
  */
}

void lpsr2LilypondTranslator::visitStart (S_msrPedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();

#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting pedal" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  fLilypondCodeOstream << endl;

  switch (elt->getPedalTypeKind ()) {
    case msrPedal::kPedalStart:
      fLilypondCodeOstream <<
        "\\sustainOn";
      break;
    case msrPedal::kPedalContinue:
      fLilypondCodeOstream <<
        "\\sustainOff\\sustainOn"; // JMI
      break;
    case msrPedal::kPedalChange:
      fLilypondCodeOstream <<
        "\\sustainOff\\sustainOn";
      break;
    case msrPedal::kPedalStop:
      fLilypondCodeOstream <<
        "\\sustainOff";
      break;
    case msrPedal::k_NoPedalType:
      {
        // should not occur

        stringstream s;

        s <<
          "msrPedal '" <<
          elt->asShortString () <<
          "' has no pedal type";

        msrInternalError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  fLilypondCodeOstream << endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrDamp& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "^\\markup {\\damp} ";
}

void lpsr2LilypondTranslator::visitStart (S_msrDampAll& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "^\\markup {\\dampAll} ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarline& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      endl <<
      "% --> Start visiting msrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  switch (elt->getBarlineCategory ()) {
    case msrBarline::kBarlineCategoryStandalone:
      switch (elt->getBarlineStyleKind ()) {
        case msrBarline::kBarlineStyleNone:
          break;
        case msrBarline::kBarlineStyleRegular:
          fLilypondCodeOstream << "\\bar \"|\" ";
          break;
        case msrBarline::kBarlineStyleDotted:
          fLilypondCodeOstream << "\\bar \";\" ";
          break;
        case msrBarline::kBarlineStyleDashed:
          fLilypondCodeOstream << "\\bar \"!\" ";
          break;
        case msrBarline::kBarlineStyleHeavy:
          fLilypondCodeOstream << "\\bar \".\" ";
          break;
        case msrBarline::kBarlineStyleLightLight:
          fLilypondCodeOstream << "\\bar \"||\" ";
          break;
        case msrBarline::kBarlineStyleLightHeavy:
          fLilypondCodeOstream <<
            endl <<
            "\\bar \"|.\" ";
          break;
        case msrBarline::kBarlineStyleHeavyLight:
          fLilypondCodeOstream << "\\bar \".|\" ";
          break;
        case msrBarline::kBarlineStyleHeavyHeavy:
          fLilypondCodeOstream << "\\bar \"..\" ";
          break;
        case msrBarline::kBarlineStyleTick:
          fLilypondCodeOstream << "\\bar \"'\" ";
          break;
        case msrBarline::kBarlineStyleShort:
          // \bar "/" is the custom short barline
          fLilypondCodeOstream << "\\bar \"/\" ";
          break;
          /* JMI
        case msrBarline::kBarlineStyleNone:
          fLilypondCodeOstream << "\\bar \"\" ";
          break;
          */
      } // switch

      if (gLpsr2LilypondOah->fInputLineNumbers) {
        // print the barline line number as a comment
        fLilypondCodeOstream <<
          "%{ " << inputLineNumber << " %} ";
      }

      fLilypondCodeOstream << endl;

/* JMI BOF ???
      switch (elt->getBarlineHasSegnoKind ()) {
        case msrBarline::kBarlineHasSegnoYes:
          // generate the segno
          generateSegno ((*i));
          break;
        case msrBarline::kBarlineHasSegnoNo:
          break;
      } // switch

      switch (elt->getBarlineHasCodaKind ()) {
        case msrBarline::kBarlineHasCodaYes:
          fLilypondCodeOstream <<
            "\\mark \\markup { \\musicglyph #\"scripts.coda\" } ";
          if (gLpsr2LilypondOah->fInputLineNumbers) {
            // print the input line number as a comment
            fLilypondCodeOstream <<
              "%{line " << note->getInputLineNumber () << "%} ";
          }
          fLilypondCodeOstream << endl;
          break;
        case msrBarline::kBarlineHasCodaNo:
          break;
      } // switch
*/
      break;

    case msrBarline::kBarlineCategoryRepeatStart:
      if (gLpsr2LilypondOah->fKeepRepeatBarlines) {
      /*
        if (gLpsr2LilypondOah->fRepeatBrackets) {
          if (fRepeatDescrsStack.size ()) {
            S_msrRepeat
              currentRepeat =
                fRepeatDescrsStack.back ()->getRepeat (),
              precedingRepeat =
                currentRepeat->getImmediatelyPrecedingRepeat ();

            if (precedingRepeat && ! precedingRepeat-> getRepeatEndings ().size ()) {
              // JMI
            }
          }
          else {
            // JMI
          }
          fLilypondCodeOstream << "\\bar \"[|:\" ";
        }
      */
        fLilypondCodeOstream << "\\bar \".|:\" ";
      }
      else {
        // JMI
      }
      break;

    case msrBarline::kBarlineCategoryRepeatEnd:
      if (gLpsr2LilypondOah->fKeepRepeatBarlines) {
        if (gLpsr2LilypondOah->fRepeatBrackets) {
          if (fRepeatDescrsStack.size ()) {
            S_msrRepeat
              currentRepeat =
                fRepeatDescrsStack.back ()->getRepeat (),
              followingRepeat =
                currentRepeat->getImmediatelyPrecedingRepeat ();

      // JMI      if (followingRepeat && ! followingRepeat-> getRepeatEndings ().size ()) {
            if (followingRepeat) {
              fLilypondCodeOstream << "\\bar \":|][|:\" ";
            }
          }
          else {
            fLilypondCodeOstream << "\\bar \":|.\" ";
          }
        }
        else {
          fLilypondCodeOstream << "\\bar \":|.\" ";
        }
      }
      else {
        // JMI
      }
      break;

    case msrBarline::kBarlineCategoryHookedEndingStart:
    case msrBarline::kBarlineCategoryHookedEndingEnd:
    case msrBarline::kBarlineCategoryHooklessEndingStart:
    case msrBarline::kBarlineCategoryHooklessEndingEnd:
      // should not occur, since
      // LilyPond will take care of displaying the repeat
      break;

    case msrBarline::k_NoBarlineCategory:
      {
        stringstream s;

        s <<
          "barline category has not been set" <<
          ", line " << elt->getInputLineNumber ();

  // JMI      msrInternalError (
        msrInternalWarning (
          gOahOah->fInputSourceName,
          inputLineNumber,
  // JMI        __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  if (gLpsr2LilypondOah->fInputLineNumbers) {
    fLilypondCodeOstream <<
      " %{ " << inputLineNumber << " %}";
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarline& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      endl <<
      "% --> End visiting msrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarCheck& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrBarCheck" <<
      ", nextBarNumber: " <<
      elt->getNextBarPuristNumber () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  int nextBarPuristNumber =
    elt->getNextBarPuristNumber ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    fLilypondCodeOstream <<
      "% nextBarPuristNumber = " <<
      nextBarPuristNumber <<
      ", fOnGoingRestMeasures = " <<
      "fOnGoingVoiceCadenza = " <<
      booleanAsString (fOnGoingVoiceCadenza) <<
      ", nextBarPuristNumber = " <<
      booleanAsString (fOnGoingRestMeasures) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (
    ! fOnGoingVoiceCadenza
      // should be tested in msr2LpsrTranslator.cpp JMI visitEnd (S_msrMeasure&)
      // MusicXML bar numbers cannot be relied upon for a LilyPond bar number check
    &&
    ! fOnGoingRestMeasures
  ) {
    // don't generate a bar check before the end of measure 1 // JMI ???
    fLilypondCodeOstream <<
      "| % " << nextBarPuristNumber;

    if (gLpsr2LilypondOah->fOriginalMeasureNumbers) {
      // print the original MusicXML measure number as a comment
      fLilypondCodeOstream <<
        " (omn: " << elt->getNextBarOriginalNumber () << ")";
    }

    fLilypondCodeOstream <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarCheck& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarNumberCheck& elt)
{
#ifdef TRACE_OAH
  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrBarNumberCheck" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    fLilypondCodeOstream <<
      "% fOnGoingVoiceCadenza = " <<
      booleanAsString (fOnGoingVoiceCadenza) <<
      ", fOnGoingRestMeasures = " <<
      booleanAsString (fOnGoingRestMeasures) <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  if (
    ! fOnGoingVoiceCadenza
      // should be tested in msr2LpsrTranslator.cpp JMI visitEnd (S_msrMeasure&)
     // MusicXML bar numbers cannot be relied upon for a LilyPond bar number check
    &&
    ! fOnGoingRestMeasures
  ) {
    int nextBarPuristNumber =
      elt->getNextBarPuristNumber ();

    fLilypondCodeOstream <<
      "\\barNumberCheck #" <<
      nextBarPuristNumber;

    if (gLpsr2LilypondOah->fOriginalMeasureNumbers) {
      fLilypondCodeOstream <<
        " %{omn: " <<
        elt->getNextBarOriginalNumber () <<
        "%}";
    }

    fLilypondCodeOstream <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrLineBreak& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\myBreak | % " << elt->getNextBarNumber ();

    if (gLpsr2LilypondOah->fOriginalMeasureNumbers) {
      // print the original MusicXML measure number as a comment
      fLilypondCodeOstream <<
        " (omn: " << elt->getNextBarNumber () << ")";
    }

  fLilypondCodeOstream <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrLineBreak& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPageBreak& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "\\myPageBreak " <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrPageBreak& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeat& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  int repeatEndingsNumber =
    elt->getRepeatEndings ().size ();

  if (repeatEndingsNumber == 0)
    repeatEndingsNumber = 2; // implicitly JMI ???

  fRepeatDescrsStack.push_back (
    lpsrRepeatDescr::create (
      elt,
      repeatEndingsNumber));

  int
    repeatTimes =
      elt->getRepeatTimes ();

  stringstream s;

  fLilypondCodeOstream << endl;

  s <<
    "\\repeat volta " <<
    repeatTimes <<
// JMI    fRepeatDescrsStack.back ()->getRepeatEndingsNumber () <<
    " {";

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      s.str () << "% start of repeat";
  }
  else {
    fLilypondCodeOstream <<
      s.str ();
  }

  fLilypondCodeOstream << endl;

  gIndenter++;

  if (repeatTimes > 2) {
    fLilypondCodeOstream <<
      "<>^\"" << repeatTimes << " times\"" << // JMI
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeat& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  /*
    CAUTION:
      the end of the repeat has to be generated
      before the endings are handled
  */

  if (! fRepeatDescrsStack.back ()->getEndOfRepeatHasBeenGenerated ()) {
    // the end of the repeat has not been generated yet JMI

    gIndenter--;

    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fLilypondCodeOstream <<
        endl <<
        "}" <<
        endl <<
        endl;
    }
  }

  fRepeatDescrsStack.pop_back ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrRepeatCommonPart" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrRepeatCommonPart" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeatEnding& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fRepeatDescrsStack.back ()->
    incrementRepeatEndingsCounter ();

  int
    repeatEndingInternalNumber =
      elt->getRepeatEndingInternalNumber ();

  if (repeatEndingInternalNumber == 1) {
    gIndenter--;

    // first repeat ending is in charge of
    // outputting the end of the repeat
    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream <<
        setw (commentFieldWidth) << left <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fLilypondCodeOstream <<
        endl <<
        "}" <<
        endl;
    }
    fRepeatDescrsStack.back ()->
      setEndOfRepeatHasBeenGenerated ();

    // first repeat ending is in charge of
    // outputting the start of the alternative
    if (gLpsr2LilypondOah->fLilyPondComments) {
      fLilypondCodeOstream << left <<
        endl <<
        setw (commentFieldWidth) <<
        "\\alternative {" <<
        "% start of alternative" <<
        endl;
    }
    else {
      fLilypondCodeOstream <<
        endl <<
        "\\alternative {" <<
        endl;
    }

    gIndenter++;
  }

  // output the start of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gLpsr2LilypondOah->fLilyPondComments) {
        fLilypondCodeOstream << left <<
          setw (commentFieldWidth) <<
          "{" << "% start of repeat hooked ending" <<
          endl;
      }
      else {
        fLilypondCodeOstream <<
          "{" <<
          endl;
      }
      break;

    case msrRepeatEnding::kHooklessEnding:
      if (gLpsr2LilypondOah->fLilyPondComments) {
        fLilypondCodeOstream << left <<
          setw (commentFieldWidth) <<
          "{" << "% start of repeat hookless ending" <<
          endl;
      }
      else {
        fLilypondCodeOstream <<
          "{" <<
          endl;
      }
      break;
  } // switch

  gIndenter++;

  // generate the repeat ending number if any
  string
    repeatEndingNumber =
      elt->getRepeatEndingNumber ();

  if (repeatEndingNumber.size ()) {
  /* only if numbers differ JMI ???
    if (repeatEndingInternalNumber == 1) {
      fLilypondCodeOstream <<
        "\\set Score.repeatCommands = #'((volta \"" <<
        repeatEndingNumber <<
        "\"))" <<
        endl;
    }
    else {
      fLilypondCodeOstream <<
        "\\set Score.repeatCommands = #'(end-repeat (volta \"" <<
        repeatEndingNumber <<
        "\"))" <<
        endl;
    }
    */
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeatEnding& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  // output the end of the ending
  fLilypondCodeOstream << endl;

  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gLpsr2LilypondOah->fLilyPondComments) {
        fLilypondCodeOstream << left <<
          setw (commentFieldWidth) <<
          "}" << "% end of repeat hooked ending" <<
          endl;
      }
      else {
        fLilypondCodeOstream <<
          "}" <<
          endl;
      }
      break;

    case msrRepeatEnding::kHooklessEnding:
      if (gLpsr2LilypondOah->fLilyPondComments)   {
        fLilypondCodeOstream << left <<
          setw (commentFieldWidth) <<
          "}" << "% end of repeat hookless ending" <<
          endl;
      }
      else {
        fLilypondCodeOstream <<
          "}" <<
          endl;
      }
      break;
  } // switch

/* JMI
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    fLilypondCodeOstream <<
      "% ===**** fRepeatDescrsStack.back () = '" <<
      fRepeatDescrsStack.back ()->repeatDescrAsString () <<
      "'" <<
      endl;
  }
#endif
*/

  if (
    fRepeatDescrsStack.back ()->getRepeatEndingsCounter ()
      ==
    fRepeatDescrsStack.back ()->getRepeatEndingsNumber ()) {

    gIndenter--; // JMI ???

    // last repeat ending is in charge of
    // outputting the end of the alternative
    if (gLpsr2LilypondOah->fLilyPondComments)   {
      fLilypondCodeOstream << left <<
        setw (commentFieldWidth) <<
        "}" << "% end of alternative" <<
        endl;
    }
    else {
      fLilypondCodeOstream <<
        "}" <<
        endl <<
        endl;
    }
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrComment& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrComment" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    "% " << elt->getContents () <<
    endl;

  if (elt->getCommentGapKind () == lpsrComment::kGapAfterwards)
    fLilypondCodeOstream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrComment& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrComment" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrSchemeFunction& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting lpsrSchemeFunction" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream <<
    endl <<
    "% Scheme function(s): \"" << elt->getFunctionName () << "\"" <<
    // endl is in the decription
    elt->getFunctionDescription () <<
    // endl is in the decription
    elt->getFunctionCode () <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrSchemeFunction& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting lpsrSchemeFunction" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRehearsal& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  fLilypondCodeOstream << endl;

  switch (elt->getRehearsalPlacementKind ()) {
    case msrPlacementKind::kPlacementNone:
      break;
    case msrPlacementKind::kPlacementAbove:
      break;
    case msrPlacementKind::kPlacementBelow:
      fLilypondCodeOstream <<
        "\\once\\override Score.RehearsalMark.direction = #DOWN";
      break;
    } // switch

  fLilypondCodeOstream <<
    endl <<
    "\\mark\\markup { ";

  switch (elt->getRehearsalKind ()) {
    case msrRehearsal::kNone:
      fLilypondCodeOstream <<
        "\\box"; // default value
      break;
    case msrRehearsal::kRectangle:
      fLilypondCodeOstream <<
        "\\box";
      break;
    case msrRehearsal::kOval:
      fLilypondCodeOstream <<
        "\\oval";
      break;
    case msrRehearsal::kCircle:
      fLilypondCodeOstream <<
        "\\circle";
      break;
    case msrRehearsal::kBracket:
      fLilypondCodeOstream <<
        "\\bracket";
      break;
    case msrRehearsal::kTriangle:
      fLilypondCodeOstream <<
        "%{\\triangle???%}";
      break;
    case msrRehearsal::kDiamond:
      fLilypondCodeOstream <<
        "%{\\diamond???";
      break;
  } // switch

  fLilypondCodeOstream <<
    " { \""<<
    elt->getRehearsalText () <<
    "\"" "}}" <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrRehearsal& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMeasuresRepeat& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrMeasuresRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  int repeatReplicasMeasuresNumber =
    elt->fetchMeasuresRepeatReplicasMeasuresNumber ();
#endif

  int replicasNumber =
    elt->fetchMeasuresRepeatReplicasNumber ();

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    int repeatPatternMeasuresNumber =
      elt->fetchMeasuresRepeatPatternMeasuresNumber ();

    fLilypondCodeOstream <<
      "% measures repeat, line " << elt->getInputLineNumber () << ":" <<
      endl;

    const int fieldWidth = 24;

    fLilypondCodeOstream << left <<
      setw (fieldWidth) <<
      "% repeatPatternMeasuresNumber" << " = " << repeatPatternMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% repeatReplicasMeasuresNumber" << " = " << repeatReplicasMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% replicasNumber" << " = " << replicasNumber <<
      endl;
  }
#endif

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "% start of measures repeat" <<
      singularOrPlural (
        elt->fetchMeasuresRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fLilypondCodeOstream <<
    endl <<
    endl <<
    "\\repeat percent " <<
    replicasNumber + 1 <<
     " { " <<
    endl;

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasuresRepeat& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrMeasuresRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  gIndenter--;

  fLilypondCodeOstream <<
    endl <<
    endl <<
    " }" <<
    endl;

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream <<
      setw (commentFieldWidth) << left <<
      "% end of measures repeat" <<
      singularOrPlural (
        elt->fetchMeasuresRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      endl <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitStart (S_msrMeasuresRepeatPattern& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> Start visiting msrMeasuresRepeatPattern" <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasuresRepeatPattern& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> End visiting msrMeasuresRepeatPattern" <<
      endl;
  }
#endif
}

void lpsr2LilypondTranslator::visitStart (S_msrMeasuresRepeatReplicas& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> Start visiting msrMeasuresRepeatReplicas" <<
      endl;
  }
#endif

  // output the start of the ending
  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "{" << "% start of measures repeat replicas" <<
      endl;
  }
  else {
    fLilypondCodeOstream <<
      endl <<
      "{" <<
      endl;
  }

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> End visiting msrMeasuresRepeatReplicas" <<
      endl;
  }
#endif

  gIndenter--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRestMeasures& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrRestMeasures" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  int restMeasuresNumber =
    elt->getRestMeasuresNumber ();

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "% start of rest measures" <<
      singularOrPlural (
        restMeasuresNumber,
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      endl <<
      endl;

    gIndenter++;
  }

  fOnGoingRestMeasures = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrRestMeasures& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrRestMeasures" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  int restMeasuresNumber =
    elt->getRestMeasuresNumber ();

  // start counting measures
  fRemainingRestMeasuresNumber =
    elt->getRestMeasuresNumber ();

  // get rest measures sounding notes
  rational
    restMeasuresMeasureSoundingNotes =
      elt->getRestMeasuresMeasureSoundingNotes ();

  // generate rest measures compression if relevant
  // right befoe Ri*n, because if affects only the next music element
  if (
    fCurrentVoice->getVoiceContainsRestMeasures ()
      ||
    gLpsr2LilypondOah->fCompressFullMeasureRests
  ) {
    fLilypondCodeOstream <<
      "\\compressFullBarRests" << // JMI
      endl;
  }

  // generate rest measures only now, in case there are
  // clef, keys or times before them in the first measure
  fLilypondCodeOstream <<
    "R%{8%}" <<
    restMeasuresWholeNoteAsLilypondString (
      inputLineNumber,
      restMeasuresMeasureSoundingNotes);

  if (restMeasuresNumber > 1) {
    fLilypondCodeOstream <<
      "*" <<
      restMeasuresNumber;
  }

  if (gLpsr2LilypondOah->fInputLineNumbers) {
    // print the rest measures line number as a comment
    fLilypondCodeOstream <<
      " %{ " << inputLineNumber << " %} ";
  }

  // wait until all measures have be visited
  // before the bar check is generated // JMI ???

  // now we can generate the bar check
  fLilypondCodeOstream <<
    " | % " <<
    elt->getRestMeasuresLastMeasurePuristMeasureNumber () + 1;

/* TO BE FINALIZED JMI
    if (gLpsr2LilypondOah->fOriginalMeasureNumbers) {
      // print the original MusicXML measure number as a comment
      fLilypondCodeOstream <<
        " (mxml3: " << measureElement->getInputLineNumber () << ")";
    }
*/

  fLilypondCodeOstream <<
    endl;

  if (gLpsr2LilypondOah->fLilyPondComments) {
    gIndenter--;

    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "% end of rest measures" <<
      singularOrPlural (
        restMeasuresNumber,
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      endl <<
      endl;
  }

  fOnGoingRestMeasures = false;
}

void lpsr2LilypondTranslator::visitStart (S_msrRestMeasuresContents& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> Start visiting msrRestMeasuresContents" <<
      endl;
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "% start of rest measures contents " <<
      /* JMI
      singularOrPlural (
        restMeasuresNumber,
        "measure",
        "measures") <<
        */
      ", line " << inputLineNumber <<
      endl <<
      endl;

    gIndenter++;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrRestMeasuresContents& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "%--> End visiting msrRestMeasuresContents" <<
      endl;
  }
#endif

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gLpsr2LilypondOah->fLilyPondComments) {
    fLilypondCodeOstream << left <<
      setw (commentFieldWidth) <<
      "% end of rest measures contents " <<
      /* JMI
      singularOrPlural (
        restMeasuresNumber,
        "measure",
        "measures") <<
        */
      ", line " << inputLineNumber <<
      endl <<
      endl;

    gIndenter--;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMidiTempo& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> Start visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif

  if (gLpsr2LilypondOah->fNoMidi) {
    fLilypondCodeOstream <<
      "%{" <<
      endl;

     gIndenter++;
  }

  fLilypondCodeOstream <<
    "\\midi" << " {" <<
    endl;

  gIndenter++;

  if (gLpsr2LilypondOah->fNoMidi) {
    fLilypondCodeOstream <<
      "% ";
  }

  fLilypondCodeOstream <<
    "\\tempo " <<
    elt->getMidiTempoDuration () << // BLARK
    " = " <<
    elt->getMidiTempoPerSecond () <<
    endl;

  gIndenter--;

  fLilypondCodeOstream <<
    "}" <<
    endl;

  if (gLpsr2LilypondOah->fNoMidi) {
    gIndenter--;

    fLilypondCodeOstream <<
      "%}" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrMidiTempo& elt)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    fLilypondCodeOstream <<
      "% --> End visiting msrMidiTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
#endif
}


} // namespace
