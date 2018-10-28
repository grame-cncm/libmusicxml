/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iomanip>      // setw, setprecision, ...
#include <cmath>
#include <string>

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "musicXMLOptions.h"
#include "lilypondOptions.h"

#include "xml2lyOptionsHandling.h"

#include "bsr2BrailleTranslator.h"


using namespace std;

namespace MusicXML2
{

// for comments highlighting in the generated Braille code
const int commentFieldWidth = 30;

/*
//______________________________________________________________________________
S_msrRepeatDescr msrRepeatDescr::create (
      int repeatEndingsNumber)
{
  msrRepeatDescr* o = new
    msrRepeatDescr (
      repeatEndingsNumber);
  assert(o!=0);
  return o;
}

msrRepeatDescr::msrRepeatDescr (
      int repeatEndingsNumber)
{
  fRepeatEndingsNumber  = repeatEndingsNumber;
  fRepeatEndingsCounter = 0;
  
  fEndOfRepeatHasBeenGenerated = false;
}

msrRepeatDescr::~msrRepeatDescr ()
{}

string msrRepeatDescr::repeatDescrAsString () const
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

void msrRepeatDescr::print (ostream& os) const
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

ostream& operator<< (ostream& os, const S_msrRepeatDescr& elt)
{
  elt->print (os);
  return os;
}
*/

//________________________________________________________________________
bsr2BrailleTranslator::bsr2BrailleTranslator (
  S_msrOptions&    msrOpts,
  S_bsrOptions&   bsrOpts,
  indentedOstream& logIOstream,
  indentedOstream& lilypondOutputStream,
  S_bsrScore      bsrScore)
    : fLogOutputStream (
        logIOstream),
      fBrailleCodeIOstream (
        lilypondOutputStream)
        /*,
      fNullIOstream (
        indentedOstream ("/dev/null"))
     */
{
  fMsrOptions  = msrOpts;
  fBsrOptions = bsrOpts;
  
  // the BSR score we're visiting
  fVisitedBsrScore = bsrScore;

  // inhibit the browsing of measure repeats replicas,
  // since Braille only needs the repeat measure
  fVisitedBsrScore->
    getMsrScore ()->
      setInhibitMeasuresRepeatReplicasBrowsing ();

/* JMI
  // inhibit the browsing of multiple rest replicas,
  // since Braille only needs the measure number
  fVisitedBsrScore->
    getMsrScore ()->
      setInhibitMultipleRestMeasuresBrowsing ();
*/

/*
  // header handling
  fOnGoingHeader = false;

  // time
  fVoiceIsCurrentlySenzaMisura = false;
  fOnGoingVoiceCadenza = false;
  
  // staves
  fOnGoingStaff = false;

  // voices
  fOnGoingVoice = false;

  // harmonies
  fOnGoingHarmonyVoice = false;
  fPowerChordHaveAlreadyBeenGenerated = false;

  // figured bass
  fOnGoingFiguredBassVoice = false;
  fCurrentFiguredBassFiguresCounter = 0;

  // repeats

  // multiple rest measures
  fRemainingMultipleRestMeasuresNumber = 0;
  fOnGoingMultipleRestMeasures = false;

  // measures
  fCurrentVoiceMeasuresCounter = -1;
  
  // durations
  fLastMetWholeNotes = rational (0, 1);

  // notes
  fCurrentNotePrintKind = msrNote::kNotePrintYes; // default value
  fOnGoingNote = false;

  // grace notes
  fOnGoingGraceNotesGroup = false;
  
  // articulations
  fCurrentArpeggioDirectionKind = kDirectionNone;

  // stems
  fCurrentStemKind = msrStem::kStemNone;

  // double tremolos

  // chords
  fOnGoingChord = false; // JMI

  // trills
  fOnGoingTrillSpanner = false;

  // spanners
  fCurrentSpannerPlacementKind = kPlacementNone;

  // stanzas
  fGenerateCodeForOngoingNonEmptyStanza = false;

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
  */
};
  
bsr2BrailleTranslator::~bsr2BrailleTranslator ()
{}

//________________________________________________________________________
void bsr2BrailleTranslator::generateBrailleCodeFromBsrScore ()
{
  if (fVisitedBsrScore) {    
    // browse a msrScore browser
    msrBrowser<bsrScore> browser (this);
    browser.browse (*fVisitedBsrScore);
  }
}

/*
//________________________________________________________________________
string bsr2BrailleTranslator::absoluteOctaveAsBrailleString (
  int absoluteOctave)
{
  string result;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    fBrailleCodeIOstream <<
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
        / * JMI
        stringstream s;

        s <<
          "%{absolute octave " << absoluteOctave << "???%}";

        result = s.str ();
        * /
      }
  } // switch

  return result;
}

//________________________________________________________________________
string bsr2BrailleTranslator::alterationKindAsBrailleString (
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
string bsr2BrailleTranslator::lilypondRelativeOctave (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // generate LilyPond octave relative to fRelativeOctaveReference

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
      fRelativeOctaveReference->
        noteDiatonicPitchKind (
          inputLineNumber);

  string
    referenceDiatonicPitchKindAsString =
      fRelativeOctaveReference->
        noteDiatonicPitchKindAsString (
          inputLineNumber);
      
  int
    referenceAbsoluteOctave =
      fRelativeOctaveReference->
        getNoteOctave ();

  / *
    If no octave changing mark is used on a pitch, its octave is calculated
    so that the interval with the previous note is less than a fifth.
    This interval is determined without considering accidentals.
  * /
    
  int
    noteAboluteDiatonicOrdinal =
      noteAbsoluteOctave * 7
        +
      noteDiatonicPitchKind - kC,
      
    referenceAboluteDiatonicOrdinal =
      referenceAbsoluteOctave * 7
        +
      referenceDiatonicPitchKind - kC;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    const int fieldWidth = 28;

    fBrailleCodeIOstream << left <<
/ *
      setw (fieldWidth) <<
      "% referenceDiatonicPitch" <<
      " = " <<
      referenceDiatonicPitch <<
      endl <<
* /
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
      endl <<
      setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      " = " <<
      referenceAboluteDiatonicOrdinal <<
      endl <<
      setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal <<
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

  return s.str ();
}

string bsr2BrailleTranslator::stringTuningAsBrailleString (
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

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceScordaturas) {
    int
      getStringTuningNumber =
        stringTuning->
          getStringTuningNumber ();

    fBrailleCodeIOstream <<
      endl <<
      "%getStringTuningNumber = " <<
      getStringTuningNumber <<
      endl <<
      "%stringTuningDiatonicPitchKind = " <<
      msrDiatonicPitchKindAsString (
        stringTuningDiatonicPitchKind) <<
      endl <<
      "%stringTuningAlterationKind = " <<
      alterationKindAsBrailleString (
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
        gBsrOptions->
          fBsrQuarterTonesPitchesLanguageKind,
          quarterTonesPitchKind) <<
      endl <<
      endl;
  }
#endif
  
  stringstream s;

  s <<
    msrQuarterTonesPitchKindAsString (
      gBsrOptions->
        fBsrQuarterTonesPitchesLanguageKind,
        quarterTonesPitchKind) <<
    absoluteOctaveAsBrailleString (
      stringTuningOctave);

  return s.str ();
}
  
//________________________________________________________________________
string bsr2BrailleTranslator::notePitchAsBrailleString (
  S_msrNote note)
{
  stringstream s;

  // should an editorial accidental be generated?
  switch (note->getNoteEditorialAccidentalKind ()) {
    case msrNote::kNoteEditorialAccidentalYes:
      s <<
        "\\editorialAccidental ";
      break;
    case msrNote::kNoteEditorialAccidentalNo:
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
        gBsrOptions->fBsrQuarterTonesPitchesLanguageKind,
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
        gBsrOptions->fBsrQuarterTonesPitchesLanguageKind,
        noteQuarterTonesDisplayPitchKind);
      
  // generate the pitch
  s <<
    quarterTonesPitchKindAsString;
    
  // in MusicXML, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();
    
  // should an absolute octave be generated?
  bool generateAbsoluteOctave =
    gBrailleOptions->fAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    int noteAbsoluteDisplayOctave =
      note->getNoteDisplayOctave ();
  
      const int fieldWidth = 28;

    fBrailleCodeIOstream << left <<
      endl <<
      setw (fieldWidth) <<
      "% line" <<
      " = " <<
      note->getInputLineNumber () <<
      endl <<
      setw (fieldWidth) <<
      "% msrQuarterTonesPitch" <<
      " = " <<
      quarterTonesPitchKindAsString <<
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
      endl;
  }
#endif

  if (generateAbsoluteOctave) {
    // generate LilyPond absolute octave
    s <<
      absoluteOctaveAsBrailleString (
        noteAbsoluteOctave);
  }

  else {
    // generate LilyPond octave relative to fRelativeOctaveReference
    s <<
      lilypondRelativeOctave (note);
  }

  // should an accidental be generated? JMI this can be fine tuned with cautionary
  switch (note->getNoteAccidentalKind ()) {
    case msrNote::kNoteAccidentalNone:
      break;
    default:
      s <<
        "!";
      break;
  } // switch

  // should a cautionary accidental be generated?
  switch (note->getNoteCautionaryAccidentalKind ()) {
    case msrNote::kNoteCautionaryAccidentalYes:
      s <<
        "?";
      break;
    case msrNote::kNoteCautionaryAccidentalNo:
      break;
  } // switch

  return s.str ();
}

//________________________________________________________________________
string bsr2BrailleTranslator::durationAsBrailleString (
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
      gBrailleOptions->fAllDurations;
  }
  
  if (generateExplicitDuration) {
    result =
      wholeNotesAsBrailleString (
        inputLineNumber,
        wholeNotes);
  }

  return result;
}

//________________________________________________________________________
string bsr2BrailleTranslator::pitchedRestAsBrailleString (
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
    quarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gBsrOptions->fBsrQuarterTonesPitchesLanguageKind,
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
        gBsrOptions->fBsrQuarterTonesPitchesLanguageKind,
        noteQuarterTonesDisplayPitchKind);
      
  // generate the display pitch
  s <<
    note->
      noteDisplayPitchKindAsString ();
//    note->notePitchAsString (); JMI
//    quarterTonesDisplayPitchAsString;
    
  // in MusicXML, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();
    
  // should an absolute octave be generated?
  bool generateAbsoluteOctave =
    gBrailleOptions->fAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;

  int noteAbsoluteDisplayOctave =
    note->getNoteDisplayOctave ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotes) {
    const int fieldWidth = 28;

    fBrailleCodeIOstream << left <<
      endl <<
      setw (fieldWidth) <<
      "% line" <<
      " = " <<
      note->getInputLineNumber () <<
      endl <<
      setw (fieldWidth) <<
      "% msrQuarterTonesPitch" <<
      " = " <<
      quarterTonesPitchKindAsString <<
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
      endl;
  }
#endif

  if (generateAbsoluteOctave) {
    // generate LilyPond absolute octave
    s <<
      absoluteOctaveAsBrailleString (
        noteAbsoluteDisplayOctave);
  }

  else {
    // generate LilyPond octave relative to fRelativeOctaveReference
    s <<
      lilypondRelativeOctave (note);
  }

  // generate the skip duration
  s <<
    durationAsBrailleString (
      note->getInputLineNumber (),
      note->
        getNoteSoundingWholeNotes ());

  // generate the '\rest'
  s <<
    "\\rest ";

  return s.str ();
}

void bsr2BrailleTranslator::generateNote (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // print the note ligatures if any
  list<S_msrLigature>
    noteLigatures =
      note->getNoteLigatures ();

  if (noteLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin ();
      i!=noteLigatures.end ();
      i++) {
      S_msrLigature ligature = (*i);

      switch (ligature->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
          
        case msrLigature::kLigatureStart:
          {
            / *
              the edge height is relative to the voice,
              i.e. a positive value points down in voices 1 and 3
              and it points up in voices 2 and 4,
            * /
            
            // fetch note's voice
            S_msrVoice
              noteVoice =
                note->
                  getNoteMeasureUplink ()->
                    getMeasureSegmentUplink ()->
                      getSegmentVoiceUplink ();

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

#ifdef TRACE_OPTIONS
            if (gTraceOptions->fTraceLigatures) {
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
            
            float       ligatureStartEdgeHeight;

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
                fBrailleCodeIOstream <<
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
                  getLigatureOtherEndSidelink ();

            // compute ligature end edge height
            float ligatureEndEdgeHeight;
                
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
                fBrailleCodeIOstream <<
                  "%{ligatureLineEndArrow???%} ";
                break;
                
              case msrLigature::kLigatureLineEndNone:
                ligatureEndEdgeHeight = 0;
                break;
            } // switch

            // generate the code the the edge-height pair of values
            fBrailleCodeIOstream <<
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
              fBrailleCodeIOstream <<
                endl <<
                "\\once\\override LigatureBracket.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fBrailleCodeIOstream <<
                endl <<
                "\\once\\override LigatureBracket.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fBrailleCodeIOstream <<
                endl <<
                "\\once\\override LigatureBracket.style = #'zigzag" <<
                endl;
              break;
          } // switch
          
          fBrailleCodeIOstream << "\\[ ";
          break;
          
        case msrLigature::kLigatureContinue:
          break;
          
        case msrLigature::kLigatureStop:
   // JMI       fBrailleCodeIOstream << "\\] ";
          break;
      } // switch
    } // for
  }

  // get note stem kind 
  S_msrStem
    noteStem =
      note->getNoteStem ();

  msrStem::msrStemKind noteStemKind;

  if (noteStem) {
    noteStemKind = noteStem->getStemKind ();
  }
  else {
    noteStemKind = msrStem::kStemNone;
  }

  // handle note kind before printing note itself
  switch (note->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kRestNote:      
      break;
      
    case msrNote::kSkipNote:      
      break;
      
    case msrNote::kUnpitchedNote:
      {
        // is this note in a tab staff?
        S_msrStaff
          noteStaff =
            note->
              getNoteMeasureUplink ()->
                getMeasureSegmentUplink ()->
                  getSegmentVoiceUplink ()->
                    getVoiceStaffUplink ();
        
        switch (noteStaff->getStaffKind ()) {
          case msrStaff::kTablatureStaff:
            break;
          case msrStaff::kHarmonyStaff:
            break;
          case msrStaff::kFiguredBassStaff:
            break;
          case msrStaff::kDrumStaff:
            break;
          case msrStaff::kRythmicStaff: // JMI
            // should the stem be omitted?
            if (note->getNoteIsStemless ()) {
              fBrailleCodeIOstream <<
                endl <<
                "\\stemNeutral "; // JMI ""\\once\\omit Stem ";

              fCurrentStemKind = msrStem::kStemNone;
            }
    
            // should stem direction be generated?
            if (noteStemKind != fCurrentStemKind) {
              switch (noteStemKind) {
                case msrStem::kStemNone:
                  fBrailleCodeIOstream << "\\stemNeutral ";
                  break;
                case msrStem::kStemUp:
                  fBrailleCodeIOstream << "\\stemUp ";
                  break;
                case msrStem::kStemDown:
                  fBrailleCodeIOstream << "\\stemDown ";
                  break;
                case msrStem::kStemDouble: // JMI ???
                  break;
              } // switch

              fCurrentStemKind = noteStemKind;
            }
            break;

          case msrStaff::kRegularStaff:
            // should the stem be omitted?
            if (note->getNoteIsStemless ()) {
              fBrailleCodeIOstream <<
                endl <<
                "\\stemNeutral "; // JMI ""\\once\\omit Stem ";

              fCurrentStemKind = msrStem::kStemNone;
            }
    
            // should stem direction be generated?
            if (gBrailleOptions->fStems) {
              if (noteStemKind != fCurrentStemKind) {
                switch (noteStemKind) {
                  case msrStem::kStemNone:
                    fBrailleCodeIOstream << "\\stemNeutral ";
                    break;
                  case msrStem::kStemUp:
                    fBrailleCodeIOstream << "\\stemUp ";
                    break;
                  case msrStem::kStemDown:
                    fBrailleCodeIOstream << "\\stemDown ";
                    break;
                  case msrStem::kStemDouble: // JMI ???
                    break;
                } // switch

                fCurrentStemKind = noteStemKind;
              }
            }
            break;
        } // switch
      }
      break;

    case msrNote::kStandaloneNote:
      {
        // is this note in a tab staff?
        S_msrStaff
          noteStaff =
            note->
              getNoteMeasureUplink ()->
                getMeasureSegmentUplink ()->
                  getSegmentVoiceUplink ()->
                    getVoiceStaffUplink ();
        
        switch (noteStaff->getStaffKind ()) {
          case msrStaff::kTablatureStaff:
            break;
          case msrStaff::kHarmonyStaff:
            break;
          case msrStaff::kFiguredBassStaff:
            break;
          case msrStaff::kDrumStaff:
            break;
          case msrStaff::kRythmicStaff:
            break;

          case msrStaff::kRegularStaff:
            // should the stem be omitted?
            if (note->getNoteIsStemless ()) {
              fBrailleCodeIOstream <<
                endl <<
                "\\stemNeutral "; // JMI ""\\once\\omit Stem ";

              fCurrentStemKind = msrStem::kStemNone;
            }
    
            // should stem direction be generated?
            if (gBrailleOptions->fStems) {
              if (noteStemKind != fCurrentStemKind) {
                switch (noteStemKind) {
                  case msrStem::kStemNone:
                    fBrailleCodeIOstream << "\\stemNeutral ";
                    break;
                  case msrStem::kStemUp:
                    fBrailleCodeIOstream << "\\stemUp ";
                    break;
                  case msrStem::kStemDown:
                    fBrailleCodeIOstream << "\\stemDown ";
                    break;
                  case msrStem::kStemDouble: // JMI ???
                    break;
                } // switch

                fCurrentStemKind = noteStemKind;
              }
            }
            break;
        } // switch
      }
      break;

    case msrNote::kDoubleTremoloMemberNote:
      {
        // should the stem be omitted?
        if (note->getNoteIsStemless ()) {
          fBrailleCodeIOstream <<
            endl <<
            "\\stemNeutral"; // JMI ""\\once\\omit Stem ";
        }

        // should stem direction be generated?
        if (gBrailleOptions->fStems) {
          if (noteStemKind != fCurrentStemKind) {
            switch (noteStemKind) {
              case msrStem::kStemNone:
                fBrailleCodeIOstream << "\\stemNeutral ";
                break;
              case msrStem::kStemUp:
                fBrailleCodeIOstream << "\\stemUp ";
                break;
              case msrStem::kStemDown:
                fBrailleCodeIOstream << "\\stemDown ";
                break;
                / * JMI
              case msrStem::kStemNone:
                break;
                * /
              case msrStem::kStemDouble: // JMI ???
                break;
            } // switch

            fCurrentStemKind = noteStemKind;
          }
        }
      }
      break;

    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      break;
      
    case msrNote::kChordMemberNote:
     // don't omit stems for chord member note JMI
     break;
      
    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
      break;
  } // switch

  // handling note head
  // these tweaks should occur right before the note itself
  if (! note->getNoteIsARest ()) { // JMI ???
    msrNote::msrNoteHeadKind
      noteHeadKind =
        note->getNoteHeadKind ();
   
    switch (noteHeadKind) {
      case msrNote::kNoteHeadSlash:
        fBrailleCodeIOstream << "\\tweak style #'slash ";
        break;
      case msrNote::kNoteHeadTriangle:
        fBrailleCodeIOstream << "\\tweak style #'triangle ";
        break;
      case msrNote::kNoteHeadDiamond:
   // JMI     fBrailleCodeIOstream << "\\tweak style #'diamond ";
        fBrailleCodeIOstream << "\\harmonic ";
        break;
      case msrNote::kNoteHeadSquare:
        fBrailleCodeIOstream << "\\tweak style #'la ";
        break;
      case msrNote::kNoteHeadCross:
        fBrailleCodeIOstream << "\\tweak style #'cross ";
        break;
      case msrNote::kNoteHeadX:
        fBrailleCodeIOstream << "\\tweak style #'cross %{x%} ";
        break;
      case msrNote::kNoteHeadCircleX:
        fBrailleCodeIOstream << "\\tweak style #'xcircle ";
        break;
      case msrNote::kNoteHeadInvertedTriangle:
        fBrailleCodeIOstream << "%{kNoteHeadInvertedTriangle%} ";
        break;
      case msrNote::kNoteHeadArrowDown:
        fBrailleCodeIOstream << "%{kNoteHeadArrowDown%} ";
        break;
      case msrNote::kNoteHeadArrowUp:
        fBrailleCodeIOstream << "%{kNoteHeadArrowUp%} ";
        break;
      case msrNote::kNoteHeadSlashed:
        fBrailleCodeIOstream << "%{kNoteHeadSlashed%} ";
        break;
      case msrNote::kNoteHeadBackSlashed:
        fBrailleCodeIOstream << "%{kNoteHeadBackSlashed%} ";
        break;
      case msrNote::kNoteHeadNormal:
   // JMI     fBrailleCodeIOstream << "%{kNoteHeadNormal%} ";
        break;
      case msrNote::kNoteHeadCluster:
        fBrailleCodeIOstream << "%{kNoteHeadCluster%} ";
        break;
      case msrNote::kNoteHeadCircleDot:
        fBrailleCodeIOstream << "%{kNoteHeadCircleDot%} ";
        break;
      case msrNote::kNoteHeadLeftTriangle:
        fBrailleCodeIOstream << "%{kNoteHeadLeftTriangle%} ";
        break;
      case msrNote::kNoteHeadRectangle:
        fBrailleCodeIOstream << "%{kNoteHeadRectangle%} ";
        break;
      case msrNote::kNoteHeadNone:
        fBrailleCodeIOstream << "\\once\\omit NoteHead ";
        break;
      case msrNote::kNoteHeadDo:
        fBrailleCodeIOstream << "\\tweak style #'do ";
        break;
      case msrNote::kNoteHeadRe:
        fBrailleCodeIOstream << "\\tweak style #'re ";
        break;
      case msrNote::kNoteHeadMi:
        fBrailleCodeIOstream << "\\tweak style #'mi ";
        break;
      case msrNote::kNoteHeadFa:
        fBrailleCodeIOstream << "\\tweak style #'fa ";
        break;
      case msrNote::kNoteHeadFaUp:
        fBrailleCodeIOstream << "\\tweak style #'triangle ";
        break;
      case msrNote::kNoteHeadSo:
        fBrailleCodeIOstream << "\\tweak style #'sol ";
        break;
      case msrNote::kNoteHeadLa:
        fBrailleCodeIOstream << "\\tweak style #'la ";
        break;
      case msrNote::kNoteHeadTi:
        fBrailleCodeIOstream << "\\tweak style #'ti ";
        break;
    } // switch
  }

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
          fBrailleCodeIOstream <<
            pitchedRestAsBrailleString (note);

          // this note is the new relative octave reference
          // (the display quarter tone pitch and octave
          // have been copied to the note octave
          // in the msrNote::msrNote () constructor,
          // since the note octave is used in relative code generation)
          fRelativeOctaveReference = note;
        }
  
        else {
          // unpitched rest
          // get the note sounding whole notes
          rational
            noteSoundingWholeNotes =
              note->getNoteSoundingWholeNotes ();
              
          // print the rest name and duration
          if (note->getNoteOccupiesAFullMeasure ()) {
            fBrailleCodeIOstream <<
              "R" <<
              / * JMI
              multipleRestWholeNoteAsBrailleString (
                inputLineNumber,
                noteSoundingWholeNotes);
                * /
              wholeNotesAsBrailleString (
                inputLineNumber,
                noteSoundingWholeNotes);
          }
          
          else {
            fBrailleCodeIOstream <<
              "r" <<
              durationAsBrailleString (
                inputLineNumber,
                noteSoundingWholeNotes);

/* JMI BOF
            if (fOnGoingVoiceCadenza) { // JMI
              if (noteSoundingWholeNotes != rational (1, 1)) {
                / * JMI
                // force the generation of the duration if needed
                if (! gBrailleOptions->fAllDurations) {
                  fBrailleCodeIOstream << // JMI
                    wholeNotesAsBrailleString (
                      inputLineNumber,
                      noteSoundingWholeNotes);
                }
                * /
                
                // generate the multiplying factor
                fBrailleCodeIOstream << // JMI
                  "*" <<
                  noteSoundingWholeNotes <<
                  "";
              }
            }
                  * /
          }

          // an unpitched rest is no relative octave reference,
          // the preceding one is kept
        }
      }
      break;
      
    case msrNote::kSkipNote:      
      // print the skip name
      fBrailleCodeIOstream << "s";
      
      // print the skip duration
      fBrailleCodeIOstream <<
        durationAsBrailleString (
          inputLineNumber,
          note->
            getNoteSoundingWholeNotes ());

      // a rest is no relative octave reference,
      // the preceding one is kept
      break;
      
    case msrNote::kUnpitchedNote:
      {
        // print the note name, "e" by convention
        fBrailleCodeIOstream <<
            "e";
  
        rational
          noteSoundingWholeNotes =
            note->
              getNoteSoundingWholeNotes ();
          
        // print the note duration
        fBrailleCodeIOstream <<
          durationAsBrailleString (
            inputLineNumber,
            noteSoundingWholeNotes);
  
        // handle delayed ornaments if any
        if (note->getNoteDelayedTurnOrnament ()) {
          // c2*2/3 ( s2*1/3\turn) JMI
          // we need the explicit duration in all cases,
          // regardless of gGeneralOptions->fAllDurations
          fBrailleCodeIOstream <<
            wholeNotesAsBrailleString (
              inputLineNumber,
              noteSoundingWholeNotes) <<
            "*" <<
            gBrailleOptions->fDelayedOrnamentsFraction;
        }
        
        // print the tie if any
        {
          S_msrTie noteTie = note->getNoteTie ();
        
          if (noteTie) {
            if (noteTie->getTieKind () == msrTie::kTieStart) {
              fBrailleCodeIOstream << " ~";
            }
          }
        }
      }
      break;

    case msrNote::kStandaloneNote:
      {
        // print the note name
        fBrailleCodeIOstream <<
          notePitchAsBrailleString (note);
  
        rational
          noteSoundingWholeNotes =
            note->
              getNoteSoundingWholeNotes ();
          
        // print the note duration
        fBrailleCodeIOstream <<
          durationAsBrailleString (
            inputLineNumber,
            noteSoundingWholeNotes);
  
        // handle delayed ornaments if any
        if (note->getNoteDelayedTurnOrnament ()) {
          // c2*2/3 ( s2*1/3\turn) JMI
          // we need the explicit duration in all cases,
          // regardless of gGeneralOptions->fAllDurations
          fBrailleCodeIOstream <<
          /*  JMI TOO MUCH
            wholeNotesAsBrailleString (
              inputLineNumber,
              noteSoundingWholeNotes) <<
              * /
            "*" <<
            gBrailleOptions->fDelayedOrnamentsFraction;
        }
        
        // print the tie if any
        {
          S_msrTie noteTie = note->getNoteTie ();
        
          if (noteTie) {
            if (noteTie->getTieKind () == msrTie::kTieStart) {
              fBrailleCodeIOstream << " ~";
            }
          }
        }
  
        // this note is the new relative octave reference
        fRelativeOctaveReference = note;
      }
      break;

    case msrNote::kDoubleTremoloMemberNote:
      // print the note name
      fBrailleCodeIOstream <<
        notePitchAsBrailleString (note);
      
      // print the note duration
      fBrailleCodeIOstream <<
        wholeNotesAsBrailleString (
          inputLineNumber,
          note->getNoteSoundingWholeNotes ());

      // handle delayed ornaments if any
      if (note->getNoteDelayedTurnOrnament ()) {
        // c2*2/3 ( s2*1/3\turn JMI
        fBrailleCodeIOstream <<
          "*" <<
          gBrailleOptions->fDelayedOrnamentsFraction;
      }
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fBrailleCodeIOstream << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kGraceNote:
      // print the note name
      fBrailleCodeIOstream <<
        notePitchAsBrailleString (note);
      
      // print the grace note's graphic duration
      fBrailleCodeIOstream <<
        msrDurationKindAsString (
          note->
            getNoteGraphicDurationKind ());

      // print the dots if any JMI ???
      for (int i = 0; i < note->getNoteDotsNumber (); i++) {
        fBrailleCodeIOstream << ".";
      } // for
      
      // don't print the tie if any, 'acciacattura takes care of it
      / *
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fBrailleCodeIOstream << "~ ";
          }
        }
      }
      * /

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kGraceChordMemberNote:
      // print the note name
      fBrailleCodeIOstream <<
        notePitchAsBrailleString (note);
      
      // dont't print the grace note's graphic duration

      // print the dots if any JMI ???
      for (int i = 0; i < note->getNoteDotsNumber (); i++) {
        fBrailleCodeIOstream << ".";
      } // for
      
      // don't print the tie if any, 'acciacattura takes care of it
      / *
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fBrailleCodeIOstream << "~ ";
          }
        }
      }
      * /

      // inside chords, a note is relative to the preceding one
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kChordMemberNote:
      {
        // print the note name
        fBrailleCodeIOstream <<
          notePitchAsBrailleString (note);
        
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
            i++) {
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
        fRelativeOctaveReference = note;
      }
      break;
      
    case msrNote::kTupletMemberNote:
      if (gBrailleOptions->fIndentTuplets) {
        fBrailleCodeIOstream <<
          endl;
      }
        
      // print the note name
      if (note->getNoteIsARest ()) {
        fBrailleCodeIOstream <<
          string (
            note->getNoteOccupiesAFullMeasure ()
              ? "R"
              : "r");
      }
      else {
        fBrailleCodeIOstream <<
          notePitchAsBrailleString (note);
      }
      
      // print the note display duration
      fBrailleCodeIOstream <<
        durationAsBrailleString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fBrailleCodeIOstream << "~ ";
          }
        }
      }

      // a rest is no relative octave reference,
      if (! note->getNoteIsARest ()) {
        // this note is the new relative octave reference
        fRelativeOctaveReference = note;
      }
      break;
      
    case msrNote::kGraceTupletMemberNote:
      if (gBrailleOptions->fIndentTuplets) {
        fBrailleCodeIOstream <<
          endl;
      }
        
      // print the note name
      if (note->getNoteIsARest ()) {
        fBrailleCodeIOstream <<
          string (
            note->getNoteOccupiesAFullMeasure ()
              ? "R"
              : "r");
      }
      else {
        fBrailleCodeIOstream <<
          notePitchAsBrailleString (note);
      }
      
      // print the note display duration
      fBrailleCodeIOstream <<
        durationAsBrailleString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fBrailleCodeIOstream << "~ ";
          }
        }
      }

      // this note is no new relative octave reference JMI ???
      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kTupletMemberUnpitchedNote:
      if (gBrailleOptions->fIndentTuplets) {
        fBrailleCodeIOstream <<
          endl;
      }
        
      // print the note name
      fBrailleCodeIOstream <<
        "e"; // by convention
      
      // print the note (display) duration
      fBrailleCodeIOstream <<
        durationAsBrailleString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fBrailleCodeIOstream << "~ ";
          }
        }
      }
      break;
  } // switch
  
  fBrailleCodeIOstream << " ";
}

//________________________________________________________________________
/* JMI
string bsr2BrailleTranslator::durationAsExplicitBrailleString (
  int      inputLineNumber,
  rational wholeNotes)
{
  string result;
  
  result =
    wholeNotesAsBrailleString (
      inputLineNumber,
      wholeNotes);

  return result;
}
*/

/*
//________________________________________________________________________
void bsr2BrailleTranslator::generateNoteArticulation (
  S_msrArticulation articulation)
{
  // should the placement be generated?
  bool doGeneratePlacement = true;

  // generate note articulation preamble if any
  switch (articulation->getArticulationKind ()) {
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
        fBrailleCodeIOstream << "-";
        break;
      case kPlacementAbove:
        fBrailleCodeIOstream << "^";
        break;
      case kPlacementBelow:
        fBrailleCodeIOstream << "_";
        break;
    } // switch
  }

  // generate note articulation itself
  switch (articulation->getArticulationKind ()) {
    case msrArticulation::kAccent:
      fBrailleCodeIOstream << ">";
      break;
    case msrArticulation::kBreathMark:
      fBrailleCodeIOstream << "\\breathe";
      break;
    case msrArticulation::kCaesura:
    / * JMI
          fBrailleCodeIOstream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl;
     * /
      fBrailleCodeIOstream <<
        endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        endl <<
        "\\breathe" <<
        endl;
      break;
    case msrArticulation::kSpiccato:
      fBrailleCodeIOstream <<
        "%{spiccato???%}";
      break;
    case msrArticulation::kStaccato:
      fBrailleCodeIOstream <<
        ".";
      break;
    case msrArticulation::kStaccatissimo:
      fBrailleCodeIOstream <<
        "!";
      break;
    case msrArticulation::kStress:
      fBrailleCodeIOstream <<
        "%{stress???%}";
      break;
    case msrArticulation::kUnstress:
      fBrailleCodeIOstream <<
        "%{unstress???%}";
      break;
    case msrArticulation::kDetachedLegato:
      fBrailleCodeIOstream <<
        "_"; // portato
      break;
    case msrArticulation::kStrongAccent:
      fBrailleCodeIOstream <<
        "^"; // marcato
      break;
    case msrArticulation::kTenuto:
      fBrailleCodeIOstream <<
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
            fBrailleCodeIOstream << "_";
            break;
        } // switch
      
        switch (fermata->getFermataKind ()) {
          case msrFermata::kNormalFermataKind:
            fBrailleCodeIOstream << "\\fermata ";
            break;
          case msrFermata::kAngledFermataKind:
            fBrailleCodeIOstream << "\\shortfermata ";
            break;
          case msrFermata::kSquareFermataKind:
            fBrailleCodeIOstream << "\\longfermata ";
            break;
        } // switch
      }
      else {
        stringstream s;
    
        s <<
          "note articulation '" <<
          articulation->asString () <<
          "'has 'fermata' kind, but is not of type S_msrFermata" <<
          ", line " << articulation->getInputLineNumber ();
          
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
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
      fBrailleCodeIOstream <<
        "\\bendAfter #+4";
      break;
    case msrArticulation::kFalloff:
      fBrailleCodeIOstream <<
        "\\bendAfter #-4";
      break;
    case msrArticulation::kPlop:
      fBrailleCodeIOstream <<
        "%{plop???%}";
      break;
    case msrArticulation::kScoop:
      fBrailleCodeIOstream <<
        "%{scoop???%}";
      break;
  } // switch
}

//________________________________________________________________________
void bsr2BrailleTranslator::generateChordArticulation (
  S_msrArticulation articulation)
{  
  switch (articulation->getArticulationPlacementKind ()) {
    case kPlacementNone:
      fBrailleCodeIOstream << "-";
      break;
    case kPlacementAbove:
      fBrailleCodeIOstream << "^";
      break;
    case kPlacementBelow:
      fBrailleCodeIOstream << "_";
      break;
  } // switch

  switch (articulation->getArticulationKind ()) {
    case msrArticulation::kAccent:
      fBrailleCodeIOstream << ">";
      break;
    case msrArticulation::kBreathMark:
      fBrailleCodeIOstream << "\\breathe";
      break;
    case msrArticulation::kCaesura:
    / * JMI
          fBrailleCodeIOstream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl;
     * /
      fBrailleCodeIOstream <<
        endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        endl <<
      "\\breathe" <<
        endl;
      break;
    case msrArticulation::kSpiccato:
      fBrailleCodeIOstream <<
        "%{spiccato???%}";
      break;
    case msrArticulation::kStaccato:
      fBrailleCodeIOstream <<
        "\\staccato"; // JMI "-.";
      break;
    case msrArticulation::kStaccatissimo:
      fBrailleCodeIOstream << "!";
      break;
    case msrArticulation::kStress:
      fBrailleCodeIOstream <<
        "%{stress???%}";
      break;
    case msrArticulation::kUnstress:
      fBrailleCodeIOstream <<
        "%{unstress%}";
      break;
    case msrArticulation::kDetachedLegato:
      fBrailleCodeIOstream <<
        "_"; // portato
      break;
    case msrArticulation::kStrongAccent:
      fBrailleCodeIOstream <<
        "^"; // marcato
      break;
    case msrArticulation::kTenuto:
      fBrailleCodeIOstream << "-";
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
            fBrailleCodeIOstream << "_";
            break;
        } // switch
      
        switch (fermata->getFermataKind ()) {
          case msrFermata::kNormalFermataKind:
            fBrailleCodeIOstream << "\\fermata ";
            break;
          case msrFermata::kAngledFermataKind:
            fBrailleCodeIOstream << "\\shortfermata ";
            break;
          case msrFermata::kSquareFermataKind:
            fBrailleCodeIOstream << "\\longfermata ";
            break;
        } // switch
      }
      else {
        stringstream s;
    
        s <<
          "chord articulation '" <<
          articulation->asString () <<
          "'has 'fermata' kind, but is not of type S_msrFermata" <<
          ", line " << articulation->getInputLineNumber ();
          
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          articulation->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      break;
      
    case msrArticulation::kArpeggiato:
      fBrailleCodeIOstream <<
        "\\arpeggio";
      break;
    case msrArticulation::kNonArpeggiato:
      fBrailleCodeIOstream <<
        "\\arpeggio";
      break;
    case msrArticulation::kDoit:
      fBrailleCodeIOstream <<
        "\\bendAfter #+4";
      break;
    case msrArticulation::kFalloff:
      fBrailleCodeIOstream <<
        "\\bendAfter #-4";
      break;
    case msrArticulation::kPlop:
      fBrailleCodeIOstream <<
        "%{plop%}";
      break;
    case msrArticulation::kScoop:
      fBrailleCodeIOstream <<
        "%{scoop%}";
      break;
  } // switch
}

//________________________________________________________________________
string bsr2BrailleTranslator::technicalAsBrailleString (
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
string bsr2BrailleTranslator::technicalWithIntegerAsBrailleString (
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
string bsr2BrailleTranslator::technicalWithFloatAsBrailleString (
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
string bsr2BrailleTranslator::technicalWithStringAsBrailleString (
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
void bsr2BrailleTranslator::generateOrnament (
  S_msrOrnament ornament)
{
  S_msrNote
    ornamentNoteUplink =
      ornament->
        getOrnamentNoteUplink ();
    
  string
    noteUplinkDuration =
      ornamentNoteUplink->
        noteSoundingWholeNotesAsMsrString ();

  switch (ornament->getOrnamentKind ()) {
    case msrOrnament::kOrnamentTrill:
      if (! ornamentNoteUplink->getNoteWavyLineSpannerStart ()) {      
        fBrailleCodeIOstream <<
          "\\trill ";
      }
      else {
        fBrailleCodeIOstream <<
          "\\startTrillSpan ";
      }
      break;
      
    case msrOrnament::kOrnamentDashes:
      if (! ornamentNoteUplink->getNoteWavyLineSpannerStart ()) {      
        fBrailleCodeIOstream <<
          "%{\\dashes%} ";
      }
      break;
      
    case msrOrnament::kOrnamentTurn:
      fBrailleCodeIOstream <<
          "\\turn ";
      break;
      
    case msrOrnament::kOrnamentInvertedTurn:
      fBrailleCodeIOstream <<
          "\\reverseturn ";
      break;
      
    case msrOrnament::kOrnamentDelayedTurn:
      {
        // c2*2/3  s2*1/3\turn
        rational
          remainingFraction =
            rational (1, 1)
              -
            gBrailleOptions->fDelayedOrnamentsFraction;
            
        int
          numerator =
            remainingFraction.getNumerator (),
          denominator =
            remainingFraction.getDenominator ();
              
        fBrailleCodeIOstream <<
          "s" <<
          noteUplinkDuration <<
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
/ * JMI
        fBrailleCodeIOstream <<
          "delayed inverted turn is not supported, replaced by inverted turn," <<
          endl <<
          "see http://lilypond.org/doc/v2.18/Documentation/snippets/expressive-marks";
        
        bsrMusicXMLWarning (
          inputLineNumber,
          s.str ());
          
        result = "\\reverseturn %{ " + s.str () + " %}";
* /
        // c2*2/3 ( s2*1/3\turn
        
        fBrailleCodeIOstream <<
          "s" <<
          noteUplinkDuration <<
          "*1/3" "\\reverseturn ";
      }
      break;
      
    case msrOrnament::kOrnamentVerticalTurn:
      fBrailleCodeIOstream <<
        "^\\markup { \\rotate #90 \\musicglyph #\"scripts.turn\" } ";
          /* JMI
      {
        string message =
          "delayed vertical turn is not supported, ignored";
        
        bsrMusicXMLWarning (
          inputLineNumber,
          message);
          
        result = "%{ " + message + " %}";
      }
        * /
      break;
      
    case msrOrnament::kOrnamentMordent:
      fBrailleCodeIOstream <<
        "\\mordent ";
      break;
      
    case msrOrnament::kOrnamentInvertedMordent:
      fBrailleCodeIOstream <<
        "\\prall ";
      break;
      \
    case msrOrnament::kOrnamentSchleifer:
      fBrailleCodeIOstream <<
        "%{\\schleifer???%} ";
      break;
      
    case msrOrnament::kOrnamentShake:
      fBrailleCodeIOstream <<
        "%{\\shake???%} ";
      break;
      
    case msrOrnament::kOrnamentAccidentalMark:
      switch (ornament->getOrnamentPlacementKind ()) {
        case kPlacementNone:
          fBrailleCodeIOstream << "-";
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fBrailleCodeIOstream << "_";
          break;
      } // switch

      switch (ornament->getOrnamentAccidentalMark ()) {
        case kTripleFlat:
          fBrailleCodeIOstream << "\\markup { \\tripleflat } ";
          break;
        case kDoubleFlat:
          fBrailleCodeIOstream << "\\markup { \\doubleflat } ";
          break;
        case kSesquiFlat:
          fBrailleCodeIOstream << "\\markup { \\sesquiflat } ";
          break;
        case kFlat:
          fBrailleCodeIOstream << "\\markup { \\flat } ";
          break;
        case kSemiFlat:
          fBrailleCodeIOstream << "\\markup { \\semiflat } ";
          break;
        case kNatural:
          fBrailleCodeIOstream << "\\markup { \\natural } ";
          break;
        case kSemiSharp:
          fBrailleCodeIOstream << "\\markup { \\semisharp } ";
          break;
        case kSharp:
          fBrailleCodeIOstream << "\\markup { \\sharp } ";
          break;
        case kSesquiSharp:
          fBrailleCodeIOstream << "\\markup { \\sesquisharp } ";
          break;
        case kDoubleSharp:
          fBrailleCodeIOstream << "\\markup { \\doublesharp } ";
          break;
        case kTripleSharp:
          fBrailleCodeIOstream << "\\markup { \\triplesharp } ";
          break;
        case k_NoAlteration:
          break;
      } // switch
      break;
  } // switch
}

//________________________________________________________________________
void bsr2BrailleTranslator::generateCodeForSpannerBeforeNote (
  S_msrSpanner spanner)
{
  switch (spanner->getSpannerKind ()) {
    case msrSpanner::kSpannerDashes:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
          fBrailleCodeIOstream <<
            "\\once \\override TextSpanner.style = #'dashed-line" <<
            endl;
          fOnGoingTrillSpanner = true;
          break;
        case kSpannerTypeStop:
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
          if (spanner->getSpannerNoteUplink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            fOnGoingTrillSpanner = true;
          }
          else {
            fBrailleCodeIOstream <<
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
            fBrailleCodeIOstream <<
              "\\textSpannerUp ";
            break;
          case msrPlacementKind::kPlacementBelow:
            fBrailleCodeIOstream <<
              "\\textSpannerDown ";
            break;
          } // switch

        fCurrentSpannerPlacementKind = spannerPlacementKind;
      }
      break;      
  } // switch
}

//________________________________________________________________________
void bsr2BrailleTranslator::generateCodeForSpannerAfterNote (
  S_msrSpanner spanner)
{
  switch (spanner->getSpannerKind ()) {
    case msrSpanner::kSpannerDashes:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
          fBrailleCodeIOstream <<
            "\\startTextSpan ";
          fOnGoingTrillSpanner = true;
          break;
        case kSpannerTypeStop:
          fBrailleCodeIOstream <<
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
          if (spanner->getSpannerNoteUplink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            fOnGoingTrillSpanner = true;
          }
          else {
            fBrailleCodeIOstream <<
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
                  getSpannerOtherEndSidelink ();

            // has the start end a trill ornament?
            if (spannerStartEnd->getSpannerNoteUplink ()->getNoteTrillOrnament ()) {
              fBrailleCodeIOstream <<
                "\\stopTrillSpan ";
            }
            else {
              fBrailleCodeIOstream <<
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

/ * JMI
      msrPlacementKind
        spannerPlacementKind =
          spanner->getSpannerPlacementKind ();
    
      if (spannerPlacementKind != fCurrentSpannerPlacementKind) {
        switch (spannerPlacementKind) {
          case msrPlacementKind::kPlacementNone:
            break;
          case msrPlacementKind::kPlacementAbove:
            fBrailleCodeIOstream <<
              "\\textSpannerUp ";
            break;
          case msrPlacementKind::kPlacementBelow:
            fBrailleCodeIOstream <<
              "\\textSpannerDown ";
            break;
          } // switch

        fCurrentSpannerPlacementKind = spannerPlacementKind;
      }
      * /

      break;      
  } // switch
}

//________________________________________________________________________
string bsr2BrailleTranslator::dynamicsAsBrailleString (
  S_msrDynamics dynamics)
{
  string result =
    "\\" + dynamics->dynamicsKindAsString ();
    
  return result;
}

//________________________________________________________________________
string bsr2BrailleTranslator::harpPedalTuningAsBrailleString (
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
void bsr2BrailleTranslator::transposeDiatonicError (
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
    gXml2lyOptions->fInputSourceName,
    inputLineNumber,
    __FILE__, __LINE__,
    s.str ());
}

//________________________________________________________________________
string bsr2BrailleTranslator::singleTremoloDurationAsBrailleString (
  S_msrSingleTremolo singleTremolo)
{  
  int
    singleTremoloMarksNumber =
      singleTremolo->
        getSingleTremoloMarksNumber ();

/ * JMI
  S_msrNote
    singleTremoloNoteUplink =
      singleTremolo->
        getSingleTremoloNoteUplink ();
* /

  msrDurationKind
    singleTremoloNoteDurationKind =
      singleTremolo->
        getSingleTremoloGraphicDurationKind ();
    / *
      singleTremoloNoteUplink->
        getNoteGraphicDurationKind ();
    * /
    
  / *
  The same output can be obtained by adding :N after the note,
  where N indicates the duration of the subdivision (it must be at least 8).
  If N is 8, one beam is added to the note’s stem.
  * /
  
  int durationToUse =
    singleTremoloMarksNumber; // JMI / singleTremoloNoteSoundingWholeNotes;
        
  if (singleTremoloNoteDurationKind >= kEighth) {
    durationToUse +=
      1 + (singleTremoloNoteDurationKind - kEighth);
  }
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    fLogOutputStream <<
      "singleTremoloDurationAsBrailleString()" <<
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
    " ";
    
  return s.str ();
}

//________________________________________________________________________
string bsr2BrailleTranslator::harmonyDegreeAlterationKindAsBrailleString (
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

string bsr2BrailleTranslator::harmonyAsBrailleString (
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
  
  // print harmony pitch and duration
  s <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->
        fMsrQuarterTonesPitchesLanguageKind,
      harmony->
        getHarmonyRootQuarterTonesPitchKind ()) <<
    wholeNotesAsBrailleString (
      inputLineNumber,
      harmony->
        getHarmonySoundingWholeNotes ());
    
  // print harmony kind
  switch (harmony->getHarmonyKind ()) {
    case k_NoHarmony:
      s << "Harmony???";
      break;

    // MusicXML chords

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

  msrQuarterTonesPitchKind
    harmonyBassQuarterTonesPitchKind =
      harmony->
        getHarmonyBassQuarterTonesPitchKind ();
      
  if (harmonyBassQuarterTonesPitchKind != k_NoQuarterTonesPitch_QTP) {
    s <<
      "/" <<
      msrQuarterTonesPitchKindAsString (
        gMsrOptions->
          fMsrQuarterTonesPitchesLanguageKind,
        harmonyBassQuarterTonesPitchKind);
  }

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
      i++) {
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
            "." <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsBrailleString (
              harmonyDegreeAlterationKind);
          break;
          
        case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
          s <<
            "." <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsBrailleString (
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
        i++) {
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
      //      "^" <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsBrailleString (
              harmonyDegreeAlterationKind);
            break;
        } // switch
      } // for
    }
  }

  int harmonyInversion =
    harmony->getHarmonyInversion ();
    
  if ( harmonyInversion!= K_HARMONY_NO_INVERSION) {
    s <<
      "%{ inversion: " << harmonyInversion << " %}";
  }
    
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

string bsr2BrailleTranslator::frameAsBrailleString (
  S_msrFrame frame)
{
/ * JMI
  int inputLineNumber =
    frame->getInputLineNumber ();
  * /
    
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
*/

//________________________________________________________________________
string bsr2BrailleTranslator::generateMultilineName (string theString)
{
  stringstream s;

  s <<
     "\\markup { \\center-column { ";
     
  list<string> chunksList;

  splitRegularStringContainingEndOfLines (
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
      s << " ";
    } // for

    s <<
      " } } " <<
      "% " << chunksList.size () << " chunk(s)" << 
      endl;
  }

  return s.str ();
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrScore& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  // initial empty line in LilyPond code
  // to help copy/paste it
// JMI  fBrailleCodeIOstream << endl;
}

void bsr2BrailleTranslator::visitEnd (S_bsrScore& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // final empty line in LilyPond code
  // to help copy/paste it
  fBrailleCodeIOstream <<
    endl;
}

    // names

string bsr2BrailleTranslator::nameAsBrailleString (
  string name)
{
  string result;

  size_t endOfLineFound = name.find ("\n");

  if (endOfLineFound != string::npos) {
    result =
      generateMultilineName (name);
  }
  else {
    result = "\"" + name + "\"";
  }

  return result;
}

/*
string bsr2BrailleTranslator::bsrVarValAssocKindAsBrailleString (
  bsrVarValAssoc::bsrVarValAssocKind
    lilyPondVarValAssocKind)
{
  string result;
  
  switch (lilyPondVarValAssocKind) {
    // library

    case bsrVarValAssoc::kLibraryVersion:
      result = "version";
      break;
      
    // MusicXML informations

    case bsrVarValAssoc::kMusicXMLWorkNumber:
      result = "workNumber";
      break;
    case bsrVarValAssoc::kMusicXMLWorkTitle:
      result = "title";
      break;
    case bsrVarValAssoc::kMusicXMLMovementNumber:
      result = "movementNumber";
      break;
    case bsrVarValAssoc::kMusicXMLMovementTitle:
      result = "subtitle";
      break;
    case bsrVarValAssoc::kMusicXMLEncodingDate:
      result = "encodingDate";
      break;
    case bsrVarValAssoc::kMusicXMLScoreInstrument:
      result = "scoreInstrument";
      break;
    case bsrVarValAssoc::kMusicXMLMiscellaneousField:
      result = "miscellaneousField";
      break;
      
    // LilyPond informations
    
    case bsrVarValAssoc::kBrailleDedication:
      result = "dedication";
      break;

    case bsrVarValAssoc::kBraillePiece:
      result = "piece";
      break;
    case bsrVarValAssoc::kBrailleOpus:
      result = "opus";
      break;

    case bsrVarValAssoc::kBrailleTitle:
      result = "title";
      break;
    case bsrVarValAssoc::kBrailleSubTitle:
      result = "subtitle";
      break;
    case bsrVarValAssoc::kBrailleSubSubTitle:
      result = "subsubtitle";
      break;
      
    case bsrVarValAssoc::kBrailleInstrument:
      result = "instrument";
      break;
    case bsrVarValAssoc::kBrailleMeter:
      result = "meter";
      break;
      
    case bsrVarValAssoc::kBrailleTagline:
      result = "tagline";
      break;
    case bsrVarValAssoc::kBrailleCopyright:
      result = "copyright";
      break;

    case bsrVarValAssoc::kBrailleMyBreak:
      result = "myBreak";
      break;
    case bsrVarValAssoc::kBrailleMyPageBreak:
      result = "myPageBreak";
      break;
    case bsrVarValAssoc::kBrailleGlobal:
      result = "global";
      break;
  } // switch

  return result;
}

string bsr2BrailleTranslator::bsrVarValAssocAsBrailleString (
  S_bsrVarValAssoc bsrVarValAssoc,
  int               fieldNameWidth)
{
  stringstream s;

  s << left <<
    setw (fieldNameWidth) <<
    bsrVarValAssocKindAsBrailleString (
      bsrVarValAssoc->
        getLilyPondVarValAssocKind ()) <<
    " = \"" <<
      bsrVarValAssoc->
        getVariableValue () <<
    "\"";

  return s.str ();
}

string bsr2BrailleTranslator::bsrVarValsListAssocKindAsBrailleString (
  bsrVarValsListAssoc::bsrVarValsListAssocKind
    lilyPondVarValsListAssocKind)
{
  string result;
  
  switch (lilyPondVarValsListAssocKind) {
    // MusicXML informations

    case bsrVarValsListAssoc::kMusicXMLRights:
      result = "rights";
      break;
    case bsrVarValsListAssoc::kMusicXMLComposer:
      result = "composer";
      break;
    case bsrVarValsListAssoc::kMusicXMLArranger:
      result = "arranger";
      break;
    case bsrVarValsListAssoc::kMusicXMLPoet:
      result = "poet";
      break;
    case bsrVarValsListAssoc::kMusicXMLLyricist:
      result = "lyricist";
      break;
    case bsrVarValsListAssoc::kMusicXMLTranslator:
      result = "translator";
      break;
    case bsrVarValsListAssoc::kMusicXMLSoftware:
      result = "software";
      break;
    } // switch

  return result;
}

void bsr2BrailleTranslator::generateBsrVarValsListAssocValues (
  S_bsrVarValsListAssoc varValsListAssoc)
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
      fBrailleCodeIOstream <<
        "\"" << variableValuesList.front () << "\"";
      break;

    default:
      // generate a markup containing the chunks
      fBrailleCodeIOstream <<
        endl <<
        "\\markup {" <<
        endl;

      gIndenter++;
      
      fBrailleCodeIOstream <<
        "\\column {" <<
        endl;
       
      gIndenter++;

      list<string>::const_iterator
        iBegin = variableValuesList.begin (),
        iEnd   = variableValuesList.end (),
        i      = iBegin;
      
      for ( ; ; ) {
        fBrailleCodeIOstream <<
          "\"" << (*i) << "\"";
        if (++i == iEnd) break;
        fBrailleCodeIOstream << endl;
      } // for
  
      fBrailleCodeIOstream <<
        endl;
        
      gIndenter--;

      fBrailleCodeIOstream <<
        "}" <<
        endl <<

      gIndenter--;

      fBrailleCodeIOstream <<
        "}" <<
        endl;
  } // switch
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrVarValAssoc& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrVarValAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (elt->getComment ().size ()) {
    fBrailleCodeIOstream <<
      "% " << elt->getComment () <<
      endl;
  }

  if (elt->getCommentedKind () == bsrVarValAssoc::kCommented) {
    fBrailleCodeIOstream << "\%";
  }
  
  switch (elt->getBackSlashKind ()) {
    case bsrVarValAssoc::kWithBackSlash:
      fBrailleCodeIOstream << "\\";
      break;
    case bsrVarValAssoc::kWithoutBackSlash:
      break;
  } // switch

  bsrVarValAssoc::bsrVarValAssocKind
    varValAssocKind =
      elt->getLilyPondVarValAssocKind ();
      
  string
    lilyPondVarValAssocKindAsBrailleString =
      bsrVarValAssocKindAsBrailleString (
        varValAssocKind);
      
  // the largest variable name length in a header is 18 JMI
  int fieldWidth;

  if (fOnGoingHeader) {
    fieldWidth = 18;
  }
  else {
    fieldWidth =
      lilyPondVarValAssocKindAsBrailleString.size ();
  }

  // generate the field name
  fBrailleCodeIOstream << left<<
    setw (fieldWidth) <<
    lilyPondVarValAssocKindAsBrailleString;

  if (elt->getVarValSeparator () == bsrVarValAssoc::kEqualSign) {
    fBrailleCodeIOstream << " = ";
  }
  else {
    fBrailleCodeIOstream << " ";
  }
  
  if (elt->getQuotesKind () == bsrVarValAssoc::kQuotesAroundValue) {
    fBrailleCodeIOstream << "\"";
  }

  // generate the value and unit if any
  if (elt->getUnit ().size ()) {
    fBrailleCodeIOstream <<
      setprecision (2) <<
      elt->getVariableValue ();

    fBrailleCodeIOstream <<
      "\\" <<
      elt->getUnit ();
  }
  else {
    fBrailleCodeIOstream <<
      elt->getVariableValue ();
  }
  
  if (elt->getQuotesKind () == bsrVarValAssoc::kQuotesAroundValue) {
    fBrailleCodeIOstream << "\"";
  }
  
  fBrailleCodeIOstream <<
  endl;

  switch (elt->getEndlKind ()) {
    case bsrVarValAssoc::kWithEndl:
      fBrailleCodeIOstream <<
        endl;
      break;
      
    case bsrVarValAssoc::kWithEndlTwice:
      fBrailleCodeIOstream <<
        endl <<
        endl;
      break;
      
    case bsrVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void bsr2BrailleTranslator::visitEnd (S_bsrVarValAssoc& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrVarValAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrVarValsListAssoc& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrVarValsListAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

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
    
  fBrailleCodeIOstream << left<<
    setw (fieldWidth) <<
    bsrVarValsListAssocKindAsBrailleString (
      elt->getVarValsListAssocKind ());
  
  fBrailleCodeIOstream << " = ";
      
  generateBsrVarValsListAssocValues (elt);

  fBrailleCodeIOstream <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_bsrVarValsListAssoc& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrVarValsListAssoc" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrSchemeVariable& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrSchemeVariable" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (elt->getComment ().size ()) {
    fBrailleCodeIOstream <<
      "% " << elt->getComment () <<
      endl;
  }

  if (elt->getCommentedKind () == bsrSchemeVariable::kCommented) {
    fBrailleCodeIOstream << "\% ";
  }
  
  fBrailleCodeIOstream <<
    "#(" <<
    elt->getVariableName () <<
    " " <<
    elt->getVariableValue () <<
    ")";
    
  switch (elt->getEndlKind ()) {
    case bsrSchemeVariable::kWithEndl:
      fBrailleCodeIOstream << endl;
      break;
    case bsrSchemeVariable::kWithEndlTwice:
      fBrailleCodeIOstream << endl << endl;
      break;
    case bsrSchemeVariable::kWithoutEndl:
      break;
  } // switch
}

void bsr2BrailleTranslator::visitEnd (S_bsrSchemeVariable& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrSchemeVariable" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrHeader& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrHeader" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\header" << " {" <<
    endl;

  gIndenter++;

  // generate header elements

  int fieldNameWidth =
    elt->maxBrailleVariablesNamesLength ();
    
  // MusicXML informations JMI ???
  
  // LilyPond informations

  {
    S_bsrVarValAssoc
      lilypondDedication =
        elt->getBrailleDedication ();
  
    if (lilypondDedication) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondDedication,
          fieldNameWidth) <<
        endl;
    }
  }

  {
    S_bsrVarValAssoc
      lilypondPiece =
        elt->getBraillePiece ();
  
    if (lilypondPiece) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondPiece,
          fieldNameWidth) <<
        endl;
    }
  }

  {
    S_bsrVarValAssoc
      lilypondOpus =
        elt->getBrailleOpus ();
  
    if (lilypondOpus) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondOpus,
          fieldNameWidth) <<
        endl;
    }
  }

  {
    S_bsrVarValAssoc
      lilypondTitle =
        elt->getBrailleTitle ();
  
    if (lilypondTitle) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondTitle,
          fieldNameWidth) <<
        endl;
    }
    
    S_bsrVarValAssoc
      lilypondSubTitle =
        elt->getBrailleSubTitle ();
  
    if (lilypondSubTitle) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondSubTitle,
          fieldNameWidth) <<
        endl;
    }
    
    S_bsrVarValAssoc
      lilypondSubSubTitle =
        elt->getBrailleSubSubTitle ();
  
    if (lilypondSubSubTitle) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondSubSubTitle,
          fieldNameWidth) <<
        endl;
    }
  }
  
  {
    S_bsrVarValAssoc
      lilypondInstrument =
        elt->getBrailleInstrument ();
  
    if (lilypondInstrument) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondInstrument,
          fieldNameWidth) <<
        endl;
    }
    
    S_bsrVarValAssoc
      lilypondMeter =
        elt->getBrailleMeter ();
  
    if (lilypondMeter) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondMeter,
          fieldNameWidth) <<
        endl;
    }
  }
    
  {
    S_bsrVarValAssoc
      lilypondCopyright =
        elt->getBrailleCopyright ();
  
    if (lilypondCopyright) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondCopyright,
          fieldNameWidth) <<
        endl;
    }
    
    S_bsrVarValAssoc
      lilypondTagline =
        elt->getBrailleTagline ();
  
    if (lilypondTagline) {
      fBrailleCodeIOstream <<
        bsrVarValAssocAsBrailleString (
          lilypondTagline,
          fieldNameWidth) <<
        endl;
    }
  }
    
  fOnGoingHeader = true;
}

void bsr2BrailleTranslator::visitEnd (S_bsrHeader& elt)
{
  fOnGoingHeader = false;

  gIndenter--;

  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrHeader" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPaper& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrPaper" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\paper" << " {" <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 20;

  // page width, height, margins and indents

  {
    float paperWidth =
      elt->getPaperWidth ();
      
    if (paperWidth > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "paper-width" << " = " <<
        setprecision (3) << paperWidth << "\\cm" <<
        endl;
    }
  }
  
  {
    float paperHeight =
      elt->getPaperHeight ();
      
    if (paperHeight > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "paper-height" << " = " <<
        setprecision (3) << paperHeight << "\\cm" <<
        endl;
    }
  }
  
  {
    float topMargin =
      elt->getTopMargin ();
      
    if (topMargin > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "top-margin" << " = " <<
        setprecision (3) << topMargin << "\\cm" <<
        endl;
    }
  }

  {
    float bottomMargin =
      elt->getBottomMargin ();
      
    if (bottomMargin > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "bottom-margin" << " = " <<
        setprecision (3) << bottomMargin << "\\cm" <<
        endl;
    }
  }

  {
    float leftMargin =
      elt->getLeftMargin ();
      
    if (leftMargin > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "left-margin" << " = " <<
        setprecision (3) << leftMargin << "\\cm" <<
        endl;
    }
  }

  {
    float rightMargin =
      elt->getRightMargin ();
      
    if (rightMargin > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "right-margin" << " = " <<
      setprecision (3) << rightMargin << "\\cm" <<
      endl;
    }
  }

  {
    float indent =
      elt->getIndent ();
      
    if (indent > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "indent" << " = " <<
      setprecision (3) << indent << "\\cm" <<
      endl;
    }
    else {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "%indent" << " = " <<
      setprecision (3) << 1.5 << "\\cm" <<
      endl;
    }
  }

  {
    float shortIndent =
      elt->getShortIndent ();
      
    if (shortIndent > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "short-indent" << " = " <<
      setprecision (3) << shortIndent << "\\cm" <<
      endl;
    }
    else {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "%short-indent" << " = " <<
      setprecision (3) << 0.0 << "\\cm" <<
      endl;
    }
  }

  // spaces

  {
    float betweenSystemSpace =
      elt->getBetweenSystemSpace ();
      
    if (betweenSystemSpace > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "between-system-space" << " = " <<
        setprecision (3) << betweenSystemSpace << "\\cm" <<
        endl;
    }
  }

  {
    float pageTopSpace =
      elt->getPageTopSpace ();
      
    if (pageTopSpace > 0) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "page-top-space" << " = " <<
        setprecision (3) << pageTopSpace << "\\cm" <<
        endl;
    }
  }

  // headers and footers

  {
    string oddHeaderMarkup =
      elt->getOddHeaderMarkup ();
      
    if (oddHeaderMarkup.size ()) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "oddHeaderMarkup" << " = " <<
        oddHeaderMarkup <<
        endl;
    }
  }

  {
    string evenHeaderMarkup =
      elt->getEvenHeaderMarkup ();
      
    if (evenHeaderMarkup.size ()) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "evenHeaderMarkup" << " = " <<
        evenHeaderMarkup <<
        endl;
    }
  }

  {
    string oddFooterMarkup =
      elt->getOddFooterMarkup ();
      
    if (oddFooterMarkup.size ()) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "oddFooterMarkup" << " = " <<
        oddFooterMarkup <<
        endl;
    }
  }

  {
    string evenFooterMarkup =
      elt->getEvenFooterMarkup ();
      
    if (evenFooterMarkup.size ()) {
      fBrailleCodeIOstream << left <<
        setw (fieldWidth) <<
        "evenFooterMarkup" << " = " <<
        evenFooterMarkup <<
        endl;
    }
  }

  fBrailleCodeIOstream <<
    endl;

  // generate a 'page-count' comment ready for the user
  fBrailleCodeIOstream << left <<
    setw (fieldWidth) <<
    "%" "page-count" << " = " <<
    setprecision (2) << 1 <<
    endl;

  // generate a 'system-count' comment ready for the user
  fBrailleCodeIOstream << left <<
    setw (fieldWidth) <<
    "%" "system-count" << " = " <<
    setprecision (2) << 1 <<
    endl;

  // fonts
  if (gBrailleOptions->fJazzFonts) {
    fBrailleCodeIOstream <<
R"(
  #(define fonts
     (set-global-fonts
      #:music "lilyjazz"
      #:brace "lilyjazz"
      #:roman "lilyjazz-text"
      #:sans "lilyjazz-chord"
      #:factor (/ staff-height pt 20)
      ))
)";
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrPaper& elt)
{
  gIndenter--;

  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrPaper" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
      
  fBrailleCodeIOstream <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrLayout& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\layout {" <<
    endl;

  gIndenter++;

  fBrailleCodeIOstream <<
    "\\context {" <<
    endl <<
    gTab << "\\Score" <<
    endl;

  if (gBrailleOptions->fCompressMultiMeasureRests) { // JMI
    fBrailleCodeIOstream <<
      gTab << "skipBars = ##t % to compress multiple measure rests" <<
      endl;
  }

  fBrailleCodeIOstream <<
    gTab << "autoBeaming = ##f % to display tuplets brackets" <<
    endl <<

    "}" <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_bsrLayout& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrLayout" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (gBrailleOptions->fRepeatBrackets) {
    fBrailleCodeIOstream <<
      "\\context " "{" <<
      endl;
    
    gIndenter++;

    fBrailleCodeIOstream <<
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
        
    fBrailleCodeIOstream <<
      "}" <<
      endl;
  }

  if (false) { // JMI XXL
    fBrailleCodeIOstream <<
      "\\context {" <<
      endl;
  
    gIndenter++;
  
    fBrailleCodeIOstream <<
      "\\Staff" <<
      endl <<
      "\\consists \"Span_arpeggio_engraver\"" <<
      endl;

    gIndenter--;

    fBrailleCodeIOstream <<
      "}" <<
      endl;
  }
    
  gIndenter--;

  fBrailleCodeIOstream <<
    "}" <<
    endl <<
    endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrScoreBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrScoreBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\score {" <<
    endl;

  gIndenter++;
/ * JMI
  if (elt->getScoreBlockElements ().size ()) {
    fBrailleCodeIOstream <<
      "<<" <<
      endl;
  
    gIndenter++;
  }
* /
  fOnGoingScoreBlock = true;
}

void bsr2BrailleTranslator::visitEnd (S_bsrScoreBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrScoreBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/ * JMI
  if (elt->getScoreBlockElements ().size ()) {
    gIndenter--;
    
    fBrailleCodeIOstream <<
      ">>" <<
      endl <<
      * endl;
  }
* /
  gIndenter--;
  
  fBrailleCodeIOstream <<
    "}" <<
    endl;

  fOnGoingScoreBlock = false;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrParallelMusicBLock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrParallelMusicBLock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  fNumberOfPartGroupBlocks =
    elt->
      getParallelMusicBLockPartGroupBlocks ().size ();
    
  if (fNumberOfPartGroupBlocks) {
    if (gBrailleOptions->fComments) {
      fBrailleCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "<<" <<
        "% parallel music";
    }
    
    else {
      fBrailleCodeIOstream <<
        "<<";
    }
    
    fBrailleCodeIOstream <<
      endl;
  
    gIndenter++;
  }

  fCurrentParallelMusicBLock = elt;
}

void bsr2BrailleTranslator::visitEnd (S_bsrParallelMusicBLock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrParallelMusicBLock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
    
  fBrailleCodeIOstream <<
    endl;

  if (fNumberOfPartGroupBlocks) {
    gIndenter--;
    
    if (gBrailleOptions->fComments) {
      fBrailleCodeIOstream << left <<
        setw (commentFieldWidth) <<
        ">>" <<
        "% parallel music";
    }

    else {
      fBrailleCodeIOstream <<
        ">>";
    }
    
    fBrailleCodeIOstream <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPartGroupBlock& elt)
{
  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();

  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrPartGroupBlock for '" <<
      partGroup->asShortString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fPartGroupBlocksCounter++;

  fNumberOfPartGroupBlockElements =
    elt -> getPartGroupBlockElements ().size ();
    
// JMI  fBrailleCodeIOstream << endl << endl << partGroup << endl << endl;

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
      if (gBrailleOptions->fComments) {
        fBrailleCodeIOstream << left <<
          setw (commentFieldWidth);
      }
      
      switch (partGroupSymbolKind) {
        case msrPartGroup::kPartGroupSymbolNone:
          break;
          
        case msrPartGroup::kPartGroupSymbolBrace: // JMI
          switch (partGroupBarlineKind) {
            case msrPartGroup::kPartGroupBarlineYes:
              fBrailleCodeIOstream <<
                "\\new PianoStaff";
              break;
            case msrPartGroup::kPartGroupBarlineNo:
              fBrailleCodeIOstream <<
                "\\new GrandStaff";
              break;
          } // switch
          break;
          
        case msrPartGroup::kPartGroupSymbolBracket:
          switch (partGroupBarlineKind) {
            case msrPartGroup::kPartGroupBarlineYes:
              fBrailleCodeIOstream <<
                "\\new StaffGroup";
              break;
            case msrPartGroup::kPartGroupBarlineNo:
              fBrailleCodeIOstream <<
                "\\new ChoirStaff";
              break;
          } // switch
          break;
          
        case msrPartGroup::kPartGroupSymbolLine:
          fBrailleCodeIOstream <<
            "\\new StaffGroup";
          break;
        
        case msrPartGroup::kPartGroupSymbolSquare:
          fBrailleCodeIOstream <<
            "\\new StaffGroup";
          break;
      } // switch

      // generate the '\with' block beginning
      // if the part group is not implicit
      switch (partGroup->getPartGroupImplicitKind ()) {
        case msrPartGroup::kPartGroupImplicitYes:
          break;
        case msrPartGroup::kPartGroupImplicitNo:
          if (partGroupName.size ()) {
            fBrailleCodeIOstream <<
              endl <<
              "\\new StaffGroup" <<
              endl <<
              "\\with {" <<
              endl;
          }
          break;
      } // switch

      gIndenter++;

      if (partGroupName.size ()) {
        fBrailleCodeIOstream <<
          "instrumentName = " <<
          nameAsBrailleString (partGroupName) <<
          endl;
      }
      if (partGroupAbbreviation.size ()) {
        fBrailleCodeIOstream <<
          "shortInstrumentName = " <<
          nameAsBrailleString (partGroupAbbreviation) <<
          endl;
      }
            
      switch (partGroupSymbolKind) {
        case msrPartGroup::kPartGroupSymbolNone:
          break;
          
        case msrPartGroup::kPartGroupSymbolBrace: // JMI
          / *
           *
           * check whether individual part have instrument names JMI
           * 
            if (partGroupInstrumentName.size ()) {
              fBrailleCodeIOstream << = "\\new PianoStaff";
            }
            else {
              fBrailleCodeIOstream << = "\\new GrandStaff";
            }
              * /
          break;
          
        case msrPartGroup::kPartGroupSymbolBracket:
          break;
          
        case msrPartGroup::kPartGroupSymbolLine:
          fBrailleCodeIOstream <<
            "%kPartGroupSymbolLine" <<
            endl <<
            "systemStartDelimiter = #'SystemStartBar" <<
            endl;
          break;
        
        case msrPartGroup::kPartGroupSymbolSquare:
          fBrailleCodeIOstream <<
            "%kPartGroupSymbolSquare" <<
            endl <<
            "systemStartDelimiter = #'SystemStartSquare" <<
            endl;
          break;
      } // switch

      gIndenter--;

      // generate the '\with' block ending      
      // if the part group is not implicit
      switch (partGroup->getPartGroupImplicitKind ()) {
        case msrPartGroup::kPartGroupImplicitYes:
          break;
        case msrPartGroup::kPartGroupImplicitNo:
          if (partGroupName.size ()) {
            fBrailleCodeIOstream <<
              endl <<
              "}" <<
              endl;
          }
          break;
      } // switch

      if (gBrailleOptions->fComments) {
        fBrailleCodeIOstream << left <<
          setw (commentFieldWidth) <<
          " <<" << "% part group " <<
          partGroup->getPartGroupCombinedName ();
      }
      else {
        fBrailleCodeIOstream <<
          " <<";
      }
          
      fBrailleCodeIOstream <<
        endl;
      break;
  } // switch

  if (partGroupInstrumentName.size ()) { // JMI
    fBrailleCodeIOstream <<
      "instrumentName = \"" <<
      partGroupInstrumentName <<
      "\"" <<
      endl;
  }
  
  if (gBrailleOptions->fConnectArpeggios) {
    fBrailleCodeIOstream <<
      "\\set PianoStaff.connectArpeggios = ##t" <<
      endl;
  }
       
  fBrailleCodeIOstream <<
    endl;

  if (elt->getPartGroupBlockElements ().size () > 1) {
    gIndenter++;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrPartGroupBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrPartGroupBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

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
      if (gBrailleOptions->fComments) {
        fBrailleCodeIOstream << left <<
          setw (commentFieldWidth) << ">>" <<      
          "% part group " <<
          partGroup->getPartGroupCombinedName ();
      }
      else {
        fBrailleCodeIOstream <<
          ">>";        
      }
          
      fBrailleCodeIOstream <<
        endl;

      if (fPartGroupBlocksCounter != fNumberOfPartGroupBlocks) {
        fBrailleCodeIOstream <<
          endl;
      }
      break;
  } // switch
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrPartBlock& elt)
{
  // fetch part block's part
  S_msrPart
    part =
      elt->getPart ();
      
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrPartBlock for '" <<
      part->asShortString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

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
        / *
      partInstrumentName =  // JMI
        part->getPartInstrumentName (),
      partInstrumentAbbreviation =  // JMI
        part->getPartInstrumentAbbreviation ();
        * /
  
    if (gBrailleOptions->fComments) {
      fBrailleCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "\\new PianoStaff" <<
        " % part " << part->getPartCombinedName ();
    }
    else {
      fBrailleCodeIOstream <<
        "\\new PianoStaff";      
    }
    fBrailleCodeIOstream <<
      endl;
    
    // generate the 'with' block beginning
    fBrailleCodeIOstream <<
      "\\with {" <<  
      endl;

    gIndenter++;

    if (partName.size ()) {
      fBrailleCodeIOstream <<
        "instrumentName = \"" <<
        partName <<
        "\"" <<
        endl;
    }
    
    if (partAbbreviation.size ()) {
      fBrailleCodeIOstream <<
        "shortInstrumentName = " <<
       nameAsBrailleString (partAbbreviation) <<
        endl;
    }
    
    if (gBrailleOptions->fConnectArpeggios) {
      fBrailleCodeIOstream <<
        "connectArpeggios = ##t" <<
        endl;
    }
       
    gIndenter--;
  
    // generate the 'with' block ending
    fBrailleCodeIOstream <<
      "}" <<  
      endl;

    fBrailleCodeIOstream <<
      "<<" <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrPartBlock& elt)
{
  // fetch current part block's part
  S_msrPart
    part =
      elt->getPart ();
      
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrPartBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (part->getPartStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave

 // JMI ???   gIndenter--;
  
    if (gBrailleOptions->fComments) {
      fBrailleCodeIOstream << left <<
        setw (commentFieldWidth) << ">>" <<    
        "% part " <<
        part->getPartCombinedName ();
    }
    else {
      fBrailleCodeIOstream <<
        ">>";
    }
    
    fBrailleCodeIOstream <<
      endl;

    if (fPartGroupBlockElementsCounter != fNumberOfPartGroupBlockElements) {
      fBrailleCodeIOstream <<
        endl;
    }
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrStaffBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrStaffBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fStaffBlocksCounter++;
  
  S_msrStaff
    staff =
      elt->getStaff ();

  // generate the staff context command
  switch (staff->getStaffKind ()) {
    case msrStaff::kRegularStaff:
      if (gBrailleOptions->fJianpu) {
        fBrailleCodeIOstream << "\\new JianpuStaff";
      }
      else {
        fBrailleCodeIOstream << "\\new Staff";
      }
      break;
      
    case msrStaff::kTablatureStaff:
      fBrailleCodeIOstream << "\\new TabStaff";
      break;
      
    case msrStaff::kHarmonyStaff:
      fBrailleCodeIOstream << "\\new kHarmonyStaff???";
      break;
      
    case msrStaff::kFiguredBassStaff:
      fBrailleCodeIOstream << "\\new FiguredBassStaff???";
      break;

    case msrStaff::kDrumStaff:
      fBrailleCodeIOstream << "\\new DrumStaff";
      break;
      
    case msrStaff::kRythmicStaff:
      fBrailleCodeIOstream << "\\new RhythmicStaff";
      break;
    } // switch

  fBrailleCodeIOstream <<
    " = \"" <<
    staff->getStaffName () <<
    "\"";

  fBrailleCodeIOstream <<
    endl;
    
  // generate the 'with' block beginning
  fBrailleCodeIOstream <<
    "\\with {" <<  
    endl;

  gIndenter++;
  
  // fetch part uplink
  S_msrPart
    staffPartUplink =
      staff->getStaffPartUplink ();

  // don't generate instrument names in the staves
  // if the containing part contains several of them
  if (staffPartUplink ->getPartStavesMap ().size () == 1) {
    // get the part uplink name to be used
    string partName =
      staffPartUplink->
        getPartNameDisplayText ();
  
    if (partName.size () == 0) {
      partName =
        staffPartUplink->
          getPartName ();
    }

    // generate the instrument name
    if (partName.size ()) {
      fBrailleCodeIOstream <<
        "instrumentName = ";
  
      // does the name contain hexadecimal end of lines?
      std::size_t found =
    // JMI    partName.find ("&#xd");
        partName.find ("\n");
  
      if (found == string::npos) {
        // no, escape quotes if any and generate the result
        fBrailleCodeIOstream <<
          "\"" <<
          escapeQuotes (partName) <<
          "\"" <<
          endl;
      }
  
      else {
        // yes, split the name into a chunks list
        // and generate a \markup{}
        fBrailleCodeIOstream <<
          endl <<
          generateMultilineName (
            partName) <<
          endl;
      }    
    }
  
    // get the part uplink abbreviation display text to be used
    string partAbbreviation =
      staffPartUplink->
        getPartAbbreviationDisplayText ();
  
    if (partAbbreviation.size () == 0) {
      partAbbreviation =
        staffPartUplink->
          getPartAbbreviation ();
    }
  
    if (partAbbreviation.size ()) {
      fBrailleCodeIOstream <<
        "shortInstrumentName = ";
        
      // does the name contain hexadecimal end of lines?
      std::size_t found =
        partAbbreviation.find ("&#xd");
  
      if (found == string::npos) {
        // no, merely generate the name
        fBrailleCodeIOstream <<
          nameAsBrailleString (partAbbreviation) <<
          endl;
      }
  
      else {
        // yes, split the name into a chunks list
        // and generate a \markup{}
        fBrailleCodeIOstream <<
          endl <<
          generateMultilineName (
            partAbbreviation) <<
          endl;
      }    
    }
  }

  gIndenter--;

  // generate the string tunings if any
  S_msrStaffDetails
    staffDetails =
      staff->getStaffStaffDetails ();

  if (staffDetails) {
    const list<S_msrStaffTuning>&
      staffTuningsList =
        staffDetails->getStaffTuningsList ();

    if (staffTuningsList.size ()) {
      fBrailleCodeIOstream <<
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
          
        fBrailleCodeIOstream <<
          msrQuarterTonesPitchKindAsString (
            gBsrOptions->
              fBsrQuarterTonesPitchesLanguageKind,
            staffTuning->
              getStaffTuningQuarterTonesPitchKind ()) <<
          absoluteOctaveAsBrailleString (
            staffTuning->getStaffTuningOctave ());
              
        if (++i == iEnd) break;
        
        fBrailleCodeIOstream << " ";
      } // for
        
      fBrailleCodeIOstream <<
        ">" <<
        endl;

      gIndenter--;

      // should letters be used for frets?
      switch (staffDetails->getShowFretsKind ()) {
        case msrStaffDetails::kShowFretsNumbers:
          break;
        case msrStaffDetails::kShowFretsLetters:
          fBrailleCodeIOstream <<
            "tablatureFormat = #fret-letter-tablature-format" <<
            endl;
          break;
      } // switch
    }
  }

  // generate the 'with' block ending
  fBrailleCodeIOstream <<
    "}" <<  
    endl;

  // generate the comment if relevant
  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "<<" <<
        " % staff \"" << staff->getStaffName () << "\"";
  }
  else {
    fBrailleCodeIOstream <<
      "<<";    
  }

  fBrailleCodeIOstream <<
    endl;
 
  if (gBrailleOptions->fJianpu) {
    fBrailleCodeIOstream <<
      " \\jianpuMusic" <<
      endl;
  }

  gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_bsrStaffBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrStaffBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;

  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) << ">>" <<    
      "% staff " <<
      elt->getStaff ()->getStaffName ();
  }
  else {
    fBrailleCodeIOstream <<
      ">>";
  }

  fBrailleCodeIOstream <<
    endl;

  if (fStaffBlocksCounter != fNumberOfStaffBlocksElements) {
    fBrailleCodeIOstream <<
      endl;
  }
}

/*
//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrNewStaffgroupBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrNewStaffgroupBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

   fBrailleCodeIOstream <<
     "\\new StaffGroup" << " " << "{" <<
      endl;

  gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_bsrNewStaffgroupBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrNewStaffgroupBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;

  fBrailleCodeIOstream <<
    " }" <<
    endl << endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrNewStaffBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrNewStaffBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_bsrNewStaffBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrNewStaffBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;
}
*/

/*
//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrUseVoiceCommand& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrUseVoiceCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  S_msrVoice
    voice = elt->getVoice ();

  S_msrStaff
    staff = voice-> getVoiceStaffUplink ();

  msrStaff::msrStaffKind
    staffKind = staff->getStaffKind ();
  
  string staffContextName;
  string voiceContextName;
  
  switch (staffKind) {
    case msrStaff::kRegularStaff:
      staffContextName = "\\context Staff";
      voiceContextName = "Voice";
      break;
      
    case msrStaff::kTablatureStaff:
      staffContextName = "\\context TabStaff";
      voiceContextName = "TabVoice";
      break;
      
    case msrStaff::kHarmonyStaff:
      staffContextName = "\\context ChordNames2"; // JMI
      voiceContextName = "???"; // JMI
      break;
      
    case msrStaff::kFiguredBassStaff:
      staffContextName = "\\context FiguredBass";
      voiceContextName = "???"; // JMI
      break;

    case msrStaff::kDrumStaff:
      staffContextName = "\\context DrumStaff";
      voiceContextName = "DrumVoice";
        // the "DrumVoice" alias exists, use it
      break;
      
    case msrStaff::kRythmicStaff:
      staffContextName = "\\context RhythmicStaff";
      voiceContextName = "Voice";
        // no "RhythmicVoice" alias exists
      break;
      
  } // switch

 // if (voice->getStaffRelativeVoiceNumber () > 0) { JMI    
    fBrailleCodeIOstream <<
      "\\context " << voiceContextName << " = " "\"" <<
      voice->getVoiceName () << "\"" << " <<" <<
       endl;
  
    gIndenter++;
  
    if (gBrailleOptions->fNoAutoBeaming) {
      fBrailleCodeIOstream <<
        "\\set " << staffContextName << ".autoBeaming = ##f" <<
        endl;
    }

    switch (staffKind) {
      case msrStaff::kRegularStaff:
        {
          int staffRegularVoicesCounter =
            staff->getStaffRegularVoicesCounter ();
            
          if (staffRegularVoicesCounter > 1) {
            switch (voice->getRegularVoiceStaffSequentialNumber ()) {
              case 1:
                fBrailleCodeIOstream << "\\voiceOne ";
                break;
              case 2:
                fBrailleCodeIOstream << "\\voiceTwo ";
                break;
              case 3:
                fBrailleCodeIOstream << "\\voiceThree ";
                break;
              case 4:
                fBrailleCodeIOstream << "\\voiceFour ";
                break;
              default:
                {}
            } // switch
    
            fBrailleCodeIOstream <<
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
  
    fBrailleCodeIOstream <<
      "\\" << voice->getVoiceName () << endl;
  
    gIndenter--;
    
    fBrailleCodeIOstream <<
      ">>" <<
      endl;
 // }
}

void bsr2BrailleTranslator::visitEnd (S_bsrUseVoiceCommand& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrUseVoiceCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
  
//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrNewLyricsBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrNewLyricsBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (! gBrailleOptions->fNoBrailleLyrics) {
    S_msrStanza stanza = elt->getStanza ();
    
    fBrailleCodeIOstream <<
      "\\new Lyrics" <<
      endl;

    gIndenter++;
    
    fBrailleCodeIOstream <<
      "\\with {" <<
      endl <<
      gTab << "associatedVoice = " <<
      "\""  << elt->getVoice ()->getVoiceName () << "\"" <<
      endl;

    if (gMsrOptions->fAddStanzasNumbers) {
      fBrailleCodeIOstream <<
        gTab << "stanza = \"" <<
        stanza->getStanzaNumber () <<
        ".\"" <<
        endl;
    }
    
    fBrailleCodeIOstream <<
      "}" <<
      endl <<

      "\\" << stanza->getStanzaName () <<
      endl;

    gIndenter--;
  }
}

void bsr2BrailleTranslator::visitEnd (S_bsrNewLyricsBlock& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrNewLyricsBlock" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (! gBrailleOptions->fNoBrailleLyrics) {
    // JMI
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrVariableUseCommand& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrVariableUseCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_bsrVariableUseCommand& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrVariableUseCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrChordNamesContext& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrChordNamesContext" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  string
    contextTypeKindAsString =
      elt->getContextTypeKindAsString (),
    contextName =
      elt->getContextName ();
    
  fBrailleCodeIOstream <<
    "\\context " << contextTypeKindAsString <<
    " = \"" << contextName << "\"" <<
    endl;

/ * JMI
  if (false) { //option JMI
    fBrailleCodeIOstream <<
      "\\with {" <<
      endl;

    gIndenter++;
    
    fBrailleCodeIOstream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      endl <<
      "\\consists \"Bar_engraver\"" <<
      endl;
        
    gIndenter--;
        
    fBrailleCodeIOstream <<
      "}" <<
      endl;
  }
      * /
        
  fBrailleCodeIOstream <<
    "\\" << contextName <<
    endl <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_bsrChordNamesContext& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrChordNamesContext" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
  
//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrFiguredBassContext& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrFiguredBassContext" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  string
    contextTypeKindAsString =
      elt->getContextTypeKindAsString (),
    contextName =
      elt->getContextName ();
    
  fBrailleCodeIOstream <<
    "\\context " << contextTypeKindAsString <<
    " = \"" << contextName << "\"" <<
    endl;

/ * JMI
  if (false) { //option JMI
    fBrailleCodeIOstream <<
      "\\with {" <<
      endl;

    gIndenter++;
    
    fBrailleCodeIOstream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      endl <<
      "\\consists \"Bar_engraver\"" <<
      endl;
        
    gIndenter--;
        
    fBrailleCodeIOstream <<
      "}" <<
      endl;
  }
  * /
        
  fBrailleCodeIOstream <<
    "\\" << contextName <<
    endl <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_bsrFiguredBassContext& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrFiguredBassContext" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
  
//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrBarCommand& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrBarCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_bsrBarCommand& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrBarCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;
}

//________________________________________________________________________
/* JMI
void bsr2BrailleTranslator::visitStart (S_bsrMelismaCommand& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrMelismaCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  switch (elt->getMelismaKind ()) {
    case bsrMelismaCommand::kMelismaStart:
// JMI      fBrailleCodeIOstream << "\\melisma ";
      break;
    case bsrMelismaCommand::kMelismaEnd:
// JMI      fBrailleCodeIOstream << "\\melismaEnd ";
      break;
  } // switch
}

void bsr2BrailleTranslator::visitEnd (S_bsrMelismaCommand& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrMelismaCommand" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
*/

/* 
//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrScore& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrScore& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrScore" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrCredit& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrCredit& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrCredit" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitStart (S_msrCreditWords& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrCreditWords& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrCreditWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrPartGroup& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrPartGroup" <<
      ", line " << elt->getInputLineNumber () <<
      elt->getPartGroupCombinedName () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrPartGroup& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrPartGroup" <<
      elt->getPartGroupCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrPart& elt)
{
  string
    partCombinedName =
      elt->getPartCombinedName ();
      
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrPart" <<
      partCombinedName <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceParts) {
    fLogOutputStream <<
      endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->" <<
      endl;
  }
#endif

  fCurrentPart = elt;

  fRemainingMultipleRestMeasuresNumber = 0; // JMI
  fOnGoingMultipleRestMeasures = false; // JMI
}

void bsr2BrailleTranslator::visitEnd (S_msrPart& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrPart" <<
      elt->getPartCombinedName () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentPart = nullptr;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrStaff& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  fOnGoingStaff = true;
}

void bsr2BrailleTranslator::visitEnd (S_msrStaff& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  fOnGoingStaff = false;
}

void bsr2BrailleTranslator::visitStart (S_msrStaffTuning& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> Start visiting msrStaffTuning" <<
      endl;
  }

/ * JMI
  list<S_msrStaffTuning>
    staffTuningsList =
      staff->getStaffTuningsList ();
      
  if (staffTuningsList.size ()) {
    // \set TabStaff.stringTunings = \stringTuning <c' g' d'' a''>

    fBrailleCodeIOstream <<
      "\\set TabStaff.stringTunings = \\stringTuning <";

    list<S_msrStaffTuning>::const_iterator
      iBegin = staffTuningsList.begin (),
      iEnd   = staffTuningsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fBrailleCodeIOstream <<
        msrQuarterTonesPitchAsString (
          gBsrOptions->fBsrQuarterTonesPitchesLanguage,
 // JMI            elt->getInputLineNumber (),
          ((*i)->getStaffTuningQuarterTonesPitch ())) <<        
 // JMI       char (tolower ((*i)->getStaffTuningStep ())) <<
        absoluteOctaveAsBrailleString (
          (*i)->getStaffTuningOctave ());
      if (++i == iEnd) break;
      fBrailleCodeIOstream << " ";
    } // for

    fBrailleCodeIOstream <<
      ">" <<
      endl;
  }
 * /
}

void bsr2BrailleTranslator::visitStart (S_msrStaffDetails& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> Start visiting msrStaffDetails" <<
      endl;
  }

  // fetch staff lines number
  int
    staffLinesNumber =
      elt->getStaffLinesNumber ();

  if (staffLinesNumber != 5) { // handle current staff lines number??? JMI
    fBrailleCodeIOstream <<
      endl <<
      "\\stopStaff " <<
      endl <<
      "\\override Staff.StaffSymbol.line-count = " <<
      staffLinesNumber <<
      endl <<
      "\\startStaff" <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrVoice& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentVoice = elt;
  
  fBrailleCodeIOstream <<
    fCurrentVoice->getVoiceName () <<
      " = ";

  // generate the beginning of the voice definition
  switch (fCurrentVoice->getVoiceKind ()) {
    
    case msrVoice::kRegularVoice:
      if (gBrailleOptions->fAbsoluteOctaves) {
        fBrailleCodeIOstream <<
          "{" <<
          endl;
      }
      else {
        fBrailleCodeIOstream <<
          "\\relative {" <<
          endl;
      }
      break;
      
    case msrVoice::kHarmonyVoice:
      fBrailleCodeIOstream <<
        "\\chordmode {" <<
        endl;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fBrailleCodeIOstream <<
        "\\figuremode {" <<
        endl;
      break;
  } // switch

  gIndenter++;

  if (gBrailleOptions->fGlobal) {
    fBrailleCodeIOstream <<
      "\\global" <<
      endl <<
      endl;
  }
    
  if (gBrailleOptions->fDisplayMusic) {
    fBrailleCodeIOstream <<
      "\\displayMusic {" <<
      endl;

    gIndenter++;
  }
    
  fBrailleCodeIOstream <<
    "\\language \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      gBsrOptions->
        fBsrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl;

  if (gBsrOptions->fBsrChordsLanguageKind != k_IgnatzekChords) {
    fBrailleCodeIOstream <<
      "\\" <<
      bsrChordsLanguageKindAsString (
        gBsrOptions->
          fBsrChordsLanguageKind) <<
      "Chords" <<
      endl;
  }

  if (gBrailleOptions->fShowAllBarNumbers) {
    fBrailleCodeIOstream <<
      "\\set Score.barNumberVisibility = #all-bar-numbers-visible" <<
      endl <<
      "\\override Score.BarNumber.break-visibility = ##(#f #t #t)" <<
      endl <<
      endl;
  }

// JMI   \set Score.alternativeNumberingStyle = #'numbers-with-letters


  if (
/ *
    fCurrentVoice->getVoiceContainsMultipleRests ()
      ||
* /
    gBrailleOptions->fCompressMultiMeasureRests) {
    fBrailleCodeIOstream <<
      "\\compressMMRests {" <<
      endl;

    gIndenter++;
  }

  if (gBrailleOptions->fAccidentalStyleKind != kDefaultStyle) {
    fBrailleCodeIOstream <<
      "\\accidentalStyle Score." <<
      bsrAccidentalStyleKindAsString (
        gBrailleOptions->fAccidentalStyleKind) <<
      endl <<
      endl;
  }

  fRelativeOctaveReference = nullptr;

  fVoiceIsCurrentlySenzaMisura = false;
  
  fOnGoingVoice = true;

  switch (fCurrentVoice->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      break;
      
    case msrVoice::kHarmonyVoice:
      fOnGoingHarmonyVoice = true;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fOnGoingFiguredBassVoice = true;
      break;
  } // switch

  // reset current voice measures counter
  fCurrentVoiceMeasuresCounter = 0; // none have been met
  
  // force durations to be displayed explicitly
  // at the beginning of the voice
  fLastMetWholeNotes = rational (0, 1);
}

void bsr2BrailleTranslator::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  if (
  / * JMI
    fCurrentVoice->getVoiceContainsMultipleRests ()
      ||
      * /
    gBrailleOptions->fCompressMultiMeasureRests) {
    fBrailleCodeIOstream <<
      "}" <<
      endl;

    gIndenter--;
  }

  if (gBrailleOptions->fDisplayMusic) {
    fBrailleCodeIOstream <<
      "}" <<
      endl;

    gIndenter--;
  }
    
  // generate the end of the voice definition
  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      fBrailleCodeIOstream <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kHarmonyVoice:
      fBrailleCodeIOstream <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fBrailleCodeIOstream <<
        "}" <<
        endl <<
        endl;
      break;
  } // switch

  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      break;
      
    case msrVoice::kHarmonyVoice:
      fOnGoingHarmonyVoice = false;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fOnGoingFiguredBassVoice = false;
      break;
  } // switch

  fCurrentVoice = nullptr;
  fOnGoingVoice = false;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    endl <<
    "\\change Staff=\"" <<
    elt->getStaffToChangeTo ()->getStaffName () <<
    "\"";

  if (gBrailleOptions->fNoteInputLineNumbers) { // JMI
    // print the staff change line number as a comment
    fBrailleCodeIOstream <<
      "%{ " << elt->getInputLineNumber () << " %} ";
  }

  fBrailleCodeIOstream <<
    endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrHarmony& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (fOnGoingNote) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceHarmonies) {
      fBrailleCodeIOstream <<
        "%{ " << elt->asString () << " %}" <<
        endl;
    }
#endif
  }
  
  else if (fOnGoingChord) { // JMI
    / *
    // register the harmony in the current chord clone
    fCurrentChord->
      setChordHarmony (elt); // JMI
      * /
  }

  else if (fOnGoingHarmonyVoice) {
    fBrailleCodeIOstream <<
      harmonyAsBrailleString (elt) <<
      " ";
      
    if (gBrailleOptions->fNoteInputLineNumbers) {
      // print the harmony line number as a comment
      fBrailleCodeIOstream <<
        "%{ " << elt->getInputLineNumber () << " %} ";
    }
  }

/ * JMI
  else if (fOnGoingChord) {
    // don't generate code for the code harmony,
    // this will be done after the chord itself JMI
  }
  * /
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrFrame& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  if (fOnGoingNote) {
#ifdef TRACE_OPTIONS
    if (gTraceOptions->fTraceFrames) {
      fBrailleCodeIOstream <<
        "%{ " << elt->asString () << " %}" <<
        endl;
    }
#endif

    fBrailleCodeIOstream <<
      frameAsBrailleString (elt) <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrFiguredBass& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentFiguredBass = elt;
  
  if (fOnGoingNote) {
    fBrailleCodeIOstream <<
      "%{ " << fCurrentFiguredBass->asString () << " %}" <<
      endl;
  }
  
  else if (fOnGoingChord) { // JMI
    / *
    // register the figured bass in the current chord clone
    fCurrentChord->
      setChordFiguredBass (elt); // JMI
      * /
  }

  else if (fOnGoingFiguredBassVoice) {
    fBrailleCodeIOstream <<
      "<";
      
    if (gBrailleOptions->fNoteInputLineNumbers) {
      // print the figured bass line number as a comment
      fBrailleCodeIOstream <<
        " %{ " << fCurrentFiguredBass->getInputLineNumber () << " %} ";
    }
  }

/ * JMI
  else if (fOnGoingChord) {
    // don't generate code for the code figured bass,
    // this will be done after the chord itself JMI
  }
  * /

  fCurrentFiguredBassFiguresCounter = 0;
}

void bsr2BrailleTranslator::visitStart (S_msrFigure& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrFigure '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fCurrentFiguredBassFiguresCounter++;
  
  // is the figured bass parenthesized?
  msrFiguredBass::msrFiguredBassParenthesesKind
    figuredBassParenthesesKind =
      fCurrentFiguredBass->
        getFiguredBassParenthesesKind ();
        
  // generate the figure number
  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      fBrailleCodeIOstream << "[";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      break;
  } // switch

  fBrailleCodeIOstream <<
    elt->getFigureNumber ();
    
  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      fBrailleCodeIOstream << "]";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      break;
  } // switch

  // handle the figure prefix
  switch (elt->getFigurePrefixKind ()) {
    case msrFigure::k_NoFigurePrefix:
      break;
    case msrFigure::kDoubleFlatPrefix:
      fBrailleCodeIOstream << "--";
      break;
    case msrFigure::kFlatPrefix:
      fBrailleCodeIOstream << "-";
      break;
    case msrFigure::kFlatFlatPrefix:
      fBrailleCodeIOstream << "flat flat";
      break;
    case msrFigure::kNaturalPrefix:
      fBrailleCodeIOstream << "!";
      break;
    case msrFigure::kSharpSharpPrefix:
      fBrailleCodeIOstream << "sharp sharp";
      break;
    case msrFigure::kSharpPrefix:
      fBrailleCodeIOstream << "+";
      break;
    case msrFigure::kDoubleSharpPrefix:
      fBrailleCodeIOstream << "++";
      break;
  } // switch

  // handle the figure suffix
  switch (elt->getFigureSuffixKind ()) {
    case msrFigure::k_NoFigureSuffix:
      break;
    case msrFigure::kDoubleFlatSuffix:
      fBrailleCodeIOstream << "double flat";
      break;
    case msrFigure::kFlatSuffix:
      fBrailleCodeIOstream << "flat";
      break;
    case msrFigure::kFlatFlatSuffix:
      fBrailleCodeIOstream << "flat flat";
      break;
    case msrFigure::kNaturalSuffix:
      fBrailleCodeIOstream << "natural";
      break;
    case msrFigure::kSharpSharpSuffix:
      fBrailleCodeIOstream << "sharp sharp";
      break;
    case msrFigure::kSharpSuffix:
      fBrailleCodeIOstream << "sharp";
      break;
    case msrFigure::kDoubleSharpSuffix:
      fBrailleCodeIOstream << "souble sharp";
      break;
    case msrFigure::kSlashSuffix:
      fBrailleCodeIOstream << "/";
      break;
  } // switch

  // generate a space if not last figure in figured bass
  if (
    fCurrentFiguredBassFiguresCounter
      <
    fCurrentFiguredBass->getFiguredBassFiguresList ().size ()) {
    fBrailleCodeIOstream << " ";
  }
  }

void bsr2BrailleTranslator::visitEnd (S_msrFiguredBass& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    ">" <<
    wholeNotesAsBrailleString (
      elt->getInputLineNumber (),
      elt->
        getFiguredBassSoundingWholeNotes ()) <<
    " ";
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrSegment& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;
  }

  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% start of segment" <<
      endl;

    gIndenter++;
  }

  fSegmentNotesAndChordsCountersStack.push (0);
}

void bsr2BrailleTranslator::visitEnd (S_msrSegment& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;
  }

  if (gBrailleOptions->fComments) {
    gIndenter--;
    
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% end of segment" <<
      endl;
  }
  / * JMI
  else
    fBrailleCodeIOstream <<
      endl;
* /

  fSegmentNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrMeasure& elt)
{    
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    fLogOutputStream <<
      endl <<
      "% <!--=== measure '" << measureNumber <<
      "', line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // take this measure into account for counting
  switch (elt->getMeasureKind ()) {
    case msrMeasure::kUnknownMeasureKind:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kFullMeasureKind:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kUpbeatMeasureKind:
      // keep fCurrentVoiceMeasuresCounter at 0
      break;
    case msrMeasure::kUnderfullMeasureKind:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kOverfullMeasureKind:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kSenzaMisuraMeasureKind:
      fCurrentVoiceMeasuresCounter++;
      break;
    case msrMeasure::kEmptyMeasureKind:
      fCurrentVoiceMeasuresCounter++;
      break;
  } // switch
  
  // force durations to be displayed explicitly
  // at the beginning of the measure
  fLastMetWholeNotes = rational (0, 1);

  // get measure kind
  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();
      
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrMeasure " <<
      measureNumber <<
      ", measureKind:" <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% start of measure " << measureNumber <<
      ", line " << inputLineNumber <<
      endl;

    gIndenter++;
  }

  // is this the end of a cadenza?
  if (
    fOnGoingVoiceCadenza
      &&
    measureKind != msrMeasure::kOverfullMeasureKind) {
    fBrailleCodeIOstream <<
      endl <<
      "\\cadenzaOff" <<
 // JMI     " \\undo \\omit Staff.TimeSignature" <<
      endl <<
      "\\bar \"|\" "; // JMI ???

    if (gBrailleOptions->fComments) {
      fBrailleCodeIOstream <<
        " % kOverfullMeasureKind End";
    }

    fBrailleCodeIOstream<<
      endl;

    fOnGoingVoiceCadenza = false;
  }

  fSegmentNotesAndChordsCountersStack.push (0);

  switch (measureKind) {
    case msrMeasure::kUnknownMeasureKind:
      {
        stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' is of unknown kind";

if (false) // JMI
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
else
        msrInternalWarning (
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          s.str ());
      }
      break;
      
    case msrMeasure::kFullMeasureKind:
      break;
      
    case msrMeasure::kUpbeatMeasureKind:
      {
        string
          upbeatDuration =
            wholeNotesAsBrailleString (
              inputLineNumber,
              elt->getMeasureLength ());

        // only generate '\partial' at the beginning of a voice
        switch (elt->getMeasureCreatedForARepeatKind ()) {
          case msrMeasure::kMeasureCreatedForARepeatNo:
            fBrailleCodeIOstream <<
              "\\partial " << upbeatDuration <<
              endl;
            break;
  
          case msrMeasure::kMeasureCreatedForARepeatBefore:
          case msrMeasure::kMeasureCreatedForARepeatAfter:
          case msrMeasure::kMeasureCreatedForARepeatPadded:
            // such a measure does not need a \partial
            break;
        } // switch
      }
      break;
      
    case msrMeasure::kUnderfullMeasureKind:
      {
        rational
          measureLength =
            elt->getMeasureLength ();

        rational
          measureFullLength =
            elt->getMeasureFullLength ();

        // we should set the score measure length in this case
        rational
          ratioToFullLength =
            measureLength / measureFullLength;
        ratioToFullLength.rationalise ();
  
#ifdef TRACE_OPTIONS
        if (gTraceOptions->fTraceMeasuresDetails) {
          const int fieldWidth = 27;
          
          fBrailleCodeIOstream << left <<
            "% Setting the measure length for measure " <<
            setw (fieldWidth) <<
            measureNumber <<
            ", line = " << inputLineNumber <<
            endl <<
            setw (fieldWidth) <<
            "% measureLength" << " = " << measureLength <<
            endl <<
            setw (fieldWidth) <<
            "% measureFullLength" << " = " << measureFullLength <<
            endl <<
            setw (fieldWidth) <<
            "% ratioToFullLength" << " = " << ratioToFullLength <<
            endl <<
            endl;
        }
#endif

        if (ratioToFullLength == rational (1, 1)) {
          stringstream s;
  
          s <<
            "underfull measure '" << measureNumber <<
            "' is actually the full measure length";
            
     // JMI       msrInternalError (
          msrInternalWarning (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
    // JMI        __FILE__, __LINE__,
            s.str ());
        }

        else {
          / * JMI 
          fBrailleCodeIOstream <<
            "\\set Score.measureLength = #(ly:make-moment " <<
            measureLength.toString () <<
            ")" <<
            endl;
    * /
    
          // should we generate a break?
          if (gBrailleOptions->fBreakLinesAtIncompleteRightMeasures) {
            fBrailleCodeIOstream <<
              "\\break" <<
              endl;
          }
        }
      }
      break;

    case msrMeasure::kOverfullMeasureKind:
      if (! fOnGoingVoiceCadenza) {
        fBrailleCodeIOstream <<
          endl <<
          "\\cadenzaOn" <<
          " \\omit Staff.TimeSignature";

        if (gBrailleOptions->fComments) {
          fBrailleCodeIOstream << " % kOverfullMeasureKind Start";
        }

        fBrailleCodeIOstream <<
          endl;

        fOnGoingVoiceCadenza = true;
      }
      break;
      
    case msrMeasure::kSenzaMisuraMeasureKind:
      if (! fOnGoingVoiceCadenza) {
        fBrailleCodeIOstream <<
          endl <<
          "\\cadenzaOn";

        if (gBrailleOptions->fComments) {
          fBrailleCodeIOstream << " % kSenzaMisuraMeasureKind Start";
        }

        fBrailleCodeIOstream <<
          endl;

        fBrailleCodeIOstream <<
          "\\once\\omit Staff.TimeSignature" <<
          endl;

        fOnGoingVoiceCadenza = true;
      }
      break;
      
    case msrMeasure::kEmptyMeasureKind:
      {
        // generate a rest the duration of the measure
        // followed by a bar check
        fBrailleCodeIOstream <<
          "R" <<
          wholeNotesAsBrailleString (
            inputLineNumber,
            elt->
              getMeasureFullLength ()) <<
          " | " <<
          endl;
      }
      break;
  } // switch
}

void bsr2BrailleTranslator::visitEnd (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureNumber ();
  string
    nextMeasureNumber =
      elt->getNextMeasureNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures) {
    fLogOutputStream <<
      endl <<
      "% <!--=== measure '" << measureNumber <<
      ", nextMeasureNumber = '" << nextMeasureNumber << "'" <<
      ", fOnGoingMultipleRestMeasures = '" <<
      booleanAsString (
        fOnGoingMultipleRestMeasures) <<
      "'" <<
      "', line " << inputLineNumber << " ===-->" <<
      endl;
  }
#endif

  // get measure kind
  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();
      
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrMeasure " <<
      measureNumber <<
      ", measureKind:" <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", line " << inputLineNumber <<
      endl;
  }

 // JMI if (! fOnGoingMultipleRestMeasures) {}
  {
    // handle the measure
    switch (measureKind) {
      case msrMeasure::kUnknownMeasureKind:
        break;
        
      case msrMeasure::kFullMeasureKind:
        {
          bool doGenerateBarCheck = true;
          
          if (fRemainingMultipleRestMeasuresNumber > 0) {
            // account for this measure
            fRemainingMultipleRestMeasuresNumber--;
  
            // the bar check will be generated upon visitEnd (S_msrMultipleRest&)
            doGenerateBarCheck = false;
          }
  
          if (doGenerateBarCheck) {
            fBrailleCodeIOstream <<
              "|";
      
            if (nextMeasureNumber.size ()) {
              fBrailleCodeIOstream <<
                " % " <<
                nextMeasureNumber;
            }
            
            fBrailleCodeIOstream <<
              endl;
          }
        }
        break;
        
      case msrMeasure::kUpbeatMeasureKind:
        fBrailleCodeIOstream <<
          "|";
  
        if (nextMeasureNumber.size ()) {
          fBrailleCodeIOstream <<
            " % " <<
            nextMeasureNumber;
        }
        
        fBrailleCodeIOstream <<
          endl;
        break;
        
      case msrMeasure::kUnderfullMeasureKind:
      / * JMI
        fBrailleCodeIOstream <<
          endl <<
          "\\unset Score.measureLength" <<
          endl;
          * /
        break;
  
      case msrMeasure::kOverfullMeasureKind:
        fBrailleCodeIOstream <<
          endl <<
          "\\cadenzaOff" <<
          " \\undo \\omit Staff.TimeSignature |" <<
          endl;
  
        fOnGoingVoiceCadenza = false;
        break;
  
      case msrMeasure::kSenzaMisuraMeasureKind:
        fBrailleCodeIOstream <<
          endl <<
          "\\cadenzaOff" <<
          endl <<
          "\\bar \"|\"" << // JMI ???
          endl;
  
        fOnGoingVoiceCadenza = false;
        break;
  
      case msrMeasure::kEmptyMeasureKind:
        break;
    } // switch
      
    if (gBrailleOptions->fComments) {
      gIndenter--;
  
      fBrailleCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "% end of measure " <<
        measureNumber <<
        ", line " << inputLineNumber <<
        endl <<
        endl;      
    }
  
    if (gBrailleOptions->fSeparatorLineEveryNMeasures > 0) {
      if (
        fCurrentVoiceMeasuresCounter
          %
        gBrailleOptions->fSeparatorLineEveryNMeasures
          ==
        0)
        fBrailleCodeIOstream <<
          endl <<
          "% ============================= " <<
          endl <<
          endl;
    }
    
    fSegmentNotesAndChordsCountersStack.pop ();
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrStanza& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (! gBrailleOptions->fNoBrailleLyrics) {
    // don't generate code for the stanza inside the code for the voice
    fGenerateCodeForOngoingNonEmptyStanza =
      ! fOnGoingVoice
        &&
      elt->getStanzaTextPresent ();

    if (fGenerateCodeForOngoingNonEmptyStanza) {
      fBrailleCodeIOstream <<
        elt->getStanzaName () << " = " << "\\lyricmode {" <<
        endl;
        
      gIndenter++;
    }
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrStanza& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (! gBrailleOptions->fNoBrailleLyrics) {
    if (fGenerateCodeForOngoingNonEmptyStanza) {
      gIndenter--;
    
      fBrailleCodeIOstream <<
        endl <<
        "}" <<
        endl <<
        endl;
    }

    fGenerateCodeForOngoingNonEmptyStanza = false;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrSyllable& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrSyllable '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (! gBrailleOptions->fNoBrailleLyrics) {
    if (fGenerateCodeForOngoingNonEmptyStanza) {
      switch (elt->getSyllableKind ()) {
        case msrSyllable::kSyllableSingle:
          writeTextsListAsBrailleString (
            elt->getSyllableTextsList (),
            fBrailleCodeIOstream);
            
          fBrailleCodeIOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableSingle %} ";
          }
#endif
          break;
          
        case msrSyllable::kSyllableBegin:
          writeTextsListAsBrailleString (
            elt->getSyllableTextsList (),
            fBrailleCodeIOstream);
            
          fBrailleCodeIOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            " -- ";
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableBegin %} ";
          }
#endif
          break;
          
        case msrSyllable::kSyllableMiddle:
          writeTextsListAsBrailleString (
            elt->getSyllableTextsList (),
            fBrailleCodeIOstream);
            
          fBrailleCodeIOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            " -- ";
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableMiddle %} ";
          }
#endif
          break;
          
        case msrSyllable::kSyllableEnd:
          writeTextsListAsBrailleString (
            elt->getSyllableTextsList (),
            fBrailleCodeIOstream);
            
          fBrailleCodeIOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableEnd %} ";
          }
#endif
          break;
                  
        case msrSyllable::kSyllableSkip:
          // LilyPond ignores the skip duration
          // when \lyricsto is used
          fBrailleCodeIOstream <<
            "\\skip" <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableSkip %} ";
          }
#endif
          break;
           
        case msrSyllable::kSyllableMeasureEnd:
      // JMI      "| " <<
          if (gBrailleOptions->fNoteInputLineNumbers) {
            // print the measure end line number as a comment
            fBrailleCodeIOstream <<
              "%{ measure end, line " <<
              elt->getInputLineNumber () <<
              " %}";
          }

          fBrailleCodeIOstream <<
            endl;
          break;
    
        case msrSyllable::kSyllableLineBreak:
          if (gBrailleOptions->fNoteInputLineNumbers) {
            // print the measure end line number as a comment
            fBrailleCodeIOstream <<
              "%{ line break, line " <<
              elt->getInputLineNumber () <<
              " %}";
          }

          fBrailleCodeIOstream <<
            endl;
          break;
    
        case msrSyllable::kSyllablePageBreak:
          if (gBrailleOptions->fNoteInputLineNumbers) {
            // print the measure end line number as a comment
            fBrailleCodeIOstream <<
              "%{ page break, line " <<
              elt->getInputLineNumber () <<
              " %}";
          }

          fBrailleCodeIOstream <<
            endl;
          break;
    
        case msrSyllable::kSyllableNone: // JMI
          break;
      } // switch

      switch (elt->getSyllableExtendKind ()) {
        case msrSyllable::kSyllableExtendSingle:
          // generate a lyric extender, i.e. a melisma,
          // after this syllable
          fBrailleCodeIOstream <<
            "__ ";
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableExtendSingle %} ";
          }
#endif
          break;
          
        case msrSyllable::kSyllableExtendStart:
          // generate a lyric extender, i.e. a melisma,
          // after this syllable
          fBrailleCodeIOstream <<
            "__ ";
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableExtendStart %} ";
          }
#endif
          break;
          
        case msrSyllable::kSyllableExtendContinue:
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableExtendContinue %} ";
          }
#endif
          break;
          
        case msrSyllable::kSyllableExtendStop:
#ifdef TRACE_OPTIONS
          if (gTraceOptions->fTraceLyrics) {
            fBrailleCodeIOstream <<
              "%{ kSyllableExtendStop %} ";
          }
#endif
          break;
          
        case msrSyllable::kSyllableExtendNone:
          break;
      } // switch
      
      if (gBrailleOptions->fNoteInputLineNumbers) {
        // print the note line number as a comment
        fBrailleCodeIOstream <<
          "%{ " << elt->getInputLineNumber () << " %} ";
      } 
    }
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrSyllable& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrSyllable '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrClef& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  msrClef::msrClefKind
    clefKind =
      elt->getClefKind ();

  if (clefKind != msrClef::k_NoClef) {
    fBrailleCodeIOstream <<
      "\\clef \"";
  
    switch (clefKind) {
      case msrClef::k_NoClef:
        break;
      case msrClef::kTrebleClef:
        fBrailleCodeIOstream << "treble";
        break;
      case msrClef::kSopranoClef:
        fBrailleCodeIOstream << "soprano";
        break;
      case msrClef::kMezzoSopranoClef:
        fBrailleCodeIOstream << "mezzosoprano";
        break;
      case msrClef::kAltoClef:
        fBrailleCodeIOstream << "alto";
        break;
      case msrClef::kTenorClef:
        fBrailleCodeIOstream << "tenor";
        break;
      case msrClef::kBaritoneClef:
        fBrailleCodeIOstream << "baritone";
        break;
      case msrClef::kBassClef:
        fBrailleCodeIOstream << "bass";
        break;
      case msrClef::kTrebleLine1Clef:
        fBrailleCodeIOstream << "french";
        break;
      case msrClef::kTrebleMinus15Clef:
        fBrailleCodeIOstream << "treble_15";
        break;
      case msrClef::kTrebleMinus8Clef:
        fBrailleCodeIOstream << "treble_8";
        break;
      case msrClef::kTreblePlus8Clef:
        fBrailleCodeIOstream << "treble^8";
        break;
      case msrClef::kTreblePlus15Clef:
        fBrailleCodeIOstream << "treble^15";
        break;
      case msrClef::kBassMinus15Clef:
        fBrailleCodeIOstream << "bass_15";
        break;
      case msrClef::kBassMinus8Clef:
        fBrailleCodeIOstream << "bass_8";
        break;
      case msrClef::kBassPlus8Clef:
        fBrailleCodeIOstream << "bass^8";
        break;
      case msrClef::kBassPlus15Clef:
        fBrailleCodeIOstream << "bass^15";
        break;
      case msrClef::kVarbaritoneClef:
        fBrailleCodeIOstream << "varbaritone";
        break;
      case msrClef::kTablature4Clef:
      case msrClef::kTablature5Clef:
      case msrClef::kTablature6Clef:
      case msrClef::kTablature7Clef:
        fBrailleCodeIOstream << "tab";
        / * JMI ???
        if (gBrailleOptions->fModernTab) {
          fBrailleCodeIOstream <<
            "\"moderntab\"" <<
            endl <<
            "\\tabFullNotation";
        }
        else {
          fBrailleCodeIOstream <<
            "\"tab\"" <<
            endl <<
            "\\tabFullNotation";
        }
            * /
        break;
      case msrClef::kPercussionClef:
        fBrailleCodeIOstream << "percussion";
        break;
      case msrClef::kJianpuClef:
        fBrailleCodeIOstream << "%{jianpuClef???%}";
        break;
    } // switch
  
  fBrailleCodeIOstream <<
    "\"" <<
    endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrClef& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrClef '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrKey& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrKey '" <<
      elt->asString () <<
      "'" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  switch (elt->getKeyKind ()) {
    case msrKey::kTraditionalKind:
      fBrailleCodeIOstream <<
        "\\key " <<
        msrQuarterTonesPitchKindAsString (
          gBsrOptions->fBsrQuarterTonesPitchesLanguageKind,
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
          fBrailleCodeIOstream <<
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
    
                                      
                fBrailleCodeIOstream <<
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
                  alterationKindAsBrailleString (
                    item->getKeyItemAlterationKind ()) <<                
                  ")";
            }
  
            else {
              // Alternatively, for each item in the list, using the more concise format (step . alter) specifies that the same alteration should hold in all octaves.
                                      
                fBrailleCodeIOstream <<
                  "(" <<
                  item->getKeyItemDiatonicPitchKind () <<
                  " . ," <<
                  alterationKindAsBrailleString (
                    item->getKeyItemAlterationKind ()) <<                
                  ")";
            }
                
            if (++i == iEnd) break;
            
            fBrailleCodeIOstream << " ";
          } // for
  
          fBrailleCodeIOstream <<
            ")";
        }
        
        else {
            msrInternalError (
              gXml2lyOptions->fInputSourceName,
              elt->getInputLineNumber (),
              __FILE__, __LINE__,
              "Humdrum/Scot key items vector is empty");
        }
      }
      break;
  } // switch
}

void bsr2BrailleTranslator::visitEnd (S_msrKey& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrKey '" <<
      elt->asString () <<
      "'"  <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTime& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  msrTime::msrTimeSymbolKind
    timeSymbolKind =
      elt->getTimeSymbolKind ();
      
  const vector<S_msrTimeItem>&
    timeItemsVector =
      elt->getTimeItemsVector ();

/ * JMI
  // is this the end of a senza misura fragment?
  if (
    fVoiceIsCurrentlySenzaMisura
      &&
    timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
    fBrailleCodeIOstream <<
      "\\undo\\omit Staff.TimeSignature" <<
      endl;

    fVoiceIsCurrentlySenzaMisura = false;
  }
* /

  // handle the time
  if (timeSymbolKind == msrTime::kTimeSymbolSenzaMisura) {
    // senza misura time

    / * JMI
    fBrailleCodeIOstream <<
      "\\omit Staff.TimeSignature" <<
      endl;
* /

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
          fBrailleCodeIOstream <<
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
          gBrailleOptions->fNumericalTime) {
          fBrailleCodeIOstream <<
            "\\numericTimeSignature ";
        }
  
        fBrailleCodeIOstream <<
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
    
        fBrailleCodeIOstream <<
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
          fBrailleCodeIOstream <<
            "(";
  
          // then generate all beats numbers in the vector
          for (int j = 0; j < beatsNumbersNumber; j++) {
            fBrailleCodeIOstream <<
              beatsNumbersVector [j] <<
              " ";
          } // for
      
          // then generate the beat type
          fBrailleCodeIOstream <<
            timeItem->getTimeBeatValue ();
  
          // and finally generate the closing parenthesis
          fBrailleCodeIOstream <<
            ")";
  
          if (i != timesItemsNumber - 1) {
            fBrailleCodeIOstream <<
              " ";
          }
        } // for
              
      fBrailleCodeIOstream <<
        ")" <<
        endl;
      }
    }
    
    else {
      // there are no time items
      if (timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          elt->getInputLineNumber (),
          __FILE__, __LINE__,
          "time items vector is empty");
      }
    }
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrTime& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTime " <<
      elt->asString () <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTranspose& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  int  transposeDiatonic  = elt->getTransposeDiatonic ();
  int  transposeChromatic = elt->getTransposeChromatic ();
  int  transposeOctaveChange = elt->getTransposeOctaveChange ();
  bool transposeDouble = elt->getTransposeDouble ();

/ *
  // transposition in LilyPond is relative to c',
  // i.e. the C in the middle of the piano keyboard

The diatonic element specifies the number of pitch steps needed to go from written to sounding pitch. This allows for correct spelling of enharmonic transpositions.

The chromatic element represents the number of semitones needed to get from written to sounding pitch. This value does not include octave-change values; the values for both elements need to be added to the written pitch to get the correct sounding pitch.

The octave-change element indicates how many octaves to add to get from written pitch to sounding pitch.

If the double element is present, it indicates that the music is doubled one octave down from what is currently written (as is the case for mixed cello / bass parts in orchestral literature).
* /
  
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
          gXml2lyOptions->fInputSourceName,
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
        gBsrOptions->
          fBsrQuarterTonesPitchesLanguageKind,
        transpositionPitchKind);

  string
    transpositionOctaveAsString =
      absoluteOctaveAsBrailleString (
        transpositionOctave);

/ * JMI
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions) {
    fBrailleCodeIOstream << // JMI
      "Handlling transpose '" <<
      elt->transposeAsString () <<
      "' ignored because it is already present in voice \"" <<
      fCurrentVoice->getVoiceName () <<
      "\"" <<
      / * JMI
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
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
* /

  // now we can generate the transpostion command
  fBrailleCodeIOstream <<
    "\\transposition " <<
    transpositionPitchKindAsString <<
    transpositionOctaveAsString <<
    " " <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_msrTranspose& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTranspose" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrPartNameDisplay& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrPartNameDisplay" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  string partNameDisplayValue =
    elt->getPartNameDisplayValue ();
    
  fBrailleCodeIOstream <<
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
void bsr2BrailleTranslator::visitStart (S_msrPartAbbreviationDisplay& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrPartAbbreviationDisplay" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  string partAbbreviationDisplayValue =
    elt->getPartAbbreviationDisplayValue ();

  fBrailleCodeIOstream <<
    "\\set Staff.shortInstrumentName = " <<
    nameAsBrailleString (partAbbreviationDisplayValue) <<
    endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTempo& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

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
      break;
    case msrPlacementKind::kPlacementBelow:
      fBrailleCodeIOstream <<
        "\\once\\override Score.MetronomeMark.direction = #DOWN";
      break;
    } // switch
  
  switch (elt->getTempoKind ()) {
    case msrTempo::k_NoTempoKind:
      break;

    case msrTempo::kTempoBeatUnitsPerMinute:
      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fBrailleCodeIOstream <<
            "\\tempo " <<
            "\\markup {" <<
            endl;
    
          gIndenter++;
    
          fBrailleCodeIOstream <<
            "\\concat {" <<
            endl;
    
          gIndenter++;
    
          fBrailleCodeIOstream <<
            "(" <<
            endl;
            
          gIndenter++;
  
          if (tempoWordsListSize) {
            list<S_msrWords>::const_iterator
              iBegin = tempoWordsList.begin (),
              iEnd   = tempoWordsList.end (),
              i      = iBegin;
              
            for ( ; ; ) {
              S_msrWords words = (*i);
        
              fBrailleCodeIOstream <<
                "\"" << words->getWordsContents () << "\"";
                
              if (++i == iEnd) break;
              
              fBrailleCodeIOstream <<
                " ";
            } // for
          }

          fBrailleCodeIOstream <<
    // JMI        "\\smaller \\general-align #Y #DOWN \\note #\"" <<
            " \\smaller \\general-align #Y #DOWN \\note {" << // 2.21.0
            dottedDurationAsBrailleStringWithoutBackSlash (
              inputLineNumber,
              tempoBeatUnit) <<
            "} #UP" <<
      //      "\" #UP" <<
            endl <<
            "=" <<
            endl <<
            tempoPerMinute <<
            endl;
            
          gIndenter--;
    
          fBrailleCodeIOstream <<
            ")" <<
            endl;
            
          gIndenter--;
    
          fBrailleCodeIOstream <<
            "}" <<
            endl;
            
          gIndenter--;
            
          fBrailleCodeIOstream <<
            "}" <<
            endl;
        break;

      case msrTempo::kTempoParenthesizedNo:
        fBrailleCodeIOstream <<
          "\\tempo ";
        
        if (tempoWordsListSize) {
          list<S_msrWords>::const_iterator
            iBegin = tempoWordsList.begin (),
            iEnd   = tempoWordsList.end (),
            i      = iBegin;
            
          for ( ; ; ) {
            S_msrWords words = (*i);
      
            fBrailleCodeIOstream <<
              "\"" << words->getWordsContents () << "\"";
              
            if (++i == iEnd) break;
            
            fBrailleCodeIOstream <<
              " ";
          } // for
        }
  
        fBrailleCodeIOstream <<
          " " <<
          dottedDurationAsBrailleString (
            inputLineNumber,
            tempoBeatUnit) <<
          " = " <<
          tempoPerMinute;

        fBrailleCodeIOstream <<
          endl;
        break;
      } // switch
      break;

    case msrTempo::kTempoBeatUnitsEquivalence:
      fBrailleCodeIOstream <<
        "\\tempo ";
    
      if (tempoWordsListSize) {
        list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;
          
        for ( ; ; ) {
          S_msrWords words = (*i);
    
          fBrailleCodeIOstream <<
            "\"" << words->getWordsContents () << "\"";
            
          if (++i == iEnd) break;
          
          fBrailleCodeIOstream <<
            " ";
        } // for
      }

      fBrailleCodeIOstream <<
        " " <<
        "\\markup {" <<
        endl;

      gIndenter++;

      fBrailleCodeIOstream <<
        "\\concat {" <<
        endl;

      gIndenter++;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fBrailleCodeIOstream <<
            "(" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch
        
      gIndenter++;
      
      fBrailleCodeIOstream <<
        "\\smaller \\general-align #Y #DOWN \\note #\"" <<
        dottedDurationAsBrailleStringWithoutBackSlash (
          inputLineNumber,
          tempoBeatUnit) <<
        "\" #UP" <<
        endl <<
        "=" <<
        endl <<
        "\\smaller \\general-align #Y #DOWN \\note #\"" <<
        dottedDurationAsBrailleStringWithoutBackSlash (
          inputLineNumber,
          elt->getTempoEquivalentBeatUnit ()) <<
        "\" #UP" <<
        endl;
        
      gIndenter--;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fBrailleCodeIOstream <<
            ")" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch
        
      gIndenter--;

      fBrailleCodeIOstream <<
        "}" <<
        endl;
        
      gIndenter--;
        
      fBrailleCodeIOstream <<
        "}" <<
        endl;
      break;
      
    case msrTempo::kTempoNotesRelationShip:
      fBrailleCodeIOstream <<
        "\\tempoRelationship #\"";

      if (tempoWordsListSize) {
        list<S_msrWords>::const_iterator
          iBegin = tempoWordsList.begin (),
          iEnd   = tempoWordsList.end (),
          i      = iBegin;
          
        for ( ; ; ) {
          S_msrWords words = (*i);
    
          fBrailleCodeIOstream <<
            "\"" << words->getWordsContents () << "\"";
            
          if (++i == iEnd) break;
          
          fBrailleCodeIOstream <<
            " ";
        } // for
      }

      fBrailleCodeIOstream <<
        "\"";

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fBrailleCodeIOstream <<
            " ##t";
          break;
        case msrTempo::kTempoParenthesizedNo:
          fBrailleCodeIOstream <<
            " ##f";
          break;
      } // switch
        
      fBrailleCodeIOstream <<
        endl;
      break;
  } // switch
}

void bsr2BrailleTranslator::visitStart (S_msrTempoRelationshipElements& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTempoRelationshipElements" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\fixed b' {" <<
    endl;

  gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_msrTempoRelationshipElements& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTempoRelationshipElements" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;

  fBrailleCodeIOstream <<
    endl <<
    "}" <<
    endl;
}

void bsr2BrailleTranslator::visitStart (S_msrTempoNote& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTempoNote" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "b" <<
    wholeNotesAsBrailleString (
      elt->getInputLineNumber (),
      elt->getTempoNoteWholeNotes ()) <<
      " ";
}

void bsr2BrailleTranslator::visitStart (S_msrTempoTuplet& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTempoTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\tuplet " <<
    elt->getTempoTupletActualNotes () <<
    "/" <<
    elt->getTempoTupletNormalNotes () << " { ";
}

void bsr2BrailleTranslator::visitEnd (S_msrTempoTuplet& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTempoTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "}";
}

void bsr2BrailleTranslator::visitEnd (S_msrTempo& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTempo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrArticulation& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrArticulation& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrArticulation" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrFermata& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/ * JMI
  switch (elt->getArticulationPlacement ()) {
    case kPlacementNone:
      // nothing needed
      break;
    case kPlacementAbove:
      fBrailleCodeIOstream << "^";
      break;
    case kPlacementBelow:
      fBrailleCodeIOstream << "_";
      break;
  } // switch

  // don't generate fermatas for chord member notes
  if (false && fOnGoingNote) { // JMI
    switch (elt->getFermataTypeKind ()) {
      case msrFermata::kFermataTypeNone:
        // no placement needed
        break;
      case msrFermata::kFermataTypeUpright:
        // no placement needed
        break;
      case msrFermata::kFermataTypeInverted:
        fBrailleCodeIOstream << "_";
        break;
    } // switch
  
    switch (elt->getFermataKind ()) {
      case msrFermata::kNormalFermataKind:
        fBrailleCodeIOstream << "\\fermata ";
        break;
      case msrFermata::kAngledFermataKind:
        fBrailleCodeIOstream << "\\shortfermata ";
        break;
      case msrFermata::kSquareFermataKind:
        fBrailleCodeIOstream << "\\longfermata ";
        break;
    } // switch
  }
* /
}

void bsr2BrailleTranslator::visitEnd (S_msrFermata& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrFermata" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrArpeggiato& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrArpeggiato& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrNonArpeggiato& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrNonArpeggiato& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrNonArpeggiato" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTechnical& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the technical here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrTechnical& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTechnical" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTechnicalWithInteger" <<
      ", fOnGoingChord = " <<
      booleanAsString (fOnGoingChord) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the technicalWithInteger here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTechnicalWithInteger" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTechnicalWithFloat& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTechnicalWithFloat" <<
      ", fOnGoingChord = " <<
      booleanAsString (fOnGoingChord) <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the technicalWithFloat here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrTechnicalWithFloat& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTechnicalWithFloat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTechnicalWithString& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the technicalWithString here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTechnicalWithString" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrOrnament& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the ornament here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrOrnament& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrOrnament" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrGlissando& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the glissando here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrGlissando& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrGlissando" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrSlide& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the slide here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrSlide& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrSlide" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrSingleTremolo& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // don't generate the singleTremolo here,
  // the note or chord will do it in its visitEnd () method
}

void bsr2BrailleTranslator::visitEnd (S_msrSingleTremolo& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrSingleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrDoubleTremolo& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // get double tremolo number of repeats
  int numberOfRepeats =
    elt->getDoubleTremoloNumberOfRepeats ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos) {
    fBrailleCodeIOstream <<
      "% visitStart (S_msrDoubleTremolo&)" <<
      endl;

    gIndenter++;
    
    fBrailleCodeIOstream <<
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
  
  fBrailleCodeIOstream <<
    "\\repeat tremolo " << numberOfRepeats << " {";

  gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrDoubleTremolo" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;
  
  fBrailleCodeIOstream <<
    "}" <<
    endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrDynamics& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrDynamics& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrOtherDynamics& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrOtherDynamics& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrOtherDynamics" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrWords& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrWords& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrWords" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrSlur& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrSlur& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrSlur" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrLigature& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrLigature& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrLigature" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrWedge& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrWedge& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrWedge" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::generateNoteBeams (S_msrNote note)
{
  const list<S_msrBeam>&
    noteBeams =
      note->getNoteBeams ();
            
  if (noteBeams.size ()) {
    list<S_msrBeam>::const_iterator i;
    for (
      i=noteBeams.begin ();
      i!=noteBeams.end ();
      i++) {
      S_msrBeam beam = (*i);

      // LilyPond will take care of multiple beams automatically,
      // so we need only generate code for the first number (level)
      switch (beam->getBeamKind ()) {
        
        case msrBeam::kBeginBeam:
          if (beam->getBeamNumber () == 1)
            fBrailleCodeIOstream << "[ ";
          break;
          
        case msrBeam::kContinueBeam:
          break;
          
        case msrBeam::kEndBeam:
          if (beam->getBeamNumber () == 1)
            fBrailleCodeIOstream << "] ";
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
void bsr2BrailleTranslator::generateNoteSlurs (S_msrNote note)
{
  const list<S_msrSlur>&
    noteSlurs =
      note->getNoteSlurs ();
      
  if (noteSlurs.size ()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=noteSlurs.begin ();
      i!=noteSlurs.end ();
      i++) {
      S_msrSlur slur = (*i);
      
      switch (slur->getSlurTypeKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kRegularSlurStart:
          fBrailleCodeIOstream << "( ";
          break;
        case msrSlur::kPhrasingSlurStart:
          fBrailleCodeIOstream << "\\( ";
          break;
        case msrSlur::kSlurContinue:
          break;
        case msrSlur::kRegularSlurStop:
          fBrailleCodeIOstream << ") ";
          break;
        case msrSlur::kPhrasingSlurStop:
          fBrailleCodeIOstream << "\\) ";
          break;
      } // switch
    } // for
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::generateGraceNotesGroup (
  S_msrGraceNotesGroup graceNotesGroup)
{
  / *
    1. no slash, no slur: \grace
    2. slash and slur: \acciaccatura
    3. slash but no slur: \slashedGrace
    4. no slash but slur: \appoggiatura
  * /
  
  if (graceNotesGroup->getGraceNotesGroupIsSlashed ()) {
    if (graceNotesGroup->getGraceNotesGroupIsTied ()) {
      fBrailleCodeIOstream <<
        "\\acciaccatura";
    }
    else {
      fBrailleCodeIOstream <<
        "\\slashedGrace";
    }
  }
  
  else {
    if (graceNotesGroup->getGraceNotesGroupIsTied ()) {
      fBrailleCodeIOstream <<
        "\\appoggiatura";
    }
    else {
      fBrailleCodeIOstream <<
        "\\grace";
    }
  }

  fBrailleCodeIOstream <<
    " { ";

  // force durations to be displayed explicitly
  // at the beginning of the grace notes
  fLastMetWholeNotes = rational (0, 1);

  list<S_msrElement>&
    graceNotesGroupElementsList =
      graceNotesGroup->
        getGraceNotesGroupElementsList ();

  if (graceNotesGroupElementsList.size ()) {
    list<S_msrElement>::const_iterator
      iBegin = graceNotesGroupElementsList.begin (),
      iEnd   = graceNotesGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      S_msrElement element = (*i);

      if (
        // note?
        S_msrNote
          note =
            dynamic_cast<msrNote*>(&(*element))
        ) {
          // print the note itself
          generateNote (note);

          // print the note beams if any,
          // unless the note is chord member
          if (! note->getNoteBelongsToAChord ()) {
            generateNoteBeams (note);
          }
        
          // print the note slurs if any,
          // unless the note is chord member
          if (! note->getNoteBelongsToAChord ()) {
            generateNoteSlurs (note);
          }
        }
        
      else if (
        // chord?
        S_msrChord
          chord =
            dynamic_cast<msrChord*>(&(*element))
        ) {
          generateChord (chord);
        }
        
      else {
        stringstream s;
    
        fBrailleCodeIOstream <<
          "grace notes group elements list in '" <<
          graceNotesGroup->asString () <<
          "' is empty" <<
          ", line " << graceNotesGroup->getInputLineNumber ();
          
        msrInternalError (
          gXml2lyOptions->fInputSourceName,
          graceNotesGroup->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
      
      if (++i == iEnd) break;
      fBrailleCodeIOstream <<
        " ";
    } // for

    fBrailleCodeIOstream << "} ";
  }
  
  else {
    stringstream s;

    fBrailleCodeIOstream <<
      "grace notes group elements list in '" <<
      graceNotesGroup->asString () <<
      "' is empty" <<
      ", line " << graceNotesGroup->getInputLineNumber ();
      
    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      graceNotesGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // force durations to be displayed explicitly
  // at the end of the grace notes
  fLastMetWholeNotes = rational (0, 1);
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrGraceNotesGroup& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrGraceNotesGroup" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingGraceNotesGroup = true;
}

void bsr2BrailleTranslator::visitEnd (S_msrGraceNotesGroup& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrGraceNotesGroup" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOnGoingGraceNotesGroup = false;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrAfterGraceNotesGroup& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrAfterGraceNotesGroup" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

 // JMI exists? if (elt->getGraceNotesGroupIsSlashed ()) {}
  fBrailleCodeIOstream <<
    "\\afterGrace { ";
}

void bsr2BrailleTranslator::visitStart (S_msrAfterGraceNotesGroupContents& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrAfterGraceNotesGroupContents" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // write a first closing right bracket right now
  fBrailleCodeIOstream <<
    "} { ";

  // force durations to be displayed explicitly
  // at the beginning of the after grace notes contents
  fLastMetWholeNotes = rational (0, 1);
}

void bsr2BrailleTranslator::visitEnd (S_msrAfterGraceNotesGroupContents& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrAfterGraceNotesGroupContents" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "} ";
}

void bsr2BrailleTranslator::visitEnd (S_msrAfterGraceNotesGroup& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrAfterGraceNotesGroup" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrNote& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting " <<
      msrNote::noteKindAsString (elt->getNoteKind ()) <<
      " note" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // is this a multiple rest to be ignored?
  switch (elt->getNoteKind ()) {
    case msrNote::kRestNote:
      // don't handle multiple rests, that's done in visitEnd (S_msrMultipleRest&)
      if (fOnGoingMultipleRestMeasures) {
        / *
        if (elt->getNoteOccupiesAFullMeasure ()) {
          bool inhibitMultipleRestMeasuresBrowsing =
            fVisitedBsrScore->
              getMsrScore ()->
                getInhibitMultipleRestMeasuresBrowsing ();
        
          if (inhibitMultipleRestMeasuresBrowsing) {
            if (
              gMsrOptions->fTraceMsrVisitors
                ||
              gTraceOptions->fTraceRepeats) {
              gLogIOstream <<
                "% ==> visiting multiple rest measure is ignored" <<
                endl;
            }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    gLogIOstream <<
      "% ==> returning from visitStart (S_msrNote&)" <<
    endl;
  }
#endif

          return;
          }
        }
        * /

#ifdef TRACE_OPTIONS
        if (
          gMsrOptions->fTraceMsrVisitors
            ||
          gTraceOptions->fTraceRepeats
        ) {
          gLogIOstream <<
            "% ==> start visiting multiple rest measure is ignored" <<
            endl;
        }
#endif

        return;
      }
      break;

    case msrNote::kSkipNote:
      if (elt->getNoteGraceNotesGroupUplink ()) {
#ifdef TRACE_OPTIONS
        if (
          gMsrOptions->fTraceMsrVisitors
            ||
          gTraceOptions->fTraceNotes
        ) {
          gLogIOstream <<
            "% ==> start visiting skip notes is ignored" <<
            endl;
        }
#endif
  
          return;
      }
      break;
      
    case msrNote::kGraceNote:
#ifdef TRACE_OPTIONS
        if (
          gMsrOptions->fTraceMsrVisitors
            ||
          gTraceOptions->fTraceGraceNotes
        ) {
          gLogIOstream <<
            "% ==> start visiting grace notes is ignored" <<
            endl;
        }
#endif

        return;
      break;
      
    case msrNote::kGraceChordMemberNote:
#ifdef TRACE_OPTIONS
        if (
          gMsrOptions->fTraceMsrVisitors
            ||
          gTraceOptions->fTraceGraceNotes
        ) {
          gLogIOstream <<
            "% ==> start visiting chord grace notes is ignored" <<
            endl;
        }
#endif

        return;
      break;
      
    default:
      ;
  } // switch

  // get the note's grace notes after
  S_msrGraceNotesGroup
    noteGraceNotesGroupAfter =
      elt->getNoteGraceNotesGroupAfter ();

  // print the note's grace notes group after opener if any
  if (noteGraceNotesGroupAfter) {
    fBrailleCodeIOstream <<
      "\\afterGrace { ";
  }
  
  // print the note's grace notes before if any,
  // but not ??? JMI
  S_msrGraceNotesGroup
    noteGraceNotesGroupBefore =
      elt->getNoteGraceNotesGroupBefore ();

  if (noteGraceNotesGroupBefore) {
    generateGraceNotesGroup (
      noteGraceNotesGroupBefore);
  }
  
  // print the note scordaturas if any
  const list<S_msrScordatura>&
    noteScordaturas =
      elt->getNoteScordaturas ();
      
  if (noteScordaturas.size ()) {
    fBrailleCodeIOstream <<
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
        
      fBrailleCodeIOstream <<
        "\\new Staff \\with { alignAboveContext = \"" <<
        elt->getNoteMeasureUplink ()->
          getMeasureSegmentUplink ()->
            getSegmentVoiceUplink ()->
              getVoiceStaffUplink ()->
                getStaffName () <<
        "\" } {" <<
        endl;
    
      gIndenter++;
    
      fBrailleCodeIOstream <<
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
            
          fBrailleCodeIOstream <<
            stringTuningAsBrailleString (
              elt->getInputLineNumber (),
              stringTuning);
    
          if (++i == iEnd) break;
          
          fBrailleCodeIOstream << " ";
        } // for
      }
        
      fBrailleCodeIOstream <<
        ">4" <<
        endl;
        
      gIndenter--;
      
      fBrailleCodeIOstream <<
        "}" <<
        endl;
    
      gIndenter--;

      fBrailleCodeIOstream <<
        "{" <<
        endl;

      gIndenter++;
      
      if (++i == iEnd) break;
    } // for
  }

  // should the note actually be printed?
  msrNote::msrNotePrintKind
    notePrintKind =
      elt->getNotePrintKind ();

  if (notePrintKind != fCurrentNotePrintKind) {
    switch (notePrintKind) {
      case msrNote::kNotePrintYes:
        fBrailleCodeIOstream <<
          endl <<
          "\\revert NoteHead.color" <<
          endl;
        break;
      case msrNote::kNotePrintNo:
        fBrailleCodeIOstream <<
          endl <<
          "\\temporary\\override NoteHead.color = #(rgb-color 0.5 0.5 0.5)" <<
          endl;
        break;
    } // switch

    fCurrentNotePrintKind = notePrintKind;
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
      i++) {
      S_msrSlash
        slash =
          (*i);
      
      switch (slash->getSlashTypeKind ()) {
        case k_NoSlashType:
          break;
          
        case kSlashTypeStart:
          fBrailleCodeIOstream <<
            endl <<
            "\\override Staff.NoteHead.style = #'slash " <<
            endl;
          break;

        case kSlashTypeStop:
          fBrailleCodeIOstream <<
            endl <<
            "\\revert Staff.NoteHead.style " <<
            endl;
          break;
      } // switch
      
      switch (slash->getSlashUseDotsKind ()) {
        case k_NoSlashUseDots:
          break;
          
        case kSlashUseDotsYes:
          fBrailleCodeIOstream <<
            endl <<
            "\\override Staff.NoteHead.style = #'slash " <<
            endl;
          break;

        case kSlashUseDotsNo:
          fBrailleCodeIOstream <<
            endl <<
            "\\revert Staff.NoteHead.style " <<
            endl;
          break;
      } // switch
      
      switch (slash->getSlashUseStemsKind ()) {
        case k_NoSlashUseStems:
          break;
          
        case kSlashUseStemsYes:
          fBrailleCodeIOstream <<
            endl <<
            "\\undo \\hide Staff.Stem " <<
            endl;
          break;

        case kSlashUseStemsNo:
          fBrailleCodeIOstream <<
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
      i++) {
      S_msrWedge wedge = (*i);
      
      switch (wedge->getWedgeKind ()) {
        case msrWedge::kWedgeKindNone:
          break;
          
        case msrWedge::kWedgeCrescendo:
          switch (wedge->getWedgeNienteKind ()) {
            case msrWedge::kWedgeNienteYes:
              fBrailleCodeIOstream <<
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
              fBrailleCodeIOstream <<
                endl <<
                "\\once\\override Hairpin.circled-tip = ##t " <<
                endl;
              break;
            case msrWedge::kWedgeNienteNo:
              break;
            } // switch
          break;
          
        case msrWedge::kWedgeStop:
        / * JMI
          fBrailleCodeIOstream <<
            "\\! ";
            * /
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
        i++) {
        S_msrSlur slur = (*i);
        
        switch (slur->getSlurTypeKind ()) {
          case msrSlur::kRegularSlurStart:
          case msrSlur::kPhrasingSlurStart:
            switch (slur->getSlurLineTypeKind ()) {
              case kLineTypeSolid:
                / * JMI ???
                fBrailleCodeIOstream <<
                  "\\once\\slurSolid ";
                * /
                break;
              case kLineTypeDashed:
                fBrailleCodeIOstream <<
                  "\\once\\slurDashed ";
                break;
              case kLineTypeDotted:
                fBrailleCodeIOstream <<
                  "\\once\\slurDotted ";
                break;
              case kLineTypeWavy:
                fBrailleCodeIOstream <<
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
      i++) {
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
              fBrailleCodeIOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fBrailleCodeIOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fBrailleCodeIOstream <<
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
      i++) {        
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
              fBrailleCodeIOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fBrailleCodeIOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fBrailleCodeIOstream <<
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
      i++) {
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
              fBrailleCodeIOstream <<
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
      i++) {
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
              fBrailleCodeIOstream <<
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

  // print the note spanners if any
  const list<S_msrSpanner>&
    noteSpanners =
      elt->getNoteSpanners ();
      
  if (noteSpanners.size ()) {
    list<S_msrSpanner>::const_iterator i;
    for (
      i=noteSpanners.begin ();
      i!=noteSpanners.end ();
      i++) {
      S_msrSpanner
        spanner = (*i);
        
      bool doGenerateSpannerCode = true;
      
      switch (spanner->getSpannerKind ()) {
        case msrSpanner::kSpannerDashes:
          break;
        case msrSpanner::kSpannerWavyLine:
          if (spanner->getSpannerNoteUplink ()->getNoteTrillOrnament ()) {
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
  
  // should the note be parenthesized?
  msrNote::msrNoteHeadParenthesesKind
    noteHeadParenthesesKind =
      elt->getNoteHeadParenthesesKind ();

  switch (noteHeadParenthesesKind) {
    case msrNote::kNoteHeadParenthesesYes:
      fBrailleCodeIOstream << "\\parenthesize ";
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
      i++) {
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
                  
                fBrailleCodeIOstream <<
                  "\\after " <<
                  wholeNotesAsBrailleString (
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
                  
                fBrailleCodeIOstream <<
                  "\\after " <<
                  wholeNotesAsBrailleString (
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
    fBrailleCodeIOstream <<
      "\\once \\override NoteHead.font-size = -3 ";
  }

  // has the note an octave shift up or down?
  S_msrOctaveShift
    noteOctaveShift =
      elt->
        getNoteOctaveShift ();

  if (noteOctaveShift) {
    msrOctaveShift::msrOctaveShiftKind
      octaveShiftKind =
        noteOctaveShift->
          getOctaveShiftKind ();

    int
      octaveShiftSize =
        noteOctaveShift->
          getOctaveShiftSize ();
  
    switch (octaveShiftKind) {
      case msrOctaveShift::kOctaveShiftNone:
        break;
      case msrOctaveShift::kOctaveShiftUp:
        fBrailleCodeIOstream <<
        "\\ottava #" <<
          "-" << (octaveShiftSize - 1) / 7 << // 1 or 2
          " ";
        break;
      case msrOctaveShift::kOctaveShiftDown:
        fBrailleCodeIOstream <<
          "\\ottava #" <<
          (octaveShiftSize - 1) / 7 << // 1 or 2
          " ";
        break;
      case msrOctaveShift::kOctaveShiftStop:
        break;
      case msrOctaveShift::kOctaveShiftContinue:
        break;
    } // switch
  }
  
  // has the note an octave stop (should precede the note)?
  if (noteOctaveShift) {
    msrOctaveShift::msrOctaveShiftKind
      octaveShiftKind =
        noteOctaveShift->
          getOctaveShiftKind ();
  
    switch (octaveShiftKind) {
      case msrOctaveShift::kOctaveShiftNone:
        break;
      case msrOctaveShift::kOctaveShiftUp:
        break;
      case msrOctaveShift::kOctaveShiftDown:
        break;
      case msrOctaveShift::kOctaveShiftStop:
        fBrailleCodeIOstream <<
          "\\ottava #0" <<
          " ";
        break;
      case msrOctaveShift::kOctaveShiftContinue:
        break;
    } // switch
  }
  
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////
  // print the note itself as a LilyPond string
  ////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////

  generateNote (elt);

  if (gBrailleOptions->fNoteInputLineNumbers) {
    // print the note line number as a comment
    fBrailleCodeIOstream <<
      "%{ " << elt->getInputLineNumber () << " %} " <<
      endl; // JMI
  }
  
  fOnGoingNote = true;
}

void bsr2BrailleTranslator::visitEnd (S_msrNote& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting " <<
      msrNote::noteKindAsString (elt->getNoteKind ()) <<
      " note" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // is this a multiple rest to be ignored?
  switch (elt->getNoteKind ()) {
    case msrNote::kRestNote:
      // don't handle multiple rests, that's done in visitEnd (S_msrMultipleRest&)
      if (fOnGoingMultipleRestMeasures) {
        if (elt->getNoteOccupiesAFullMeasure ()) {
          bool inhibitMultipleRestMeasuresBrowsing =
            fVisitedBsrScore->
              getMsrScore ()->
                getInhibitMultipleRestMeasuresBrowsing ();
        
          if (inhibitMultipleRestMeasuresBrowsing) {
#ifdef TRACE_OPTIONS
            if (
              gTraceOptions->fTraceNotes
                ||
              gTraceOptions->fTraceRepeats
            ) {
              gLogIOstream <<
                "% ==> end visiting multiple rest measure is ignored" <<
                endl;
            }
#endif
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceNotesDetails) {
    gLogIOstream <<
      "% ==> returning from visitEnd (S_msrNote&)" <<
      endl;
  }
#endif
            
          return;
          }
        }
      }
      break;

    case msrNote::kSkipNote:
      if (elt->getNoteGraceNotesGroupUplink ()) {
#ifdef TRACE_OPTIONS
        if (
          gMsrOptions->fTraceMsrVisitors
            ||
          gTraceOptions->fTraceNotes
        ) {
          gLogIOstream <<
            "% ==> end visiting skip notes is ignored" <<
            endl;
        }
#endif

        return;
      }
      break;
      
    case msrNote::kGraceNote:
#ifdef TRACE_OPTIONS
        if (
          gMsrOptions->fTraceMsrVisitors
            ||
          gTraceOptions->fTraceGraceNotes) {
          gLogIOstream <<
            "% ==> end visiting grace notes is ignored" <<
            endl;
        }
#endif

        return;
      break;
      
    default:
      ;
  } // switch
    
  // fetch the note single tremolo
  S_msrSingleTremolo
    noteSingleTremolo =
      elt->getNoteSingleTremolo ();

  if (noteSingleTremolo) {
    // generate code for the single tremolo only
    // if note doesn't belong to a chord,
    // otherwise it will be generated for the chord itself
    if (! elt->getNoteBelongsToAChord ()) {
      fBrailleCodeIOstream <<
        singleTremoloDurationAsBrailleString (
          noteSingleTremolo);
    }
  }
  
  // print the note words if any,
  // which should precede the articulations in LilyPond
  const list<S_msrWords>&
    noteWords =
      elt->getNoteWords ();

  if (noteWords.size ()) {
    list<S_msrWords>::const_iterator i;
    for (
      i=noteWords.begin ();
      i!=noteWords.end ();
      i++) {
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
          / * JMI
            s <<
              "%{ " <<
              wordsFontSize->getFontNumericSize () <<
              " points %} ";
              * /
            break;
        } // switch

        s <<
 // JMI         quoteStringIfNonAlpha (wordsContents) <<
          "\"" << wordsContents << "\"" <<
          " } ";

        markup = s.str ();
        }

      fBrailleCodeIOstream <<
        markup;
    } // for
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
        i++) {
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
                  fBrailleCodeIOstream << "_";
                  break;
              } // switch
            
              switch (fermata->getFermataKind ()) {
                case msrFermata::kNormalFermataKind:
                  fBrailleCodeIOstream << "\\fermata ";
                  break;
                case msrFermata::kAngledFermataKind:
                  fBrailleCodeIOstream << "\\shortfermata ";
                  break;
                case msrFermata::kSquareFermataKind:
                  fBrailleCodeIOstream << "\\longfermata ";
                  break;
              } // switch
            }
            else {
              stringstream s;
          
              s <<
                "note articulation '" <<
                articulation->asString () <<
                "'has 'fermata' kind, but is not of type S_msrFermata" <<
                ", line " << articulation->getInputLineNumber ();
                
              msrInternalError (
                gXml2lyOptions->fInputSourceName,
                articulation->getInputLineNumber (),
                __FILE__, __LINE__,
                s.str ());
            }
            break;
            
          default:
            generateNoteArticulation ((*i));
            fBrailleCodeIOstream <<
              " ";
        } // switch
      } // for
    }
  }
  * /
  
  // print the note technicals if any
  const list<S_msrTechnical>&
    noteTechnicals =
      elt->getNoteTechnicals ();
      
  if (noteTechnicals.size ()) {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=noteTechnicals.begin ();
      i!=noteTechnicals.end ();
      i++) {
        
      fBrailleCodeIOstream <<
        technicalAsBrailleString ((*i));

      switch ((*i)->getTechnicalPlacementKind ()) {
        case kPlacementNone:
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fBrailleCodeIOstream << "_";
          break;
      } // switch

      fBrailleCodeIOstream << " ";
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
            i++) {
              
            S_msrTechnicalWithInteger
                technicalWithInteger = (*i);
      
            fBrailleCodeIOstream <<
              technicalWithIntegerAsBrailleString (
                technicalWithInteger);
      
            switch (technicalWithInteger->getTechnicalWithIntegerPlacementKind ()) {
              case kPlacementNone:
                break;
              case kPlacementAbove:
                fBrailleCodeIOstream << "^";
                break;
              case kPlacementBelow:
                fBrailleCodeIOstream << "_";
                break;
            } // switch
      
            fBrailleCodeIOstream << " ";
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
            i++) {
              
            S_msrTechnicalWithFloat
                technicalWithFloat = (*i);
      
            fBrailleCodeIOstream <<
              technicalWithFloatAsBrailleString (
                technicalWithFloat);
      
            switch (technicalWithFloat->getTechnicalWithFloatPlacementKind ()) {
              case kPlacementNone:
                break;
              case kPlacementAbove:
                fBrailleCodeIOstream << "^";
                break;
              case kPlacementBelow:
                fBrailleCodeIOstream << "_";
                break;
            } // switch
      
            fBrailleCodeIOstream << " ";
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
      i++) {
        
      fBrailleCodeIOstream <<
        technicalWithStringAsBrailleString ((*i));

      switch ((*i)->getTechnicalWithStringPlacementKind ()) {
        case kPlacementNone:
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fBrailleCodeIOstream << "_";
          break;
      } // switch

      fBrailleCodeIOstream << " ";
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
      i++) {
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
        i++) {
        S_msrDynamics
          dynamics = (*i);
          
        switch (dynamics->getDynamicsPlacementKind ()) {
          case kPlacementNone:
     // JMI       fBrailleCodeIOstream << "-3";
            break;
          case kPlacementAbove:
            fBrailleCodeIOstream << "^";
            break;
          case kPlacementBelow:
            // this is done by LilyPond by default
            break;
        } // switch
  
        fBrailleCodeIOstream <<
          dynamicsAsBrailleString (dynamics) << " ";
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
        i++) {
        fBrailleCodeIOstream <<
          "-\\markup { "
          "\\dynamic \"" << (*i)->getOtherDynamicsString () << "\" } ";
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
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
        case msrLigature::kLigatureStart:
   // JMI       fBrailleCodeIOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
          fBrailleCodeIOstream << "\\] ";
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
      i++) {
      S_msrWedge wedge = (*i);
      
      switch (wedge->getWedgeKind ()) {
        case msrWedge::kWedgeKindNone:
          break;
          
        case msrWedge::kWedgeCrescendo:
          switch (wedge->getWedgePlacementKind ()) {
            case kPlacementNone:
              break;
            case kPlacementAbove:
              fBrailleCodeIOstream <<
                "^";
              break;
            case kPlacementBelow:
              fBrailleCodeIOstream <<
                "_";
              break;
            } // switch
          fBrailleCodeIOstream <<
            "\\< ";
          break;

        case msrWedge::kWedgeDecrescendo:
          switch (wedge->getWedgePlacementKind ()) {
            case kPlacementNone:
              break;
            case kPlacementAbove:
              fBrailleCodeIOstream <<
                "^";
              break;
            case kPlacementBelow:
              fBrailleCodeIOstream <<
                "_";
              break;
            } // switch
          fBrailleCodeIOstream <<
            "\\> ";
          break;
          
        case msrWedge::kWedgeStop:
          fBrailleCodeIOstream <<
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
        i++) {
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
                  fBrailleCodeIOstream << "_";
                  break;
              } // switch
            
              switch (fermata->getFermataKind ()) {
                case msrFermata::kNormalFermataKind:
                  fBrailleCodeIOstream << "\\fermata ";
                  break;
                case msrFermata::kAngledFermataKind:
                  fBrailleCodeIOstream << "\\shortfermata ";
                  break;
                case msrFermata::kSquareFermataKind:
                  fBrailleCodeIOstream << "\\longfermata ";
                  break;
              } // switch
            }
            else {
              stringstream s;
          
              s <<
                "note articulation '" <<
                articulation->asString () <<
                "'has 'fermata' kind, but is not of type S_msrFermata" <<
                ", line " << articulation->getInputLineNumber ();
                
              msrInternalError (
                gXml2lyOptions->fInputSourceName,
                articulation->getInputLineNumber (),
                __FILE__, __LINE__,
                s.str ());
            }
            break;
            
          default:
            generateNoteArticulation ((*i));
            fBrailleCodeIOstream <<
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
      i++) {
      S_msrGlissando glissando = (*i);
        
      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
          break;
          
        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando itself
          fBrailleCodeIOstream <<
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
      i++) {        
      S_msrSlide slide = (*i);
        
      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
          break;
          
        case msrSlide::kSlideTypeStart:
          // generate the glissando itself
          fBrailleCodeIOstream <<
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
      i++) {
      S_msrSpanner
        spanner = (*i);
        
      bool doGenerateSpannerCode = true;
      
      switch (spanner->getSpannerKind ()) {
        case msrSpanner::kSpannerDashes:
          break;
        case msrSpanner::kSpannerWavyLine:
          if (spanner->getSpannerNoteUplink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            doGenerateSpannerCode = false;
          }
          break;
      } // switch

      if (doGenerateSpannerCode) {
        switch (spanner->getSpannerPlacementKind ()) {
          case kPlacementNone:
     // JMI       fBrailleCodeIOstream << "-3";
            break;
          case kPlacementAbove:
            fBrailleCodeIOstream << "^";
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
    
    fBrailleCodeIOstream <<
      endl <<
      "}" <<
      endl;

    gIndenter--;
    
    fBrailleCodeIOstream <<
      ">>" <<
      endl;
  }

  if (false && elt->getNoteIsFollowedByGraceNotesGroup ()) { // JMI
    if (! elt->getNoteIsARest ()) {
      fBrailleCodeIOstream <<
       " { % NoteIsFollowedByGraceNotesGroup" <<
        endl; // JMI ???
    }
  }

  // get the note's grace notes after
  S_msrGraceNotesGroup
    noteGraceNotesGroupAfter =
      elt->getNoteGraceNotesGroupAfter ();

  // print the note's grace notes after group closer if any
  if (noteGraceNotesGroupAfter) {
    fBrailleCodeIOstream <<
      "} { ";
    generateGraceNotesGroup (
      noteGraceNotesGroupAfter);
    fBrailleCodeIOstream <<
      "} ";
  }

  fOnGoingNote = false;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrOctaveShift& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrOctaveShift& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrOctaveShift" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrAccordionRegistration& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrAccordionRegistration" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

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
  
  fBrailleCodeIOstream <<
    "\\discant \"" <<
    numbersToBeUsed <<
    "\" ";
}

void bsr2BrailleTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrHarpPedalsTuning" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  map<msrDiatonicPitchKind, msrAlterationKind>
    harpPedalsAlterationKindsMap =
      elt->getHarpPedalsAlterationKindsMap ();
      
  if (harpPedalsAlterationKindsMap.size ()) {
    gIndenter++;

    fBrailleCodeIOstream <<
      "_\\markup { \\harp-pedal #\"" <<
      harpPedalTuningAsBrailleString (
        harpPedalsAlterationKindsMap [kD]) <<
      harpPedalTuningAsBrailleString (
        harpPedalsAlterationKindsMap [kC]) <<
      harpPedalTuningAsBrailleString (
        harpPedalsAlterationKindsMap [kB]) <<
      "|" <<
      harpPedalTuningAsBrailleString (
        harpPedalsAlterationKindsMap [kE]) <<
      harpPedalTuningAsBrailleString (
        harpPedalsAlterationKindsMap [kF]) <<
      harpPedalTuningAsBrailleString (
        harpPedalsAlterationKindsMap [kG]) <<
      harpPedalTuningAsBrailleString (
        harpPedalsAlterationKindsMap [kA]) <<
      "\" } " <<
      endl;
  }
  else {
    fBrailleCodeIOstream <<
      "%{empty harp pedals tuning???%} "; // JMI
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrStem& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrStem& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrStem" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrBeam& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrBeam& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrBeam" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::generateChord (S_msrChord chord)
{
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
      i++) {
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
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'dashed-line ";
              break;
            case kLineTypeDotted:
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'dotted-line ";
              break;
            case kLineTypeWavy:
              fBrailleCodeIOstream <<
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
      i++) {        
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
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'dashed-line ";
              break;
            case kLineTypeDotted:
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'dotted-line ";
              break;
            case kLineTypeWavy:
              fBrailleCodeIOstream <<
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
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
        case msrLigature::kLigatureStart:
          fBrailleCodeIOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
  // JMI        fBrailleCodeIOstream << "\\] ";
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
      i++) {
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
            fBrailleCodeIOstream <<
              endl <<
              "\\arpeggioNormal";
            break;
          case kDirectionUp:
            fBrailleCodeIOstream <<
              endl <<
              "\\arpeggioArrowUp";
            break;
          case kDirectionDown:
            fBrailleCodeIOstream <<
              endl <<
              "\\arpeggioArrowDown";
            break;
        } // switch
          
        fBrailleCodeIOstream << " ";

        fCurrentArpeggioDirectionKind = directionKind;
      }
      
      else if (
        // non arpeggiato?
        S_msrNonArpeggiato
          nonArpeggiato =
            dynamic_cast<msrNonArpeggiato*>(&(*articulation))
        ) {
        fBrailleCodeIOstream <<
          endl <<
          "\\arpeggioBracket";
        
        switch (nonArpeggiato->getNonArpeggiatoTypeKind ()) {
          case msrNonArpeggiato::kNonArpeggiatoTypeNone:
            fBrailleCodeIOstream << " %{\\kNonArpeggiatoTypeNone???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeTop:
            fBrailleCodeIOstream << " %{\\kNonArpeggiatoTypeTop???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeBottom:
            fBrailleCodeIOstream << " %{\\kNonArpeggiatoTypeBottom???%}";
            break;
        } // switch
          
        fBrailleCodeIOstream <<
          endl;
      }
   } // for
  }

  // should stem direction be generated?
  const list<S_msrStem>&
    chordStems =
      chord->getChordStems ();
  
  if (chordStems.size ()) {
   list<S_msrStem>::const_iterator
      iBegin = chordStems.begin (),
      iEnd   = chordStems.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrStem stem = (*i);

      switch (stem->getStemKind ()) {
        case msrStem::kStemNone:
          fBrailleCodeIOstream << "\\stemNeutral ";
          break;
        case msrStem::kStemUp:
          fBrailleCodeIOstream << "\\stemUp ";
          break;
        case msrStem::kStemDown:
          fBrailleCodeIOstream << "\\stemDown ";
          break;
        case msrStem::kStemDouble: // JMI ???
          break;
      } // switch
        
      if (++i == iEnd) break;
      fBrailleCodeIOstream <<
        " ";
    } // for
    
    fBrailleCodeIOstream <<
      " ";
  }

  // generate the start of the chord
  fBrailleCodeIOstream <<
    "<";

  fOnGoingChord = true;

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
        note = (*i);

      generateNote (note);
        
      if (++i == iEnd) break;
      fBrailleCodeIOstream <<
        " ";
    } // for
  }

  // generate the end of the chord
  fBrailleCodeIOstream <<
    ">";

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  fRelativeOctaveReference =
    chordNotesVector [0];

  // generate the note duration if relevant
  if (
    chord->getChordIsFirstChordInADoubleTremolo ()
      ||
    chord->getChordIsSecondChordInADoubleTremolo ()) {
      // print chord note duration
      fBrailleCodeIOstream <<
        chord->getChordSoundingWholeNotes ();
  }
  
  else {
    int chordInputLineNumber =
      chord->getInputLineNumber ();
      
    // print the chord duration
    fBrailleCodeIOstream <<
      durationAsBrailleString (
        chordInputLineNumber,
        chord->
          getChordDisplayWholeNotes ()); // JMI test wether chord is in a tuplet?
  }

  // are there pending chord member notes string numbers?
  if (fPendingChordMemberNotesStringNumbers.size ()) {
    list<int>::const_iterator
      iBegin = fPendingChordMemberNotesStringNumbers.begin (),
      iEnd   = fPendingChordMemberNotesStringNumbers.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      int stringNumber = (*i);

      fBrailleCodeIOstream <<
        "\\" <<
        stringNumber;
        
      if (++i == iEnd) break;
      fBrailleCodeIOstream <<
        " ";
    } // for
    fBrailleCodeIOstream <<
      " ";

    // forget about the pending string numbers
    fPendingChordMemberNotesStringNumbers.clear ();
  }
 
  // fetch the chord single tremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      chord->getChordSingleTremolo ();

  if (chordSingleTremolo) {
    // generate code for the chord single tremolo
    fBrailleCodeIOstream <<
      singleTremoloDurationAsBrailleString (
        chordSingleTremolo);
  }

  fBrailleCodeIOstream <<
    " ";
    
  // print the chord articulations if any
  if (chordArticulations.size ()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin ();
      i!=chordArticulations.end ();
      i++) {
      generateChordArticulation ((*i));
             
      fBrailleCodeIOstream <<
        " ";
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
      i++) {
      fBrailleCodeIOstream <<
        technicalAsBrailleString ((*i)) <<
        " "; // JMI
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
      i++) {
      fBrailleCodeIOstream <<
        technicalWithIntegerAsBrailleString ((*i)) <<
        " "; // JMI
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
      i++) {
      fBrailleCodeIOstream <<
        technicalWithFloatAsBrailleString ((*i)) <<
        " "; // JMI
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
      i++) {
      fBrailleCodeIOstream <<
        technicalWithStringAsBrailleString ((*i)) <<
        " "; // JMI
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
      i++) {
      S_msrOrnament
        ornament = (*i);
        
      switch (ornament->getOrnamentPlacementKind ()) {
        case kPlacementNone:
          fBrailleCodeIOstream << "-";
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fBrailleCodeIOstream << "-";
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
      i++) {
      S_msrDynamics
        dynamics = (*i);
        
      switch (dynamics->getDynamicsPlacementKind ()) {
        case kPlacementNone:
          fBrailleCodeIOstream << "-";
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fBrailleCodeIOstream <<
        dynamicsAsBrailleString (dynamics) << " ";
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
      i++) {
      S_msrOtherDynamics
        otherDynamics = (*i);
        
      switch (otherDynamics->getOtherDynamicsPlacementKind ()) {
        case kPlacementNone:
          fBrailleCodeIOstream << "-";
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fBrailleCodeIOstream <<
        "\\" << otherDynamics->asString () << " ";
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
      i++) {
        
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
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fBrailleCodeIOstream << "_";
          break;
      } // switch
    
      fBrailleCodeIOstream <<
        "\\markup" << " { " <<
        quoteStringIfNonAlpha (wordsContents) <<
        " } ";
    } // for
  }

/ *
  // print the chord beams if any
  list<S_msrBeam>
    chordBeams =
      chord->getChordBeams ();
      
  if (chordBeams.size ()) {
    list<S_msrBeam>::const_iterator i;
    for (
      i=chordBeams.begin ();
      i!=chordBeams.end ();
      i++) {
      fBrailleCodeIOstream <<
        "] ";
    } // for
  }
* /

  // print the chord beams if any
  list<S_msrBeam>
    chordBeams =
      chord->getChordBeams ();
      
  if (chordBeams.size ()) {
    list<S_msrBeam>::const_iterator i;
    for (
      i=chordBeams.begin ();
      i!=chordBeams.end ();
      i++) {

      S_msrBeam beam = (*i);
      
      // LilyPond will take care of multiple beams automatically,
      // so we need only generate code for the first number (level)
      switch (beam->getBeamKind ()) {
        
        case msrBeam::kBeginBeam:
          if (beam->getBeamNumber () == 1)
            fBrailleCodeIOstream << "[ ";
          break;
          
        case msrBeam::kContinueBeam:
          break;
          
        case msrBeam::kEndBeam:
          if (beam->getBeamNumber () == 1)
            fBrailleCodeIOstream << "] ";
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

  // print the chord slurs if any
  list<S_msrSlur>
    chordSlurs =
      chord->getChordSlurs ();
      
  if (chordSlurs.size ()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=chordSlurs.begin ();
      i!=chordSlurs.end ();
      i++) {
        
      switch ((*i)->getSlurTypeKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kRegularSlurStart:
          fBrailleCodeIOstream << "( ";
          break;
        case msrSlur::kPhrasingSlurStart:
          fBrailleCodeIOstream << "\\( ";
          break;
        case msrSlur::kSlurContinue:
          break;
        case msrSlur::kRegularSlurStop:
          fBrailleCodeIOstream << ") ";
          break;
        case msrSlur::kPhrasingSlurStop:
          fBrailleCodeIOstream << "\\) ";
          break;
      } // switch
    } // for
  }

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
      fBrailleCodeIOstream << "~ ";
    } // for
  }

  // print the chord ligatures if any
  if (chordLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin ();
      i!=chordLigatures.end ();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
        case msrLigature::kLigatureStart:
  // JMI        fBrailleCodeIOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
          fBrailleCodeIOstream << "\\] ";
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
      i++) {
        
      switch ((*i)->getWedgeKind ()) {
        case msrWedge::kWedgeKindNone:
          break;
        case msrWedge::kWedgeCrescendo:
          fBrailleCodeIOstream << "\\< ";
          break;
        case msrWedge::kWedgeDecrescendo:
          fBrailleCodeIOstream << "\\> ";
          break;
        case msrWedge::kWedgeStop:
          fBrailleCodeIOstream << "\\! ";
          break;
      } // switch
    } // for
  }

  // print the chord glissandos if any
  if (chordGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (
      i=chordGlissandos.begin ();
      i!=chordGlissandos.end ();
      i++) {
      S_msrGlissando glissando = (*i);
        
      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
          break;
          
        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando itself
          fBrailleCodeIOstream <<
            "\\glissando ";
          break;
          
        case msrGlissando::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // print the chord slides if any, implemented as glissandos
  if (chordSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (
      i=chordSlides.begin ();
      i!=chordSlides.end ();
      i++) {        
      S_msrSlide slide = (*i);
        
      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
          break;
          
        case msrSlide::kSlideTypeStart:
          // generate the glissando itself
          fBrailleCodeIOstream <<
            "\\glissando ";
          break;
          
        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

/ *
  // print the tie if any
  {
    S_msrTie chordTie = chord->getChordTie ();
  
    if (chordTie) {
      if (chordTie->getTieKind () == msrTie::kTieStart) {
        fBrailleCodeIOstream << "~ ";
      }
    }
  }
* /

  // a grace chord doesn't matter for the octave relative octave reference

  fOnGoingChord = false; 
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrChord& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  int chordInputLineNumber =
    elt->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (fOnGoingGraceNotesGroup) {
    msrInternalWarning (
      gXml2lyOptions->fInputSourceName,
      chordInputLineNumber,
      "% ==> Start visiting grace chords is ignored");

    return;
  }
#endif
  
  // print the chord's grace notes before if any,
  // but not ??? JMI
  S_msrGraceNotesGroup
    chordGraceNotesGroupBefore =
      elt->getChordGraceNotesGroupBefore ();

/ * JMI
  gLogIOstream <<
    "% chordGraceNotesGroupBefore = ";
  if (chordGraceNotesGroupBefore) {
    gLogIOstream <<
      chordGraceNotesGroupBefore;
  }
  else {
    gLogIOstream <<
      "nullptr";
  }
  gLogIOstream <<
    endl;
* /

  if (chordGraceNotesGroupBefore) {
    generateGraceNotesGroup (
      chordGraceNotesGroupBefore);
  }
  
  // print the chord glissandos styles if any
  const list<S_msrGlissando>&
    chordGlissandos =
      elt->getChordGlissandos ();
      
  if (chordGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (
      i=chordGlissandos.begin ();
      i!=chordGlissandos.end ();
      i++) {
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
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'dashed-line ";
              break;
            case kLineTypeDotted:
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'dotted-line ";
              break;
            case kLineTypeWavy:
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'zigzag ";
              break;
          } // switch
          break;
          
        case msrGlissando::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // print the chord slides styles if any, implemented as glissandos
  const list<S_msrSlide>&
    chordSlides =
      elt->getChordSlides ();
      
  if (chordSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (
      i=chordSlides.begin ();
      i!=chordSlides.end ();
      i++) {        
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
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'dashed-line ";
              break;
            case kLineTypeDotted:
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'dotted-line ";
              break;
            case kLineTypeWavy:
              fBrailleCodeIOstream <<
                "\\once\\override Glissando.style = #'zigzag ";
              break;
          } // switch
          break;
          
        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

  // print the chord ligatures if any
  list<S_msrLigature>
    chordLigatures =
      elt->getChordLigatures ();
      
  if (chordLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin ();
      i!=chordLigatures.end ();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
        case msrLigature::kLigatureStart:
          fBrailleCodeIOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
  // JMI        fBrailleCodeIOstream << "\\] ";
          break;
      } // switch
    } // for
  }

  // don't take the chord into account for line breaking ??? JMI
  
  // print the chord arpeggios directions if any
  list<S_msrArticulation>
    chordArticulations =
      elt->getChordArticulations ();
      
  if (chordArticulations.size ()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin ();
      i!=chordArticulations.end ();
      i++) {
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
            fBrailleCodeIOstream <<
              endl <<
              "\\arpeggioNormal";
            break;
          case kDirectionUp:
            fBrailleCodeIOstream <<
              endl <<
              "\\arpeggioArrowUp";
            break;
          case kDirectionDown:
            fBrailleCodeIOstream <<
              endl <<
              "\\arpeggioArrowDown";
            break;
        } // switch
          
        fBrailleCodeIOstream << " ";

        fCurrentArpeggioDirectionKind = directionKind;
      }
      
      else if (
        // non arpeggiato?
        S_msrNonArpeggiato
          nonArpeggiato =
            dynamic_cast<msrNonArpeggiato*>(&(*articulation))
        ) {
        fBrailleCodeIOstream <<
          endl <<
          "\\arpeggioBracket";
        
        switch (nonArpeggiato->getNonArpeggiatoTypeKind ()) {
          case msrNonArpeggiato::kNonArpeggiatoTypeNone:
            fBrailleCodeIOstream << " %{\\kNonArpeggiatoTypeNone???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeTop:
            fBrailleCodeIOstream << " %{\\kNonArpeggiatoTypeTop???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeBottom:
            fBrailleCodeIOstream << " %{\\kNonArpeggiatoTypeBottom???%}";
            break;
        } // switch
          
        fBrailleCodeIOstream <<
          endl;
      }
   } // for
  }

  // should stem direction be generated?
  const list<S_msrStem>&
    chordStems =
      elt->getChordStems ();
  
  if (chordStems.size ()) {
   list<S_msrStem>::const_iterator
      iBegin = chordStems.begin (),
      iEnd   = chordStems.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      S_msrStem stem = (*i);

      switch (stem->getStemKind ()) {
        case msrStem::kStemNone:
          fBrailleCodeIOstream << "\\stemNeutral ";
          break;
        case msrStem::kStemUp:
          fBrailleCodeIOstream << "\\stemUp ";
          break;
        case msrStem::kStemDown:
          fBrailleCodeIOstream << "\\stemDown ";
          break;
        case msrStem::kStemDouble: // JMI ???
          break;
      } // switch
        
      if (++i == iEnd) break;
      fBrailleCodeIOstream <<
        " ";
    } // for
    
    fBrailleCodeIOstream <<
      " ";
  }

  // generate the start of the chord
  fBrailleCodeIOstream <<
    "<";

  fOnGoingChord = true;
}

void bsr2BrailleTranslator::visitEnd (S_msrChord& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrChord" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  int chordInputLineNumber =
    elt->getInputLineNumber ();
    
#ifdef TRACE_OPTIONS
  if (fOnGoingGraceNotesGroup) {
    msrInternalWarning (
      gXml2lyOptions->fInputSourceName,
      chordInputLineNumber,
      "% ==> End visiting grace chords is ignored");

    return;
  }
#endif
  
  // generate the end of the chord
  fBrailleCodeIOstream <<
    ">";

  if (
    elt->getChordIsFirstChordInADoubleTremolo ()
      ||
    elt->getChordIsSecondChordInADoubleTremolo ()) {
      // print chord note duration
      fBrailleCodeIOstream <<
        elt->getChordSoundingWholeNotes ();
  }
  
  else {
    // print the chord duration
    fBrailleCodeIOstream <<
      durationAsBrailleString (
        chordInputLineNumber,
        elt->
          getChordDisplayWholeNotes ()); // JMI test wether chord is in a tuplet?
  }

  // are there pending chord member notes string numbers?
  if (fPendingChordMemberNotesStringNumbers.size ()) {
    list<int>::const_iterator
      iBegin = fPendingChordMemberNotesStringNumbers.begin (),
      iEnd   = fPendingChordMemberNotesStringNumbers.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      int stringNumber = (*i);

      fBrailleCodeIOstream <<
        "\\" <<
        stringNumber;
        
      if (++i == iEnd) break;
      fBrailleCodeIOstream <<
        " ";
    } // for
    fBrailleCodeIOstream <<
      " ";

    // forget about the pending string numbers
    fPendingChordMemberNotesStringNumbers.clear ();
  }
 
  // fetch the chord single tremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      elt->getChordSingleTremolo ();

  if (chordSingleTremolo) {
    // generate code for the chord single tremolo
    fBrailleCodeIOstream <<
      singleTremoloDurationAsBrailleString (
        chordSingleTremolo);
  }

  fBrailleCodeIOstream <<
    " ";
    
  // print the chord articulations if any
  list<S_msrArticulation>
    chordArticulations =
      elt->getChordArticulations ();
      
  if (chordArticulations.size ()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin ();
      i!=chordArticulations.end ();
      i++) {
      generateChordArticulation ((*i));
             
      fBrailleCodeIOstream <<
        " ";
    } // for
  }

  // print the chord technicals if any
  list<S_msrTechnical>
    chordTechnicals =
      elt->getChordTechnicals ();
      
  if (chordTechnicals.size ()) {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=chordTechnicals.begin ();
      i!=chordTechnicals.end ();
      i++) {
      fBrailleCodeIOstream <<
        technicalAsBrailleString ((*i)) <<
        " "; // JMI
    } // for
  }

  // print the chord technicals with integer if any
  list<S_msrTechnicalWithInteger>
    chordTechnicalWithIntegers =
      elt->getChordTechnicalWithIntegers ();
      
  if (chordTechnicalWithIntegers.size ()) {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=chordTechnicalWithIntegers.begin ();
      i!=chordTechnicalWithIntegers.end ();
      i++) {
      fBrailleCodeIOstream <<
        technicalWithIntegerAsBrailleString ((*i)) <<
        " "; // JMI
    } // for
  }

  // print the chord technicals with float if any
  list<S_msrTechnicalWithFloat>
    chordTechnicalWithFloats =
      elt->getChordTechnicalWithFloats ();
      
  if (chordTechnicalWithFloats.size ()) {
    list<S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i=chordTechnicalWithFloats.begin ();
      i!=chordTechnicalWithFloats.end ();
      i++) {
      fBrailleCodeIOstream <<
        technicalWithFloatAsBrailleString ((*i)) <<
        " "; // JMI
    } // for
  }

  // print the chord technicals with string if any
  list<S_msrTechnicalWithString>
    chordTechnicalWithStrings =
      elt->getChordTechnicalWithStrings ();
      
  if (chordTechnicalWithStrings.size ()) {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=chordTechnicalWithStrings.begin ();
      i!=chordTechnicalWithStrings.end ();
      i++) {
      fBrailleCodeIOstream <<
        technicalWithStringAsBrailleString ((*i)) <<
        " "; // JMI
    } // for
  }

  // print the chord ornaments if any
  list<S_msrOrnament>
    chordOrnaments =
      elt->getChordOrnaments ();
      
  if (chordOrnaments.size ()) {
    list<S_msrOrnament>::const_iterator i;
    for (
      i=chordOrnaments.begin ();
      i!=chordOrnaments.end ();
      i++) {
      S_msrOrnament
        ornament = (*i);
        
      switch (ornament->getOrnamentPlacementKind ()) {
        case kPlacementNone:
          fBrailleCodeIOstream << "-";
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fBrailleCodeIOstream << "-";
          break;
      } // switch

      generateOrnament (ornament); // some ornaments are not yet supported
    } // for
  }

  // print the chord dynamics if any
  list<S_msrDynamics>
    chordDynamics =
      elt->getChordDynamics ();
      
  if (chordDynamics.size ()) {
    list<S_msrDynamics>::const_iterator i;
    for (
      i=chordDynamics.begin ();
      i!=chordDynamics.end ();
      i++) {
      S_msrDynamics
        dynamics = (*i);
        
      switch (dynamics->getDynamicsPlacementKind ()) {
        case kPlacementNone:
          fBrailleCodeIOstream << "-";
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fBrailleCodeIOstream <<
        dynamicsAsBrailleString (dynamics) << " ";
    } // for
  }
  
  // print the chord other dynamics if any
  list<S_msrOtherDynamics>
    chordOtherDynamics =
      elt->getChordOtherDynamics ();
      
  if (chordOtherDynamics.size ()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (
      i=chordOtherDynamics.begin ();
      i!=chordOtherDynamics.end ();
      i++) {
      S_msrOtherDynamics
        otherDynamics = (*i);
        
      switch (otherDynamics->getOtherDynamicsPlacementKind ()) {
        case kPlacementNone:
          fBrailleCodeIOstream << "-";
          break;
        case kPlacementAbove:
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fBrailleCodeIOstream <<
        "\\" << otherDynamics->asString () << " ";
    } // for
  }

  // print the chord words if any
  list<S_msrWords>
    chordWords =
      elt->getChordWords ();
      
  if (chordWords.size ()) {
    list<S_msrWords>::const_iterator i;
    for (
      i=chordWords.begin ();
      i!=chordWords.end ();
      i++) {
        
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
          fBrailleCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fBrailleCodeIOstream << "_";
          break;
      } // switch
    
      fBrailleCodeIOstream <<
        "\\markup" << " { " <<
        quoteStringIfNonAlpha (wordsContents) <<
        " } ";
    } // for
  }

/ *
  // print the chord beams if any
  list<S_msrBeam>
    chordBeams =
      elt->getChordBeams ();
      
  if (chordBeams.size ()) {
    list<S_msrBeam>::const_iterator i;
    for (
      i=chordBeams.begin ();
      i!=chordBeams.end ();
      i++) {
      fBrailleCodeIOstream <<
        "] ";
    } // for
  }
* /

  // print the chord beams if any
  list<S_msrBeam>
    chordBeams =
      elt->getChordBeams ();
      
  if (chordBeams.size ()) {
    list<S_msrBeam>::const_iterator i;
    for (
      i=chordBeams.begin ();
      i!=chordBeams.end ();
      i++) {

      S_msrBeam beam = (*i);
      
      // LilyPond will take care of multiple beams automatically,
      // so we need only generate code for the first number (level)
      switch (beam->getBeamKind ()) {
        
        case msrBeam::kBeginBeam:
          if (beam->getBeamNumber () == 1)
            fBrailleCodeIOstream << "[ ";
          break;
          
        case msrBeam::kContinueBeam:
          break;
          
        case msrBeam::kEndBeam:
          if (beam->getBeamNumber () == 1)
            fBrailleCodeIOstream << "] ";
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

  // print the chord slurs if any
  list<S_msrSlur>
    chordSlurs =
      elt->getChordSlurs ();
      
  if (chordSlurs.size ()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=chordSlurs.begin ();
      i!=chordSlurs.end ();
      i++) {
        
      switch ((*i)->getSlurTypeKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kRegularSlurStart:
          fBrailleCodeIOstream << "( ";
          break;
        case msrSlur::kPhrasingSlurStart:
          fBrailleCodeIOstream << "\\( ";
          break;
        case msrSlur::kSlurContinue:
          break;
        case msrSlur::kRegularSlurStop:
          fBrailleCodeIOstream << ") ";
          break;
        case msrSlur::kPhrasingSlurStop:
          fBrailleCodeIOstream << "\\) ";
          break;
      } // switch
   } // for
  }

  // print the chord ligatures if any
  list<S_msrLigature>
    chordLigatures =
      elt->getChordLigatures ();
      
  if (chordLigatures.size ()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin ();
      i!=chordLigatures.end ();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::kLigatureNone:
          break;
        case msrLigature::kLigatureStart:
  // JMI        fBrailleCodeIOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
          fBrailleCodeIOstream << "\\] ";
          break;
      } // switch
   } // for
  }

  // print the chord wedges if any
  list<S_msrWedge>
    chordWedges =
      elt->getChordWedges ();
      
  if (chordWedges.size ()) {
    list<S_msrWedge>::const_iterator i;
    for (
      i=chordWedges.begin ();
      i!=chordWedges.end ();
      i++) {
        
      switch ((*i)->getWedgeKind ()) {
        case msrWedge::kWedgeKindNone:
          break;
        case msrWedge::kWedgeCrescendo:
          fBrailleCodeIOstream << "\\< ";
          break;
        case msrWedge::kWedgeDecrescendo:
          fBrailleCodeIOstream << "\\> ";
          break;
        case msrWedge::kWedgeStop:
          fBrailleCodeIOstream << "\\! ";
          break;
      } // switch
    } // for
  }

  // print the chord glissandos if any
  const list<S_msrGlissando>&
    chordGlissandos =
      elt->getChordGlissandos ();
      
  if (chordGlissandos.size ()) {
    list<S_msrGlissando>::const_iterator i;
    for (
      i=chordGlissandos.begin ();
      i!=chordGlissandos.end ();
      i++) {
      S_msrGlissando glissando = (*i);
        
      switch (glissando->getGlissandoTypeKind ()) {
        case msrGlissando::kGlissandoTypeNone:
          break;
          
        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando itself
          fBrailleCodeIOstream <<
            "\\glissando ";
          break;
          
        case msrGlissando::kGlissandoTypeStop:
          break;
      } // switch
    } // for
  }

  // print the chord slides if any, implemented as glissandos
  const list<S_msrSlide>&
    chordSlides =
      elt->getChordSlides ();
      
  if (chordSlides.size ()) {
    list<S_msrSlide>::const_iterator i;
    for (
      i=chordSlides.begin ();
      i!=chordSlides.end ();
      i++) {        
      S_msrSlide slide = (*i);
        
      switch (slide->getSlideTypeKind ()) {
        case msrSlide::kSlideTypeNone:
          break;
          
        case msrSlide::kSlideTypeStart:
          // generate the glissando itself
          fBrailleCodeIOstream <<
            "\\glissando ";
          break;
          
        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

/ *
  // print the tie if any
  {
    S_msrTie chordTie = elt->getChordTie ();
  
    if (chordTie) {
      if (chordTie->getTieKind () == msrTie::kTieStart) {
        fBrailleCodeIOstream << "~ ";
      }
    }
  }
* /

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  fRelativeOctaveReference =
    elt->getChordNotesVector () [0];

  fOnGoingChord = false; 
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTuplet& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
    
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
          / * JMI
        containingTuplet->
          getTupletActualNotes (),
        containingTuplet->
          getTupletNormalNotes ());
          * /
  }

  if (gBrailleOptions->fIndentTuplets) {
    fBrailleCodeIOstream <<
      endl;
  }

  // get bracket kind
  msrTuplet::msrTupletBracketKind
    tupletBracketKind =
      elt->getTupletBracketKind ();

  switch (tupletBracketKind) {
    case msrTuplet::kTupletBracketYes:
    / * JMI
      fBrailleCodeIOstream <<
        "%{kTupletBracketYes%}" <<
        endl;
        * /
      break;
    case msrTuplet::kTupletBracketNo:
      fBrailleCodeIOstream <<
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
      fBrailleCodeIOstream <<
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
    / * JMI
      fBrailleCodeIOstream <<
        "%{tupletShowNumberActual%}" <<
        endl;
        * /
      break;
    case msrTuplet::kTupletShowNumberBoth:
      fBrailleCodeIOstream <<
        "\\once \\override TupletNumber.text = #tuplet-number::calc-fraction-text" <<
        endl;
      break;
    case msrTuplet::kTupletShowNumberNone:
      fBrailleCodeIOstream <<
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
      fBrailleCodeIOstream <<
     // JMI ???   "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-fraction-text \"" <<
        "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-denominator-text \"" <<
        wholeNotesAsBrailleString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        "\")" <<
        endl;
      break;
    case msrTuplet::kTupletShowTypeBoth:
      fBrailleCodeIOstream <<
        "\\once \\override TupletNumber.text = #(tuplet-number::fraction-with-notes \"" <<
        wholeNotesAsBrailleString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        "\" \"" <<
        wholeNotesAsBrailleString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        "\")" <<
        endl;
      break;
    case msrTuplet::kTupletShowTypeNone:
    / * JMI
      fBrailleCodeIOstream <<
        "%{tupletShowTypeNone%}" <<
        endl;
        * /
      break;
  } // switch

  fBrailleCodeIOstream <<
    "\\tuplet " <<
    elt->getTupletFactor ().asRational () <<
  / *
    elt->getTupletActualNotes () <<
    "/" <<
    elt->getTupletNormalNotes () <<
    * /
    " { ";

  fTupletsStack.push (elt);

  gIndenter++;

  // force durations to be displayed explicitly
  // at the beginning of the tuplet
  fLastMetWholeNotes = rational (0, 1);
}

void bsr2BrailleTranslator::visitEnd (S_msrTuplet& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTuplet" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;

  if (gBrailleOptions->fIndentTuplets) {
    fBrailleCodeIOstream <<
      endl;
  }
  
  fBrailleCodeIOstream <<
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
      fBrailleCodeIOstream <<
        "\\undo \\tupletsCurvedBrackets" <<
        endl;
      break;
  } // switch

  fTupletsStack.pop ();
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrTie& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
  
  switch (elt->getTieKind ()) {
    case msrTie::kTieNone:
      break;
    case msrTie::kTieStart:
 // JMI     fBrailleCodeIOstream << "~ "; // JMI
      break;
    case msrTie::kTieContinue:
      break;
    case msrTie::kTieStop:
      break;
   } // switch
}

void bsr2BrailleTranslator::visitEnd (S_msrTie& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrTie" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrSegno& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrSegno" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.segno\" }" <<
    endl;
}

void bsr2BrailleTranslator::visitStart (S_msrCoda& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrCoda" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.coda\" }" <<
    endl;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrEyeGlasses& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting eyeGlasses" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "^\\markup {\\eyeglasses} ";
}

void bsr2BrailleTranslator::visitStart (S_msrScordatura& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting scordatura" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

/ * JMI
  const list<S_msrStringTuning>&
    scordaturaStringTuningsList =
      elt->getScordaturaStringTuningsList ();
  
  gIndenter++;
    
  fBrailleCodeIOstream <<
    "\\new Staff {" <<
    endl;

  gIndenter++;

  fBrailleCodeIOstream <<
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
        
      fBrailleCodeIOstream <<
        stringTuningAsBrailleString (
          elt->getInputLineNumber (),
          stringTuning);

      if (++i == iEnd) break;
      
      fBrailleCodeIOstream << " ";
    } // for
  }
    
  fBrailleCodeIOstream <<
    ">4" <<
    endl <<
    "}" <<
    endl;

  gIndenter--;
  
  fBrailleCodeIOstream <<
    "{ c'4 }" <<
    endl <<

  gIndenter--;
  * /
}

void bsr2BrailleTranslator::visitStart (S_msrPedal& elt)
{
  int inputLineNumber =
    elt->getInputLineNumber ();
    
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting pedal" <<
      ", line " << inputLineNumber <<
      endl;
  }
      
  fBrailleCodeIOstream <<
    endl;
    
  switch (elt->getPedalTypeKind ()) {
    case msrPedal::kPedalStart:
      fBrailleCodeIOstream <<
        "\\sustainOn";
      break;
    case msrPedal::kPedalContinue:
      fBrailleCodeIOstream <<
        "\\sustainOff\\sustainOn"; // JMI
      break;
    case msrPedal::kPedalChange:
      fBrailleCodeIOstream <<
        "\\sustainOff\\sustainOn";
      break;
    case msrPedal::kPedalStop:
      fBrailleCodeIOstream <<
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
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  fBrailleCodeIOstream <<
    endl;
}

void bsr2BrailleTranslator::visitStart (S_msrDamp& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting damp" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "^\\markup {\\damp} ";
}

void bsr2BrailleTranslator::visitStart (S_msrDampAll& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting dampAll" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "^\\markup {\\dampAll} ";
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrBarline& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      endl <<
      "% --> Start visiting msrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
  
  switch (elt->getBarlineCategory ()) {
    
    case msrBarline::kBarlineCategoryStandalone:
      switch (elt->getBarlineStyleKind ()) {
        case msrBarline::kBarlineStyleNone:
          break;
        case msrBarline::kBarlineStyleRegular:
          fBrailleCodeIOstream << "\\bar \"|\" ";
          break;
        case msrBarline::kBarlineStyleDotted:
          fBrailleCodeIOstream << "\\bar \";\" ";
          break;
        case msrBarline::kBarlineStyleDashed:
          fBrailleCodeIOstream << "\\bar \"!\" ";
          break;
        case msrBarline::kBarlineStyleHeavy:
          fBrailleCodeIOstream << "\\bar \".\" ";
          break;
        case msrBarline::kBarlineStyleLightLight:
          fBrailleCodeIOstream << "\\bar \"||\" ";
          break;
        case msrBarline::kBarlineStyleLightHeavy:
          fBrailleCodeIOstream <<
            endl <<
            "\\bar \"|.\" ";
          break;
        case msrBarline::kBarlineStyleHeavyLight:
          fBrailleCodeIOstream << "\\bar \".|\" ";
          break;
        case msrBarline::kBarlineStyleHeavyHeavy:
          fBrailleCodeIOstream << "\\bar \"..\" ";
          break;
        case msrBarline::kBarlineStyleTick:
          fBrailleCodeIOstream << "\\bar \"'\" ";
          break;
        case msrBarline::kBarlineStyleShort:
          // \bar "/" is the custom short barline
          fBrailleCodeIOstream << "\\bar \"/\" ";
          break;
          / * JMI
        case msrBarline::kBarlineStyleNone:
          fBrailleCodeIOstream << "\\bar \"\" ";
          break;
          * /
      } // switch

      if (gBrailleOptions->fNoteInputLineNumbers) {
        // print the barline line number as a comment
        fBrailleCodeIOstream <<
          "%{ " << inputLineNumber << " %} ";
      }

      fBrailleCodeIOstream <<
        endl;

      switch (elt->getBarlineHasSegnoKind ()) {
        case msrBarline::kBarlineHasSegnoYes:
          fBrailleCodeIOstream <<
            "\\mark \\markup { \\musicglyph #\"scripts.segno\" } ";
          break;
        case msrBarline::kBarlineHasSegnoNo:
          break;
      } // switch

      switch (elt->getBarlineHasCodaKind ()) {
        case msrBarline::kBarlineHasCodaYes:
          fBrailleCodeIOstream <<
            "\\mark \\markup { \\musicglyph #\"scripts.coda\" } ";
          break;
        case msrBarline::kBarlineHasCodaNo:
          break;
      } // switch
      break;

    case msrBarline::kBarlineCategoryRepeatStart:
    case msrBarline::kBarlineCategoryRepeatEnd:
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
          gXml2lyOptions->fInputSourceName,
          inputLineNumber,
  // JMI        __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

  if (gBrailleOptions->fNoteInputLineNumbers) {
    fBrailleCodeIOstream <<
      " %{ " << inputLineNumber << " %}";
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrBarline& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      endl <<
      "% --> End visiting msrBarline" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrBarCheck& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
      
  string nextBarNumber =
    elt->getNextBarNumber ();

  // don't generate a bar check before the end of measure 1 // JMI ???
  fBrailleCodeIOstream <<
    "| % " << nextBarNumber << " % bar check" <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_msrBarCheck& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrBarCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrBarNumberCheck& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // MusicXML bar numbers cannot be relied upon for a LilyPond bar number check
  fBrailleCodeIOstream <<
    "\\barNumberCheck #" <<
    fCurrentVoiceMeasuresCounter <<
    // JMI elt->getNextBarNumber () <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrBarNumberCheck" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrLineBreak& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\myBreak | % " << elt->getNextBarNumber () <<
    endl <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_msrLineBreak& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrLineBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrPageBreak& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "\\myPageBreak " <<
    endl <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_msrPageBreak& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrPageBreak" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrRepeat& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  int repeatEndingsNumber =
    elt->getRepeatEndings ().size ();
    
  if (repeatEndingsNumber == 0)
    repeatEndingsNumber = 2; // implicitly JMI ???

  fRepeatsDescrStack.push_back (
    msrRepeatDescr::create (
      repeatEndingsNumber));
    
  stringstream s;
  
  s <<
 // JMI   "\\repeat volta " << elt->getRepeatTimes () << " {";
    "\\repeat volta " <<
    fRepeatsDescrStack.back ()->getRepeatEndingsNumber () <<
    " {";

  if (gBrailleOptions->fNoteInputLineNumbers) {
    s <<
      " %{ " << elt->getInputLineNumber () << " %}";
  }
  
  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      s.str () << "% start of repeat";
  }
  else {
    fBrailleCodeIOstream <<
      s.str ();
  }

  fBrailleCodeIOstream <<
    endl;

  gIndenter++;

  int
    repeatTimes =
      elt->getRepeatTimes ();

  if (repeatTimes > 2) {
    fBrailleCodeIOstream <<
      "<>^\"" << repeatTimes << " times\"" << // JMI
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrRepeat& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  / *
    CAUTION:
      the end of the repeat has to be generated
      before the endings are handled
  * /
  
  if (! fRepeatsDescrStack.back ()->getEndOfRepeatHasBeenGenerated ()) {
    // the end of the repeat has not been generated yet

    gIndenter--;

    if (gBrailleOptions->fComments) {      
      fBrailleCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fBrailleCodeIOstream <<
        endl <<
        "}" <<
        endl <<
        endl;
    }
  }

  fRepeatsDescrStack.pop_back ();
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrRepeatCommonPart" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrRepeatCommonPart" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrRepeatEnding& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fRepeatsDescrStack.back ()->
    incrementRepeatEndingsCounter ();

  int
    repeatEndingInternalNumber =
      elt->getRepeatEndingInternalNumber ();
      
  if (repeatEndingInternalNumber == 1) {
    
    gIndenter--;
    
    // first repeat ending is in charge of
    // outputting the end of the repeat
    if (gBrailleOptions->fComments) {      
      fBrailleCodeIOstream <<
        setw (commentFieldWidth) << left <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fBrailleCodeIOstream <<
        endl <<
        "}" <<
        endl;
    }
    fRepeatsDescrStack.back ()->
      setEndOfRepeatHasBeenGenerated ();

    // first repeat ending is in charge of
    // outputting the start of the alternative
    if (gBrailleOptions->fComments) {      
      fBrailleCodeIOstream << left <<
        endl <<
        setw (commentFieldWidth) <<
        "\\alternative {" <<
        "% start of alternative" <<
        endl;
    }
    else {
      fBrailleCodeIOstream <<
        endl <<
        "\\alternative {" <<
        endl;
    }
      
    gIndenter++;
  }

  // output the start of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gBrailleOptions->fComments) {  
        fBrailleCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "{" << "% start of repeat hooked ending" <<
          endl;
      }
      else {
        fBrailleCodeIOstream <<
          "{" <<
          endl;
      }
      break;
      
    case msrRepeatEnding::kHooklessEnding:
      if (gBrailleOptions->fComments) {
        fBrailleCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "{" << "% start of repeat hookless ending" <<
          endl;
      }
      else {
        fBrailleCodeIOstream <<
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
    if (repeatEndingInternalNumber == 1) {
      fBrailleCodeIOstream <<
        "\\set Score.repeatCommands = #'((volta \"" <<
        repeatEndingNumber <<
        "\"))" <<
        endl;
    }
    else {
      fBrailleCodeIOstream <<
        "\\set Score.repeatCommands = #'(end-repeat (volta \"" <<
        repeatEndingNumber <<
        "\"))" <<
        endl;
    }
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrRepeatEnding" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;

  // output the end of the ending
  fBrailleCodeIOstream <<
    endl;

  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gBrailleOptions->fComments) {
        fBrailleCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "}" << "% end of repeat hooked ending" <<
          endl;
      }
      else {
        fBrailleCodeIOstream <<
          "}" <<
          endl;
      }
      break;
      
    case msrRepeatEnding::kHooklessEnding:
      if (gBrailleOptions->fComments)   { 
        fBrailleCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "}" << "% end of repeat hookless ending" <<
          endl;
      }
      else {
        fBrailleCodeIOstream <<
          "}" <<
          endl;
      }
      break;
  } // switch

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    fBrailleCodeIOstream <<
      "% ===**** fRepeatsDescrStack.back () = '" <<
      fRepeatsDescrStack.back ()->repeatDescrAsString () <<
      "'" <<
      endl;
  }
#endif
  
  if (
    fRepeatsDescrStack.back ()->getRepeatEndingsCounter ()
      ==
    fRepeatsDescrStack.back ()->getRepeatEndingsNumber ()) {
      
    gIndenter--;
    
    // last repeat ending is in charge of
    // outputting the end of the alternative
    if (gBrailleOptions->fComments)   { 
      fBrailleCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "}" << "% end of alternative" <<
        endl;
    }
    else {
      fBrailleCodeIOstream <<
        "}" <<
        endl <<
        endl;
    }
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrComment& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrComment" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    "% " << elt->getContents () <<
    endl;
    
  if (elt->getCommentGapKind () == bsrComment::kGapAfterwards)
    fBrailleCodeIOstream << endl;
}

void bsr2BrailleTranslator::visitEnd (S_bsrComment& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrComment" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_bsrSchemeFunction& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting bsrSchemeFunction" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    endl <<
    "% Scheme function(s): \"" << elt->getFunctionName () << "\"" <<
    // endl is in the decription
    elt->getFunctionDescription () <<
    // endl is in the decription
    elt->getFunctionCode () <<
    endl <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_bsrSchemeFunction& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting bsrSchemeFunction" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrRehearsal& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    endl;

  switch (elt->getRehearsalPlacementKind ()) {
    case msrPlacementKind::kPlacementNone:
      break;
    case msrPlacementKind::kPlacementAbove:
      break;
    case msrPlacementKind::kPlacementBelow:
      fBrailleCodeIOstream <<
        "\\once\\override Score.RehearsalMark.direction = #DOWN";
      break;
    } // switch
  
  fBrailleCodeIOstream <<
    endl <<
    "\\mark\\markup { ";

  switch (elt->getRehearsalKind ()) {
    case msrRehearsal::kNone:
      fBrailleCodeIOstream <<
        "\\box"; // default value
      break;
    case msrRehearsal::kRectangle:
      fBrailleCodeIOstream <<
        "\\box";
      break;
    case msrRehearsal::kOval:
      fBrailleCodeIOstream <<
        "\\oval";
      break;
    case msrRehearsal::kCircle:
      fBrailleCodeIOstream <<
        "\\circle";
      break;
    case msrRehearsal::kBracket:
      fBrailleCodeIOstream <<
        "\\bracket";
      break;
    case msrRehearsal::kTriangle:
      fBrailleCodeIOstream <<
        "%{\\triangle???%}";
      break;
    case msrRehearsal::kDiamond:
      fBrailleCodeIOstream <<
        "%{\\diamond???";
      break;
  } // switch
      
  fBrailleCodeIOstream <<
    " { \""<<
    elt->getRehearsalText () <<
    "\"" "}}" <<
    endl;
}

void bsr2BrailleTranslator::visitEnd (S_msrRehearsal& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrRehearsal" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrMeasuresRepeat& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrMeasuresRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  int repeatMeasuresNumber =
    elt->measuresRepeatPatternMeasuresNumber ();

  int replicasMeasuresNumber =
    elt->measuresRepeatReplicasMeasuresNumber ();
    
  int replicasNumber =
    elt->measuresRepeatReplicasNumber ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceRepeats) {
    fBrailleCodeIOstream <<
      "% measure repeat, line " << elt->getInputLineNumber () << ":" <<
      endl;

    const int fieldWidth = 24;

    fBrailleCodeIOstream << left <<
      setw (fieldWidth) <<
      "% repeatMeasuresNumber" << " = " << repeatMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% replicasMeasuresNumber" << " = " << replicasMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% replicasNumber" << " = " << replicasNumber <<
      endl;
  }
#endif
  
  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% start of measure repeat" <<
      singularOrPlural (
        elt->measuresRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fBrailleCodeIOstream <<
    endl <<
    endl <<
    "\\repeat percent " <<
    replicasNumber + 1 <<
     " { " <<
    endl;

    gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_msrMeasuresRepeat& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrMeasuresRepeat" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  gIndenter--;
  
  fBrailleCodeIOstream <<
    endl <<
    endl <<
    " }" <<
    endl;

  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream <<
      setw (commentFieldWidth) << left <<
      "% end of measure repeat" <<
      singularOrPlural (
        elt->measuresRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      endl <<
      endl;      
  }
}

void bsr2BrailleTranslator::visitStart (S_msrMeasuresRepeatPattern& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> Start visiting msrMeasuresRepeatPattern" <<
      endl;
  }

  gIndenter++;

  // JMI
}

void bsr2BrailleTranslator::visitEnd (S_msrMeasuresRepeatPattern& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> End visiting msrMeasuresRepeatPattern" <<
      endl;
  }

  gIndenter--;
}
  
void bsr2BrailleTranslator::visitStart (S_msrMeasuresRepeatReplicas& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> Start visiting msrMeasuresRepeatReplicas" <<
      endl;
  }

  // output the start of the ending
  if (gBrailleOptions->fComments) {  
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "{" << "% start of measures repeat replicas" <<
      endl;
  }
  else {
    fBrailleCodeIOstream <<
      endl <<
      "{" <<
      endl;
  }

  gIndenter++;
}

void bsr2BrailleTranslator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> End visiting msrMeasuresRepeatReplicas" <<
      endl;
  }

  gIndenter--;
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrMultipleRest& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrMultipleRest" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  // generate the code now:
  // waiting until visitEnd (S_msrMultipleRest&)
  // would get barlines if any generated too early
  
  int inputLineNumber =
    elt->getInputLineNumber ();

  int restMeasuresNumber =
    elt->getMultipleRestMeasuresNumber ();

  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% start of multiple rest" <<
      singularOrPlural (
        elt->getMultipleRestMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << elt->getInputLineNumber () <<
      endl <<
      endl;      

    gIndenter++;
  }

  // start counting measures
  fRemainingMultipleRestMeasuresNumber =
    elt->getMultipleRestMeasuresNumber ();

  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% start of multiple rest" <<
      singularOrPlural (
        elt->getMultipleRestMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      endl;
  }

  // get multiple rest measure sounding notes
  rational
    multipleRestMeasureSoundingNotes =
      elt->getMultipleRestMeasureSoundingNotes ();

  // generate multiple measure rest
  fBrailleCodeIOstream <<
    "R" <<
    multipleRestWholeNoteAsBrailleString (
      inputLineNumber,
      multipleRestMeasureSoundingNotes);

  if (restMeasuresNumber > 1) {
    fBrailleCodeIOstream <<
      "*" <<
      restMeasuresNumber;
  }

  if (gBrailleOptions->fNoteInputLineNumbers) {
    // print the multiple rest line number as a comment
    fBrailleCodeIOstream <<
      " %{ " << inputLineNumber << " %} ";
  }

  // wait until all measures have be visited
  // before the bar check is generated

  fOnGoingMultipleRestMeasures = true;
}

void bsr2BrailleTranslator::visitEnd (S_msrMultipleRest& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrMultipleRest" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
    
  // now we can generate the bar check
  fBrailleCodeIOstream <<    
    " | % " <<
    elt->getMultipleRestNextMeasureNumber () <<
    endl;

  /* JMI
  if (gBrailleOptions->fComments) {
    gIndenter--;

    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% end of multiple rest" <<
      singularOrPlural (
        elt->getMultipleRestMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << elt->getInputLineNumber () <<
      endl <<
      endl;      
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  int restMeasuresNumber =
    elt->getMultipleRestMeasuresNumber ();

  int restSegmentMeasuresNumber =
    elt->multipleRestContentsMeasuresNumber ();
    
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceRepeats) {
    fBrailleCodeIOstream <<
      "% multiple rest, line " << inputLineNumber << ":" <<
      endl;

    const int fieldWidth = 24;

    fBrailleCodeIOstream << left <<
      setw (fieldWidth) <<
      "% restMeasuresNumber" << " = " << restMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% restSegmentMeasuresNumber" << " = " << restSegmentMeasuresNumber <<
      endl <<
      endl;
  }
#endif

  if (gBrailleOptions->fComments) {
    fBrailleCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% start of multiple rest" <<
      singularOrPlural (
        elt->getMultipleRestMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      endl;
  }

  // get multiple rest measure sounding notes
  rational
    multipleRestMeasureSoundingNotes =
      elt->getMultipleRestMeasureSoundingNotes ();

  // generate multiple measure rest
  fBrailleCodeIOstream <<
    "R" <<
    multipleRestWholeNoteAsBrailleString (
      inputLineNumber,
      multipleRestMeasureSoundingNotes);

  if (restMeasuresNumber > 1) {
    fBrailleCodeIOstream <<
      "*" <<
      restMeasuresNumber;
  }

  if (gBrailleOptions->fNoteInputLineNumbers) {
    // print the multiple rest line number as a comment
    fBrailleCodeIOstream <<
      " %{ " << inputLineNumber << " %} ";
  }

  fBrailleCodeIOstream <<    
    " | % " <<
    elt->getMultipleRestNextMeasureNumber () <<
    endl;
* /

  fOnGoingMultipleRestMeasures = false;
}

void bsr2BrailleTranslator::visitStart (S_msrMultipleRestContents& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> Start visiting msrMultipleRestContents" <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrMultipleRestContents& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "%--> End visiting msrMultipleRestContents" <<
      endl;
  }
}

//________________________________________________________________________
void bsr2BrailleTranslator::visitStart (S_msrMidi& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> Start visiting msrMidi" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  if (gBrailleOptions->fNoMidi) {
    fBrailleCodeIOstream <<
      "%{" <<
      endl;

     gIndenter++;
  }

  fBrailleCodeIOstream <<
    "\\midi" << " {" <<
    endl;
  
  gIndenter++;
  
  if (gBrailleOptions->fNoMidi) {
    fBrailleCodeIOstream <<
      "% ";
  }
  
  fBrailleCodeIOstream <<
    "\\tempo " <<
    elt->getMidiTempoDuration () <<
    " = " <<
    elt->getMidiTempoPerSecond () <<
    endl;
  
  gIndenter--;
  
  fBrailleCodeIOstream <<
    "}" <<
    endl;

  if (gBrailleOptions->fNoMidi) {
    gIndenter--;

    fBrailleCodeIOstream <<
      "%}" <<
      endl;
  }
}

void bsr2BrailleTranslator::visitEnd (S_msrMidi& elt)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    fBrailleCodeIOstream <<
      "% --> End visiting msrMidi" <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }
}
*/


} // namespace
