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

#include <sstream>
#include <stdlib.h>     /* abort, NULL */
#include <climits>      /* INT_MIN */
#include <algorithm>    /* for_each */
#include <iomanip>      // setw, set::precision, ...

#include "conversions.h"

#include "lpsr2LilypondTranslator.h"


using namespace std;

namespace MusicXML2
{

const int commentFieldWidth = 30;

//________________________________________________________________________
lpsr2LilypondTranslator::lpsr2LilypondTranslator (
  S_msrOptions&  msrOpts,
  S_lpsrOptions& lpsrOpts,
  ostream&       os,
  S_lpsrScore    lpScore)
    : fOstream (os)
{
  fMsrOptions       = msrOpts;
  fLpsrOptions      = lpsrOpts;
  
  // the LPSR score we're visiting
  fVisitedLpsrScore = lpScore;

  // inhibit the browsing of measure repeats replicas,
  // since Lilypond only needs the repeat measure
  fVisitedLpsrScore->
    getMsrScore ()->
      setInhibitMeasureRepeatReplicasBrowsing ();

  // inhibit the browsing of multiple rest replicas,
  // since Lilypond only needs the measure number
  fVisitedLpsrScore->
    getMsrScore ()->
      setInhibitMultipleRestMeasuresBrowsing ();

  // header handling
  fOnGoingHeader = false;

  // time
  fVoiceIsCurrentlySenzaMisura = false;
  fOnGoingVoiceCadenza = false;
  
  // staves
  fOnGoingStaff = false;

  // voices
  fOnGoingVoice = false;
  fOnGoingHarmonyVoice = false;

  // repeats
  fCurrentRepeatEndingsNumber = 0;

  // measures
  fMeasuresCounter = 0;
  
  // notes
  fOnGoingNote = false;

  // stems
  fCurrentStemKind = msrStem::k_NoStem;
//  fOnGoingStemNone = false; JMI

  // double tremolos
  fCurrentDoubleTremoloElementsLpsrDuration = -39;

  // chords
  fOnGoingChord = false; // JMI

  // stanzas
  fOngoingNonEmptyStanza = false;

  // score blocks
  fOnGoingScoreBlock = false;

  // limiting output line size
  fStanzaOlec.setMaxElementsPerLine (10);
  
  fMusicOlec.setMaxElementsPerLine (
    gLilypondOptions->fInputLineNumbers
      ?  5
      : 10);
};
  
lpsr2LilypondTranslator::~lpsr2LilypondTranslator () {}

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
  // generate LilyPond absolute octave
  switch (absoluteOctave) {
    case 0:
      return ",,,";
      break;
    case 1:
      return ",,";
      break;
    case 2:
      return ",";
      break;
    case 3:
      return "";
      break;
    case 4:
      return "'";
      break;
    case 5:
      return "''";
      break;
    case 6:
      return "'''";
      break;
    case 7:
      return "''''";
      break;
    case 8:
      return "'''''";
      break;
    default:
      return "###";
  } // switch
}

//________________________________________________________________________
string lpsr2LilypondTranslator::alterationAsLilypondString (
  msrAlteration alteration)
{
  string result;
  
  switch (alteration) {
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
    case k_NoAlteration:
      result = "alteration???";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
string lpsr2LilypondTranslator::lilypondizeDurationString (
  string msrDurationString)
{
  string result = msrDurationString;

  if (! isdigit (result [0])) {
    result [0] = tolower (result [0]);
    result = "\\" + result;
  }

  return result;
}

string lpsr2LilypondTranslator::divisionsAsLilypondString (
  int       inputLineNumber,
  S_msrPart part,
  int       divisions)
{
  string result;

  msrAssert(part != 0, "part == 0"); // JMI
  result =
    part->
      divisionsAsMsrString (
        inputLineNumber,
        divisions);

  return lilypondizeDurationString (result);
}

string lpsr2LilypondTranslator::tupletDivisionsAsLilypondString (
  int       inputLineNumber,
  S_msrPart part,
  int       divisions,
  int       actualNotes,
  int       normalNotes)
{
  string result;

  msrAssert(part != 0, "part == 0"); // JMI
  
  result =
    part->
      tupletDivisionsAsMsrString (
        inputLineNumber,
        divisions,
        actualNotes,
        normalNotes);

  return lilypondizeDurationString (result);
}

//________________________________________________________________________
string lpsr2LilypondTranslator::noteAsLilypondString (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();
    
  stringstream s;

  // is the note unpitched?
  if (note->getNoteIsUnpitched ())
    s <<
      "\\once \\override NoteHead #'style = #'cross ";

  // generate the pitch in all cases
  s <<
    msrQuartertonesPitchAsString (
      gLpsrOptions->fLpsrQuatertonesPitchesLanguage,
      note->getQuatertonesPitch ());
  
  msrQuartertonesPitch
    noteQuartertonesPitch =
      note->getQuatertonesPitch ();
              
  // in MusicXML, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();

  // should an absolute octave be generated?
  bool genAbsoluteOctave =
    gLilypondOptions->fAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;

  if (gGeneralOptions->fTraceNotes) {
    const int fieldWidth = 28;

    cerr << left <<
      endl <<
      idtr <<
          setw(fieldWidth) <<
          "% line" <<
          " = " <<
          note->getInputLineNumber () <<
          endl <<
      idtr <<
        setw(fieldWidth) <<
        "% msrQuartertonesPitch" <<
        " = - " <<
        msrQuartertonesPitchAsString (
          gLpsrOptions->fLpsrQuatertonesPitchesLanguage,
          noteQuartertonesPitch) <<
        " -" <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "% noteAbsoluteOctave" <<
        " = - " <<
        noteAbsoluteOctave <<
        " -" <<
        endl <<
      endl;
  }

  if (genAbsoluteOctave) {
    
    // generate LilyPond absolute octave
    s <<
      absoluteOctaveAsLilypondString (
        noteAbsoluteOctave);

  }

  else {
    
    // generate LilyPond octave relative to fRelativeOctaveReference

    msrDiatonicPitch
      noteDiatonicPitch =
        note->
          noteDiatonicPitch (
            inputLineNumber);

    msrDiatonicPitch
      referenceDiatonicPitch =
        fRelativeOctaveReference->
          noteDiatonicPitch (
            inputLineNumber);

    string
      referenceDiatonicPitchAsString =
        fRelativeOctaveReference->
          noteDiatonicPitchAsString (
            inputLineNumber);
        
    int
      referenceAbsoluteOctave =
        fRelativeOctaveReference->getNoteOctave ();

    /*
      If no octave changing mark is used on a pitch, its octave is calculated
      so that the interval with the previous note is less than a fifth.
      This interval is determined without considering accidentals.
    */
      
    int
      noteAboluteDiatonicOrdinal =
        noteAbsoluteOctave * 7
          +
        noteDiatonicPitch - kC,
        
      referenceAboluteDiatonicOrdinal =
        referenceAbsoluteOctave * 7
          +
        referenceDiatonicPitch - kC;

    if (gGeneralOptions->fTraceNotes) {
      const int fieldWidth = 28;

      cerr << left <<
/*
        idtr <<
          setw(fieldWidth) <<
          "% referenceDiatonicPitch" <<
          " = " <<
          referenceDiatonicPitch <<
          endl <<
*/
        idtr <<
          setw(fieldWidth) <<
          "% referenceDiatonicPitchAsString" <<
          " = " <<
          referenceDiatonicPitchAsString <<
          endl <<
        idtr <<
          setw(fieldWidth) <<
          "% referenceAbsoluteOctave" <<
           " = " <<
           referenceAbsoluteOctave <<
           endl <<
        endl <<
        idtr <<
          setw(fieldWidth) <<
          "% referenceAboluteDiatonicOrdinal" <<
          " = " <<
          referenceAboluteDiatonicOrdinal <<
          endl <<
        idtr <<
          setw(fieldWidth) <<
          "% noteAboluteDiatonicOrdinal" <<
          " = " <<
          noteAboluteDiatonicOrdinal <<
          endl <<
        endl;
    }

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
  }

  return s.str();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::noteSoundingDivisionsAsLpsrString (
  S_msrNote note)
{
  string
    noteSoundingDivisionsAsMsrString =
      note->noteSoundingDivisionsAsMsrString ();

  string
    lilypondDivisionsAsString =
      noteSoundingDivisionsAsMsrString;

  if (! isdigit (lilypondDivisionsAsString [0])) {
    if      (lilypondDivisionsAsString == "Breve")
      lilypondDivisionsAsString = "\\breve";
    else if (lilypondDivisionsAsString == "Long") // OK??? JMI
      lilypondDivisionsAsString = "\\longa";
    else if (lilypondDivisionsAsString == "Maxima")
      lilypondDivisionsAsString = "\\maxima";
  }

  return lilypondDivisionsAsString;
}
        
//________________________________________________________________________
string lpsr2LilypondTranslator::technicalKindAsLilypondString (
  S_msrTechnical technical)
{
  string result;

  /* JMI
  string
    noteDuration =
      (*i)->getTechnicalNoteUplink ()->
        noteSoundingDivisionsAsMsrString ();
  */

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
string lpsr2LilypondTranslator::technicalWithIntegerKindAsLilypondString (
  S_msrTechnicalWithInteger technicalWithInteger)
{
  stringstream s;

  switch (technicalWithInteger->getTechnicalWithIntegerKind ()) {
    case msrTechnicalWithInteger::kBend:
      s <<
        "\\bendAfter" " " <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
    case msrTechnicalWithInteger::kFingering:
      s <<
        "-" " " <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
    case msrTechnicalWithInteger::kFret:
      // LilyPond will take care of that JMI
      break;
    case msrTechnicalWithInteger::kString:
      s << // no space is allowed between the backslash and the number
        "\\" <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
  } // switch
     
  return s.str();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::technicalWithStringKindAsLilypondString (
  S_msrTechnicalWithString technicalWithString)
{
  string result;
  
  switch (technicalWithString->getTechnicalWithStringKind ()) {
    case msrTechnicalWithString::kHammerOn:
      result = "%{\\HammerOn???%}";
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
      result = "%{\\PullOff???%}";
      break;
  } // switch

  result +=
    string (" ") +
    "-\\markup {\"" + technicalWithString->
      getTechnicalWithStringValue () +
      "\"}";
    
  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::ornamentKindAsLilypondString (
  int                          inputLineNumber,
  msrOrnament::msrOrnamentKind ornamentKind,
  string                       noteUplinkDuration)
{
  string result;
  
  switch (ornamentKind) {
    case msrOrnament::kTrillMark:
      result = "\\trill";
      break;
      
    case msrOrnament::kWavyLine:
      result = "%{\\wavy line???%}";
      break;
      
    case msrOrnament::kTurn:
      result = "\\turn";
      break;
      
    case msrOrnament::kInvertedTurn:
      result = "\\reverseturn";
      break;
      
    case msrOrnament::kDelayedTurn:
      {
        // c2*2/3 ( s2*1/3\turn
        stringstream s;

        s <<
          "s" <<
          noteUplinkDuration <<
          "*" <<
          gLilypondOptions->fDelayedOrnamentFractionDenominator
            -
          gLilypondOptions->fDelayedOrnamentFractionNumerator <<
          "/" <<
          gLilypondOptions->fDelayedOrnamentFractionDenominator <<
          "\\turn";
          
        result = s.str ();
      }
      break;
      
    case msrOrnament::kDelayedInvertedTurn:
      {
/* JMI
        stringstream s;

        s <<
          "delayed inverted turn is not supported, replaced by inverted turn," <<
          endl <<
          "see http://lilypond.org/doc/v2.18/Documentation/snippets/expressive-marks";
        
        lpsrMusicXMLWarning (
          inputLineNumber,
          s.str ());
          
        result = "\\reverseturn %{ " + s.str () + " %}";
*/
        // c2*2/3 ( s2*1/3\turn
        stringstream s;

        s <<
          "s" <<
          noteUplinkDuration <<
          "*1/3" "\\reverseturn";
          
        result = s.str ();
      }
      break;
      
    case msrOrnament::kVerticalTurn:
      result =
        "^\\markup { \\rotate #90 \\musicglyph #\"scripts.turn\" }";
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
      
    case msrOrnament::kMordent:
      result = "\\mordent";
      break;
      
    case msrOrnament::kInvertedMordent:
      result = "%{\\inverted mordent???%}";
      break;
      \
    case msrOrnament::kSchleifer:
      result = "%{\\schleifer???%}";
      break;
      
    case msrOrnament::kShake:
      result = "%{\\shake???%}";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::singleTremoloDurationAsLilypondString (
  int         inputLineNumber,
  msrDuration singleTremoloDuration,
  int         singleTremoloMarksNumber)
{  
  /*
  The same output can be obtained by adding :N after the note,
  where N indicates the duration of the subdivision (it must be at least 8).
  If N is 8, one beam is added to the note’s stem.
  */
  
  int durationToUse =
    singleTremoloMarksNumber; // JMI / singleTremoloNoteSoundingDivisions;
        
  if (singleTremoloDuration >= kEighth)
    durationToUse +=
      1 + (singleTremoloDuration - kEighth);
  
  stringstream s;

  s <<
    ":" <<
    int (pow (2, durationToUse + 2)) <<
    " ";
    
  return s.str();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::harmonyAsLilypondString (
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  stringstream s;

  s <<
    msrQuartertonesPitchAsString (
      gMsrOptions->fMsrQuatertonesPitchesLanguage,
      harmony->getHarmonyRootQuartertonesPitch ()) <<                     
    harmony->getHarmonyDirectPartUplink ()->
      divisionsAsMsrString (
        inputLineNumber,
        harmony->getHarmonySoundingDivisions ());
    
  switch (harmony->getHarmonyKind ()) {
    case msrHarmony::kMajor:
      break;
    case msrHarmony::kMinor:
      s << ":m";
      break;
    case msrHarmony::kDominant:
      s << ":7";
      break;
    case msrHarmony::kAugmented:
      s << ":aug";
      break;
    case msrHarmony::kDiminished:
      s << ":dim";
      break;
    case msrHarmony::kSuspendedFourth:
      s << ":sus4";
      break;
    case msrHarmony::kMajorSeventh:
      s << ":maj7";
      break;
    case msrHarmony::kMinorSeventh:
      s << ":m7";
      break;
    case msrHarmony::kMajorNinth:
      s << ":9";
      break;
    case msrHarmony::kMinorNinth:
      s << ":m9";
      break;
    case msrHarmony::k_NoHarmony:
      s << ":Harmony???";
      break;
  } // switch

  msrQuartertonesPitch
    harmonyBassQuartertonesPitch =
      harmony->getHarmonyBassQuartertonesPitch ();
      
  if (harmonyBassQuartertonesPitch != k_NoQuaterTonesPitch)
    s <<
      "/" <<
      msrQuartertonesPitchAsString (
        gMsrOptions->fMsrQuatertonesPitchesLanguage,
        harmonyBassQuartertonesPitch);

  return s.str();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrScore" <<
      endl;

  // initial empty line in LilyPond code
  // to help copy/paste it
// JMI  fOstream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrScore" <<
      endl;

  // final empty line in LilyPond code
  // to help copy/paste it
  fOstream << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrLilypondVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrLilypondVarValAssoc" <<
      endl;

  fOstream << idtr;

  if (elt->getComment ().size())
    fOstream <<
      "% " << elt->getComment () << endl <<
      idtr;

  if (elt->getCommentedKind () == lpsrLilypondVarValAssoc::kCommented)
    fOstream << "\%";
  
  switch (elt->getBackslashKind ()) {
    case lpsrLilypondVarValAssoc::kWithBackslash:
      fOstream << "\\";
      break;
    case lpsrLilypondVarValAssoc::kWithoutBackslash:
      break;
  } // switch

  // the largest variable name length in a header is 18
  int fieldWidth;
  
  if (fOnGoingHeader)
    fieldWidth = 18;
  else
    fieldWidth = elt->getVariableName ().size();
    
  fOstream <<
    left<< setw(fieldWidth) << elt->getVariableName ();
  
  if (elt->getVarValSeparator () == lpsrLilypondVarValAssoc::kEqualSign)
    fOstream << " = ";
  else
    fOstream << " ";
  
  if (elt->getQuotesKind () == lpsrLilypondVarValAssoc::kQuotesAroundValue)
    fOstream << "\"";
    
  fOstream <<
    elt->getVariableValue ();

  if (elt->getUnit ().size())
    fOstream <<
      "\\" <<
      elt->getUnit ();
  
  if (elt->getQuotesKind () == lpsrLilypondVarValAssoc::kQuotesAroundValue)
    fOstream << "\"";
  
  fOstream << endl;

  switch (elt->getEndlKind ()) {
    case lpsrLilypondVarValAssoc::kWithEndl:
      fOstream << endl;
      break;
      
    case lpsrLilypondVarValAssoc::kWithEndlTwice:
      fOstream << endl << endl;
      break;
      
    case lpsrLilypondVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrLilypondVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrLilypondVarValAssoc" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrSchemeVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrSchemeVarValAssoc" <<
      endl;

  fOstream << idtr;

  if (elt->getComment ().size())
    fOstream <<
      "% " << elt->getComment () << endl <<
      idtr;

  if (elt->getCommentedKind () == lpsrSchemeVarValAssoc::kCommented)
    fOstream << "\% ";
  
  fOstream <<
    "#(" <<
    elt->getVariableName () <<
    " " <<
    elt->getVariableValue () <<
    ")";
    
  switch (elt->getEndlKind ()) {
    case lpsrSchemeVarValAssoc::kWithEndl:
      fOstream << endl;
      break;
    case lpsrSchemeVarValAssoc::kWithEndlTwice:
      fOstream << endl << endl;
      break;
    case lpsrSchemeVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrSchemeVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrSchemeVarValAssoc" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrHeader& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrHeader" <<
      endl;

  fOstream << idtr <<
    "\\header" << " {" <<
    endl;

  idtr++;

  fOnGoingHeader = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrHeader& elt)
{
  fOnGoingHeader = false;

  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrHeader" <<
      endl;

  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPaper& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrPaper" <<
      endl;

  fOstream << idtr <<
    "\\paper" << " {" <<
    endl;

  idtr++;
  
  const int fieldWidth = 20;

  // page width, height and margins

  {
    float paperWidth =
      elt->getPaperWidth ();
      
    if (paperWidth > 0) {
      fOstream <<
        idtr << left <<
          setw(fieldWidth) <<
          "paper-width" << " = " <<
          setprecision(4) << paperWidth << "\\cm" <<
          endl;
    }
  }
  
  {
    float paperHeight =
      elt->getPaperHeight ();
      
    if (paperHeight > 0) {
      fOstream <<
        idtr << left <<
          setw(fieldWidth) <<
          "paper-height" << " = " <<
          setprecision(4) << paperHeight << "\\cm" <<
          endl;
    }
  }
  
  {
    float topMargin =
      elt->getTopMargin ();
      
    if (topMargin > 0) {
      fOstream <<
        idtr << left <<
          setw(fieldWidth) <<
          "top-margin" << " = " <<
          setprecision(4) << topMargin << "\\cm" <<
          endl;
    }
  }

  {
    float bottomMargin =
      elt->getBottomMargin ();
      
    if (bottomMargin > 0) {
      fOstream <<
        idtr << left <<
          setw(fieldWidth) <<
          "bottom-margin" << " = " <<
          setprecision(4) << bottomMargin << "\\cm" <<
          endl;
    }
  }

  {
    float leftMargin =
      elt->getLeftMargin ();
      
    if (leftMargin > 0) {
      fOstream <<
        idtr << left <<
          setw(fieldWidth) <<
          "left-margin" << " = " <<
          setprecision(4) << leftMargin << "\\cm" <<
          endl;
    }
  }

  {
    float rightMargin =
      elt->getRightMargin ();
      
    if (rightMargin > 0) {
      fOstream <<
        idtr << left <<
          setw(fieldWidth) <<
          "right-margin" << " = " <<
        setprecision(4) << rightMargin << "\\cm" <<
        endl;
    }
  }

  // spaces

  {
    float betweenSystemSpace =
      elt->getBetweenSystemSpace ();
      
    if (betweenSystemSpace > 0) {
      fOstream <<
        idtr << left <<
        setw(fieldWidth) <<
        "between-system-space" << " = " <<
        setprecision(4) << betweenSystemSpace << "\\cm" <<
        endl;
    }
  }

  {
    float pageTopSpace =
      elt->getPageTopSpace ();
      
    if (pageTopSpace > 0) {
      fOstream <<
        idtr << left <<
        setw(fieldWidth) <<
        "page-top-space" << " = " <<
        setprecision(4) << pageTopSpace << "\\cm" <<
        endl;
    }
  }

  // headers and footers

  {
    string oddHeaderMarkup =
      elt->getOddHeaderMarkup ();
      
    if (oddHeaderMarkup.size ()) {
      fOstream <<
        idtr << left <<
        setw(fieldWidth) <<
        "oddHeaderMarkup" << " = " <<
        oddHeaderMarkup <<
        endl;
    }
  }

  {
    string evenHeaderMarkup =
      elt->getEvenHeaderMarkup ();
      
    if (evenHeaderMarkup.size ()) {
      fOstream <<
        idtr << left <<
        setw(fieldWidth) <<
        "evenHeaderMarkup" << " = " <<
        evenHeaderMarkup <<
        endl;
    }
  }

  {
    string oddFooterMarkup =
      elt->getOddFooterMarkup ();
      
    if (oddFooterMarkup.size ()) {
      fOstream <<
        idtr << left <<
        setw(fieldWidth) <<
        "oddFooterMarkup" << " = " <<
        oddFooterMarkup <<
        endl;
    }
  }

  {
    string evenFooterMarkup =
      elt->getEvenFooterMarkup ();
      
    if (evenFooterMarkup.size ()) {
      fOstream <<
        idtr << left <<
        setw(fieldWidth) <<
        "evenFooterMarkup" << " = " <<
        evenFooterMarkup <<
        endl;
    }
  }

  // generate the default 'indent' setting ready for the user
  fOstream <<
    idtr << left <<
      setw(fieldWidth) <<
      "indent" << " = " <<
    setprecision(4) << 1.5 << "\\cm" <<
    endl;

  // generate the default 'short-indent' setting ready for the user
  fOstream <<
    idtr << left <<
      setw(fieldWidth) <<
      "short-indent" << " = " <<
    setprecision(4) << 1.0 << "\\cm" <<
    endl;

  fOstream << endl;

  // generate a 'page-count' comment ready for the user
  fOstream <<
    idtr << left <<
      setw(fieldWidth) <<
      "%" "page-count" << " = " <<
    setprecision(4) << 1 <<
    endl;

  // generate a 'system-count' comment ready for the user
  fOstream <<
    idtr << left <<
      setw(fieldWidth) <<
      "%" "system-count" << " = " <<
    setprecision(4) << 1 <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPaper& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrPaper" <<
      endl;
      
  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrLayout& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrLayout" <<
      endl;

  fOstream <<
    idtr << "\\layout" << " {" <<
    endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrLayout& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrLayout" <<
      endl;

  if (gLilypondOptions->fRepeatBrackets) {
    fOstream <<
      idtr <<
      "\\context" " " "{" <<
      endl;
    
    idtr++;

    fOstream <<
      idtr <<
        "\\Score" <<
        endl <<
      idtr <<
        "% defaultBarType = #\"!\"" <<
        endl <<
      idtr <<
        "startRepeatType = #\"[|:\"" <<
        endl <<
      idtr <<
        "endRepeatType = #\":|]\"" <<
        endl <<
      idtr <<
        "doubleRepeatType = #\":|][|:\"" <<
        endl;

    idtr--;
        
    fOstream <<
      idtr <<
        "}" <<
        endl;
  }

  if (true) { // JMI XXL
    fOstream <<
      idtr <<
      "\\context" " " "{" <<
      endl;
  
    idtr++;
  
    fOstream <<
      idtr <<
        "\\Staff" <<
        endl <<
      idtr <<
        "\\consists \"Span_arpeggio_engraver\"" <<
        endl;

    idtr--;

    fOstream <<
      idtr <<
        "}" <<
        endl;
  }
    
  idtr--;

  fOstream << idtr <<
    "}" <<
    endl <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrScoreBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrScoreBlock" <<
      endl;

  fOstream << idtr <<
    "\\score {" <<
    endl;

  idtr++;
/*
  if (elt->getScoreBlockElements ().size()) {
    fOstream <<
      idtr << "<<" <<
      endl;
  
    idtr++;
  }
*/
  fOnGoingScoreBlock = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrScoreBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrScoreBlock" <<
      endl;
/*
  if (elt->getScoreBlockElements ().size()) {
    idtr--;
    
    fOstream <<
      idtr << ">>" <<
      endl << endl;
  }
*/
  idtr--;
  
  fOstream << idtr <<
    "}" <<
    endl;

  fOnGoingScoreBlock = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrParallelMusic& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrParallelMusic" <<
      endl;

  if (elt->getParallelMusicElements ().size()) { // JMI
    fOstream <<
      idtr << "<<" <<
      endl;
  
    idtr++;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrParallelMusic& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrParallelMusic" <<
      endl;

  if (elt->getParallelMusicElements ().size()) { // JMI
    idtr--;
    
    fOstream <<
      idtr << ">>" <<
      endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPartGroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrPartGroupBlock" <<
      endl;

  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();
      
  msrPartGroup::msrPartGroupSymbolKind
    partGroupSymbolKind =
      partGroup->getPartGroupSymbolKind ();

  bool
    partGroupBarline =
      partGroup->getPartGroupBarline ();
      
  string
    partGroupInstrumentName =
      partGroup->getPartGroupInstrumentName ();
      
  string partGroupContextName;

  // LPNR, page 567

  // don't generate code for a 1-element part group block
  if (elt->getPartGroupBlockElements ().size () > 1) {
    switch (partGroupSymbolKind) {
      case msrPartGroup::k_NoPartGroupSymbol:
        partGroupContextName = "";
        break;
        
      case msrPartGroup::kBracePartGroupSymbol: // JMI
      /*
       *
       * check whether individual part have instrument names
       * 
        if (partGroupInstrumentName.size ())
          partGroupContextName = "\\new PianoStaff";
        else
          partGroupContextName = "\\new GrandStaff";
          */
        partGroupContextName =
          partGroupBarline // only partGroup has an instrument name
            ? "\\new PianoStaff"
            : "\\new GrandStaff";
        break;
        
      case msrPartGroup::kBracketPartGroupSymbol:
        partGroupContextName =
          partGroupBarline
            ? "\\new StaffGroup"
            : "\\new ChoirStaff";
        break;
        
      case msrPartGroup::kLinePartGroupSymbol:
        partGroupContextName = "\\new StaffGroup";
        break;
        
      case msrPartGroup::kSquarePartGroupSymbol:
        partGroupContextName = "\\new StaffGroup";
        break;
    } // switch
  
    fOstream << idtr <<
      setw(commentFieldWidth) <<
      partGroupContextName + " " "<<";
      
    if (gLilypondOptions->fComments)
      fOstream <<
        "% part group " <<
        partGroup->getPartGroupCombinedName ();
        
    fOstream <<
      endl;
  }

  if (partGroupInstrumentName.size ())
    fOstream << idtr <<
      "\\set PianoStaff.instrumentName = \"" <<
      partGroupInstrumentName <<
      "\"" <<
      endl;

    bool doConnectArpeggios = true; // JMI
    
    if (doConnectArpeggios)
      fOstream << idtr <<
        "\\set PianoStaff.connectArpeggios = ##t" <<
        endl;
  
    if (partGroupSymbolKind == msrPartGroup::kSquarePartGroupSymbol) {
      idtr++;
      fOstream << idtr <<
        "\\set StaffGroup.systemStartDelimiter = #'SystemStartSquare" <<
        endl;
      idtr--;
    }
       
  fOstream <<
    endl;

  if (elt->getPartGroupBlockElements ().size () > 1)
    idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPartGroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrPartGroupBlock" <<
      endl;

  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();
      
  if (elt->getPartGroupBlockElements ().size () > 1)
    idtr--;

  // don't generate code for a 1-element part group block
  if (elt->getPartGroupBlockElements ().size () > 1) {
    fOstream <<
      idtr <<
      setw(commentFieldWidth) << ">>";
      
    if (gLilypondOptions->fComments)
      fOstream <<
         "% part group " <<
        partGroup->getPartGroupCombinedName ();
        
    fOstream <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPartBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrPartBlock" <<
      endl;

  // fetch part block's part
  S_msrPart
    part =
      elt->getPart ();
      
  if (part->getPartStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave

    string
      partName =
        part->getPartName (),
      partAbbreviation =
        part->getPartAbbreviation ();
        /*
      partInstrumentName =  // JMI
        part->getPartInstrumentName (),
      partInstrumentAbbreviation =  // JMI
        part->getPartInstrumentAbbreviation ();
        */
  
    if (gLilypondOptions->fComments) {
      fOstream << left <<
        idtr <<
        setw(commentFieldWidth) <<
        "\\new PianoStaff <<" <<
        "% part " << part->getPartCombinedName ();
    }
    else {
      fOstream << idtr <<
        "\\new PianoStaff" <<  " " "<<";      
    }
    fOstream <<
      endl;
    
    idtr++;

    if (partName.size ()) {
      fOstream << idtr <<
        "\\set PianoStaff.instrumentName = \"" <<
        partName <<
        "\"" <<
        endl;
    }
    
    if (partAbbreviation.size ()) {
      fOstream << idtr <<
        "\\set PianoStaff.shortInstrumentName = \"" <<
        partAbbreviation <<
        "\"" <<
        endl;
    }

    bool doConnectArpeggios = true; // JMI
    
    if (doConnectArpeggios) {
      fOstream << idtr <<
        "\\set PianoStaff.connectArpeggios = ##t" <<
        endl;
    }
       
    fOstream <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPartBlock& elt)
{
  // fetch current part block's part
  S_msrPart
    part =
      elt->getPart ();
      
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrPartBlock" <<
      endl;

  if (part->getPartStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave

    idtr--;
  
    if (gLilypondOptions->fComments) {
      fOstream <<
        idtr <<
        setw(commentFieldWidth) << ">>" <<    
        "% part " <<
        part->getPartCombinedName ();
    }
    else {
      fOstream <<
        idtr <<
        ">>";
    }
    
    fOstream <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrStaffBlock" <<
      endl;

  S_msrStaff
    staff =
      elt->getStaff ();
      
  string staffContextName;
  
  switch (staff->getStaffKind ()) {
    case msrStaff::kRegularStaff:
      staffContextName = "Staff";
      break;
      
    case msrStaff::kTablatureStaff:
      staffContextName = "TabStaff";
      break;
      
    case msrStaff::kPercussionStaff:
      staffContextName = "DrumStaff";
      break;
      
    case msrStaff::kHarmonyStaff:
      staffContextName = "kHarmonyStaff???";
      break;
  } // switch

  string newContext;

  newContext =
    "\\new " + staffContextName + " <<" ;
      
  if (gLilypondOptions->fComments) {
    fOstream << left <<
      idtr <<
      setw(commentFieldWidth) <<
       newContext <<
      "% staff \"" << staff->getStaffName () << "\"";
  }
  else {
    fOstream << idtr <<
      newContext;   
  }
      
  fOstream <<
    endl;

  idtr++;

/* JMI
  // fetch part
  S_msrPart
    part =
      staff->getStaffDirectPartUplink ();
*/

  string
    staffBlockInstrumentName =
      elt->getStaffBlockInstrumentName (),
    staffShortInstrumentName =
      elt->getStaffBlockShortInstrumentName ();

  if (staffBlockInstrumentName.size ())
    fOstream << idtr <<
      "\\set " << staffContextName << ".instrumentName = \"" <<
      staffBlockInstrumentName <<
      "\"" <<
      endl;

  if (staffShortInstrumentName.size ())
    fOstream << idtr <<
      "\\set " << staffContextName << ".shortInstrumentName = \"" <<
      staffShortInstrumentName <<
      "\"" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrStaffBlock" <<
      endl;

  idtr--;

  if (gLilypondOptions->fComments) {
    fOstream <<
      idtr <<
      setw(commentFieldWidth) << ">>" <<    
      "% staff " <<
      elt->getStaff ()->getStaffName ();
  }
  else {
    fOstream <<
      idtr <<
      ">>";
  }

  fOstream <<
    endl <<
    endl;
}

/*
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewStaffgroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffgroupBlock" <<
      endl;

   fOstream << idtr <<
     "\\new StaffGroup" << " " << "{" <<
      endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewStaffgroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffgroupBlock" <<
      endl;

  idtr--;

  fOstream <<
    " }" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffBlock" <<
      endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffBlock" <<
      endl;

  idtr--;
}
*/

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrUseVoiceCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrUseVoiceCommand" <<
      endl;

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
      
    case msrStaff::kPercussionStaff:
      staffContextName = "\\context DrumStaff";
      voiceContextName = "DrumVoice";
      break;
      
    case msrStaff::kHarmonyStaff:
      staffContextName = "\\context ChordNames";
      voiceContextName = "???"; // JMI
      break;
  } // switch

 // if (voice->getStaffRelativeVoiceNumber () > 0) { JMI
    // don't include the master voice in the staff by default
    
    fOstream << idtr <<
      "\\context " << voiceContextName << " = " "\"" <<
      voice->getVoiceName () << "\"" << " <<" <<
       endl;
  
    idtr++;
  
    if (gLilypondOptions->fNoAutoBeaming)
      fOstream << idtr <<
        "\\set " << staffContextName << ".autoBeaming = ##f" <<
        endl;
  
    if (staffKind == msrStaff::kRegularStaff) {
      if (staff->getStaffNumberOfMusicVoices () > 1) {
        fOstream << idtr;
        switch (voice->getStaffRelativeVoiceNumber ()) {
          case 1:
            fOstream << "\\voiceOne ";
            break;
          case 2:
            fOstream << "\\voiceTwo ";
            break;
          case 3:
            fOstream << "\\voiceThree ";
            break;
          case 4:
            fOstream << "\\voiceFour ";
            break;
          default:
            {}
        } // switch

        fOstream << "% " << staff->getStaffNumberOfMusicVoices () << " music voices" <<
      endl;
        fOstream << endl;
      }
    }
  
    fOstream <<
      idtr << "\\" << voice->getVoiceName () << endl;
  
    idtr--;
    
    fOstream <<
      idtr << ">>" <<
      endl;
 // }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrUseVoiceCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrUseVoiceCommand" <<
      endl;
}
  
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewLyricsBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewLyricsBlock" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    fOstream <<
      idtr <<
        "\\new Lyrics" <<
      endl;

    idtr++;
    
    fOstream <<
      idtr <<
        "\\lyricsto" " " <<
        "\""  << elt->getVoice ()->getVoiceName () << "\""  <<
        endl <<
      idtr <<
        "\\" << elt->getStanza ()->getStanzaName () <<
      endl;

    idtr--;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewLyricsBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrNewLyricsBlock" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    // JMI
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrVariableUseCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrVariableUseCommand" <<
      endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrVariableUseCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrVariableUseCommand" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrContext" <<
      endl;

  string contextName =
    elt->getContextName ();
    
  fOstream << idtr <<
    "\\context" " " << elt->getContextType () <<
    " = \"" << contextName << "\"" <<
    endl;

  idtr++;

  fOstream << idtr <<
    "\\" << contextName <<
    endl << endl;
    
  idtr--;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrContext" <<
      endl;
}
  
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrBarCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrBarCommand" <<
      endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrBarCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrBarCommand" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrMelismaCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrMelismaCommand" <<
      endl;

  switch (elt->getMelismaKind ()) {
    case lpsrMelismaCommand::kMelismaStart:
      fOstream << "\\melisma" " ";
      break;
    case lpsrMelismaCommand::kMelismaEnd:
      fOstream << "\\melismaEnd" " ";
      break;
  } // switch

  fMusicOlec++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrMelismaCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrMelismaCommand" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrScore" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrScore" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrCredit" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrCredit" <<
      endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrCreditWords" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrCreditWords" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPartGroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrPartGroup" <<
      elt->getPartGroupCombinedName () <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrPartGroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrPartGroup" <<
      elt->getPartGroupCombinedName () <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrPart" <<
      elt->getPartCombinedName () <<
      endl;

  fCurrentPart = elt;
}

void lpsr2LilypondTranslator::visitEnd (S_msrPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrPart" <<
      elt->getPartCombinedName () <<
      endl;

  fCurrentPart = 0;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStaff& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      endl;
  
  fOnGoingStaff = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrStaff& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      endl;
  
  fOnGoingStaff = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStaffLinesNumber& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "%--> Start visiting msrStaffLinesNumber" <<
      endl;

  // fetch staff lines number
  int
    linesNumber =
      elt->getLinesNumber ();

  if (linesNumber != 5) { // default value
    int saveIndent =
      idtr.getIndent ();
    
    fOstream <<
      endl <<
      idtr <<
      "\\stopStaff " <<
      "\\override Staff.StaffSymbol.line-count = " <<
      linesNumber <<
      " \\startStaff" <<
      endl;

    if (saveIndent > 0) {
      idtr.resetToZero ();
      
      for (int i = 0; i < saveIndent; i++) {
        fOstream <<
          idtr;
          
        saveIndent++;
      }
    }
  }
}

void lpsr2LilypondTranslator::visitStart (S_msrStaffTuning& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "%--> Start visiting msrStaffTuning" <<
      endl;

/* JMI
  list<S_msrStaffTuning>
    staffTuningsList =
      staff->getStaffTuningsList ();
      
  if (staffTuningsList.size ()) {
    // \set TabStaff.stringTunings = \stringTuning <c' g' d'' a''>

    fOstream << idtr <<
      "\\set TabStaff.stringTunings = \\stringTuning <";

    list<S_msrStaffTuning>::const_iterator
      iBegin = staffTuningsList.begin(),
      iEnd   = staffTuningsList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      fOstream <<
        msrQuartertonesPitchAsString (
          gLpsrOptions->fLpsrQuatertonesPitchesLanguage,
 // JMI            elt->getInputLineNumber (),
          ((*i)->getStaffTuningQuartertonesPitch ())) <<        
 // JMI       char (tolower ((*i)->getStaffTuningStep ())) <<
        absoluteOctaveAsLilypondString (
          (*i)->getStaffTuningOctave ());
      if (++i == iEnd) break;
      fOstream << " ";
    } // for

    fOstream <<
      ">" <<
      endl;
  }
 */
}

void lpsr2LilypondTranslator::visitStart (S_msrStaffDetails& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr << idtr <<
      "%--> Start visiting msrStaffDetails" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrVoice& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      endl;
 
  fOstream <<
    idtr <<
    elt->getVoiceName () << " = ";

  // generate the beginning of the voice definition
  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      if (! gLilypondOptions->fAbsoluteOctaves)
        fOstream <<
          "\\relative " << "{" <<
          endl;
      break;
      
    case msrVoice::kHarmonyVoice:
      fOstream <<
        "\\chordmode " << "{" <<
        endl;
      break;
      
    case msrVoice::kMasterVoice:
      // ?? JMI
      break;
  } // switch

  idtr++;

  if (gLilypondOptions->fGlobal) {
    fOstream <<
      idtr <<
        "\\global" <<
        endl <<
        endl;
  }
    
  if (gLilypondOptions->fDisplayMusic) {
    fOstream <<
      idtr <<
        "\\displayMusic {" <<
        endl;

    idtr++;
  }
    
  fOstream <<
    idtr <<
      "\\language \"" <<
      msrQuatertonesPitchesLanguageAsString (
        gLpsrOptions->fLpsrQuatertonesPitchesLanguage) <<
      "\"" <<
      endl;

  if (gLpsrOptions->fLpsrChordsLanguage != k_IgnatzekChords)
    fOstream <<
      idtr <<
        "\\" <<
        lpsrChordsLanguageAsString (
          gLpsrOptions->fLpsrChordsLanguage) <<
        "Chords" <<
        endl;

  if (gLilypondOptions->fShowAllBarNumbers)
    fOstream <<
      idtr <<
        "\\set Score.barNumberVisibility = #all-bar-numbers-visible" <<
        endl <<
      idtr <<
        "\\override Score.BarNumber.break-visibility = ##(#f #t #t)" <<
        endl <<
      endl;

// JMI   \set Score.alternativeNumberingStyle = #'numbers-with-letters


  if (gLilypondOptions->fCompressMultiMeasureRests)
    fOstream <<
      idtr <<
        "\\compressMMRests" <<
        endl <<
      endl;

  if (gLilypondOptions->fAccidentalStyle.size ())
    fOstream <<
      idtr <<
        "\\accidentalStyle Score." << gLilypondOptions->fAccidentalStyle <<
        endl <<
      endl;

  fRelativeOctaveReference = 0;

  fMusicOlec.resetToZero ();

  fVoiceIsCurrentlySenzaMisura = false;
  
  fOnGoingVoice = true;

  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      break;
      
    case msrVoice::kHarmonyVoice:
      fOnGoingHarmonyVoice = true;
      break;
      
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      endl;
  
  if (gLilypondOptions->fDisplayMusic) {
    fOstream <<
      idtr <<
        "}" <<
        endl;

    idtr--;
  }
    
  // generate the end of the voice definition
  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      fOstream << idtr <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kHarmonyVoice:
      fOstream << idtr <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kMasterVoice:
      // ?? JMI
      break;
  } // switch

  fOnGoingVoice = false;

  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      break;
      
    case msrVoice::kHarmonyVoice:
      fOnGoingHarmonyVoice = false;
      break;
      
    case msrVoice::kMasterVoice:
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrHarmony& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrHarmony '" <<
      elt->harmonyAsString () <<
      "'" <<
      endl;

  if (fOnGoingNote) {
    fOstream <<
      "%{ " << elt->harmonyAsString () << " %}" <<
      endl <<
      idtr;
  }
  
  else if (fOnGoingChord) { // JMI
    /*
    // register the harmony in the current chord clone
    fCurrentChord->
      setChordHarmony (elt); // JMI
      */
  }

  else if (fOnGoingHarmonyVoice) {
    fOstream <<
      harmonyAsLilypondString (elt) <<
      " ";
      
    if (gLilypondOptions->fInputLineNumbers)
      // print the harmony line number as a comment
      fOstream <<
        "%{ " << elt->getInputLineNumber () << " %} ";  
  }

/* JMI
  else if (fOnGoingChord) {
    // don't generate code for the code harmony,
    // this will be done after the chord itself JMI
  }
  */
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "%--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;

  if (gLilypondOptions->fComments) {
    fOstream << idtr <<
      setw(commentFieldWidth) <<
      "% start of segment" <<
      endl;

    idtr++;
  }

  fSegmentNotesAndChordsCountersStack.push (0);
}

void lpsr2LilypondTranslator::visitEnd (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "%--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;

  if (gLilypondOptions->fComments) {
    idtr--;
    
    fOstream <<
      idtr <<
      setw(commentFieldWidth) <<
      "% end of segment" <<
      endl;
  }
  /* JMI
  else
    fOstream <<
      endl;
*/

  fSegmentNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMeasure& elt)
{    
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureNumber ();

  // take this measure into account
  fMeasuresCounter++;
  
  // get measure kind
  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();
      
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrMeasure " <<
      measureNumber <<
      ", measureKind:" <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", line " << inputLineNumber <<
      endl;

  if (gLilypondOptions->fComments) {
    fOstream << idtr <<
      setw(commentFieldWidth) <<
      "% start of measure \"" << measureNumber << "\"" <<
      ", line " << inputLineNumber <<
      endl;

    idtr++;
  }

  // is this the end of a cadenza?
  if (
    fOnGoingVoiceCadenza
      &&
    measureKind != msrMeasure::kOverfullMeasureKind) {
    fOstream <<
      endl <<
      idtr <<
        "\\cadenzaOff" <<
        endl;

    fOnGoingVoiceCadenza = false;
  }

  fSegmentNotesAndChordsCountersStack.push (0);

  switch (elt->getMeasureKind ()) {
    case msrMeasure::kUnknownMeasureKind:
      {
        stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' is of unknown kind";

        msrInternalWarning (
          inputLineNumber, s.str ());
      }
      break;
      
    case msrMeasure::kFullMeasureKind:
      break;
      
    case msrMeasure::kUpbeatMeasureKind:
      {
        int measureLength =
          elt->getMeasureLength ();

        string
          partialDuration =
            divisionsAsLilypondString (
              inputLineNumber,
              elt->getMeasureDirectPartUplink (),
              measureLength);

        fOstream << idtr <<
          "\\partial" " " << partialDuration <<
          endl;
      }
      break;
      
    case msrMeasure::kUnderfullMeasureKind:
      {
        int
          mesurePosition =
            elt->getMeasurePosition (),
          measureDivisionsPerFullMeasure =
            elt->getMeasureDivisionsPerFullMeasure ();

        // we should set the score measure length in this case
        rational r (
          mesurePosition - 1, // positions start at 1
          measureDivisionsPerFullMeasure);
        r.rationalise ();
  
        if (gGeneralOptions->fTraceMeasures) {
          cerr <<
            idtr <<
              "% Setting the measure length for measure " <<
              measureNumber <<
              endl <<
            idtr <<
              ", line = " << inputLineNumber <<
              endl <<
            idtr <<
              "% mesurePosition              = " << mesurePosition <<
              endl <<
            idtr <<
              "% measureDivisionsPerFullMeasure  = " << measureDivisionsPerFullMeasure <<
              endl <<
            idtr <<
              "% r.getNumerator ()      = " << r.getNumerator () <<
              endl <<
            idtr <<
              "% r.getDenominator ()    = " << r.getDenominator () <<
              endl <<
            endl;
        }

        int numerator =
          r.getNumerator ();

        if (numerator <= 0) { // TEMP JMI ???
          // IGNORE
        }

        else {
          fOstream << idtr <<
            "\\set Score.measureLength = #(ly:make-moment " <<
            to_string (numerator) <<
            "/" <<
            to_string (r.getDenominator ()) <<
            ")" << // JMI
            endl;
    
          // should we generate a break?
          if (gLilypondOptions->fBreakLinesAtIncompleteRightMeasures)
            fOstream << idtr <<
              "\\break" <<
              endl;
        }
      }
      break;

    case msrMeasure::kOverfullMeasureKind:
      if (! fOnGoingVoiceCadenza) {
        fOstream << idtr <<
          "\\cadenzaOn" <<
          endl;
          
        fOnGoingVoiceCadenza = true;
      }
      break;
      
    case msrMeasure::kSenzaMisuraMeasureKind:
      if (! fOnGoingVoiceCadenza) {
        fOstream << idtr <<
          "\\cadenzaOn" <<
          endl;
          
        fOnGoingVoiceCadenza = true;
      }
      break;
      
    case msrMeasure::kEmptyMeasureKind:
      {
        int
          measureDivisionsPerFullMeasure =
            elt->getMeasureDivisionsPerFullMeasure ();

        // generate a rest the duration of the measure
        fOstream <<
          "R" <<
            divisionsAsLilypondString (
              inputLineNumber,
              elt->getMeasureDirectPartUplink (),
              measureDivisionsPerFullMeasure) <<
          " | ";
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
      elt->getMeasureNumber ();
    
  // get measure kind
  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();
      
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrMeasure " <<
      measureNumber <<
      ", measureKind:" <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", line " << elt->getInputLineNumber () <<
      endl;

  // handle the measure
  switch (measureKind) {
    case msrMeasure::kUnknownMeasureKind:
      break;
      
    case msrMeasure::kFullMeasureKind:
      break;
      
    case msrMeasure::kUpbeatMeasureKind:
      break;
      
    case msrMeasure::kUnderfullMeasureKind:
      fOstream << idtr <<
        "\\unset Score.measureLength" <<
        endl;
      break;

    case msrMeasure::kOverfullMeasureKind:
    /* JMI
      fOstream <<
        endl <<
        idtr <<
          "\\cadenzaOff" <<
          endl;

      fOnGoingVoiceCadenza = true;
      */
      
/* JMI
      idtr--;
      fOstream <<
        "}" <<
        endl <<
        "\\bar \"|\"" <<
        endl;
        */
      break;

    case msrMeasure::kSenzaMisuraMeasureKind:
    /* JMI   
      fOstream <<
        idtr <<
          "\\cadenzaOff" <<
          endl <<
        idtr<<
          "\\bar \"|\"" <<
          endl;
          */
      break;

    case msrMeasure::kEmptyMeasureKind:
      break;
  } // switch
    
  if (gLilypondOptions->fComments) {
    idtr--;

    if (fMusicOlec > 0) {
      fOstream <<
        endl <<
        idtr <<
          setw(commentFieldWidth) <<
           "% end of measure " <<
          measureNumber <<
          ", line " << inputLineNumber <<
          endl <<
          endl;
    }
    else {
      fOstream <<
        idtr <<
          setw(commentFieldWidth) <<
          "% end of measure " <<
          measureNumber <<
          ", line " << inputLineNumber <<
          endl <<
          endl;      
    }
  }

  if (gLilypondOptions->fSeparatorLineEveryNMeasures) {
    if (
      fMeasuresCounter
        %
      gLilypondOptions->fSeparatorLineEveryNMeasuresValue
        ==
      0)
      fOstream <<
        endl <<
        idtr <<
        "% ============================= " <<
        endl <<
        endl <<
        idtr;
  }
  
  fSegmentNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    // don't generate code for the stanza inside the code for the voice
    fOngoingNonEmptyStanza =
      ! fOnGoingVoice && elt->getStanzaTextPresent ();

    if (fOngoingNonEmptyStanza) {
      fOstream << idtr <<
        elt->getStanzaName () << " = " << "\\lyricmode" << " {" <<
        endl;
        
      idtr++;
      
      fOstream << idtr;
      
      fStanzaOlec.resetToZero ();
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    if (fOngoingNonEmptyStanza) {
      idtr--;
    
      fOstream <<
        idtr << "}" <<
        endl <<
        endl;
    }

    fOngoingNonEmptyStanza = false;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSyllable '" <<
      elt->syllableAsString () <<
      "'" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    if (gGeneralOptions->fTraceLyrics || fOngoingNonEmptyStanza) {
      
      switch (elt->getSyllableKind ()) {
        case msrSyllable::kSingleSyllable:
          fOstream <<
            quoteStringIfNonAlpha (elt->getSyllableText ()) << " ";
          break;
          
        case msrSyllable::kBeginSyllable:
          fOstream <<
            quoteStringIfNonAlpha (elt->getSyllableText ()) << " ";
          break;
          
        case msrSyllable::kMiddleSyllable:
          fOstream <<
            "-- " <<
            quoteStringIfNonAlpha (elt->getSyllableText ()) << " ";
          break;
          
        case msrSyllable::kEndSyllable:
          fOstream <<
            "-- " <<
            quoteStringIfNonAlpha (elt->getSyllableText ()) << " ";
          break;
          
        case msrSyllable::kRestSyllable:
          // LilyPond ignores the skip duration
          // when \lyricsto is used
       //   fOstream <<
         //   "\\skip1" " ";

          fStanzaOlec++; // for the comment        
          break;
          
        case msrSyllable::kSkipSyllable:
          // LilyPond ignores the skip duration
          // when \lyricsto is used
          fOstream <<
            "\\skip1" " ";
          break;
          
        case msrSyllable::kTiedSyllable:
          fOstream <<
            "%{ ~ " << "\"" << elt->getSyllableText () << "\"" << " %}" <<
            endl <<
            idtr;
          break;
          
        case msrSyllable::kSlurSyllable:
          fOstream <<
            "%{ slur " << "\"" << elt->getSyllableText () << "\"" << " %}" <<
            endl <<
            idtr;
          break;
  
        case msrSyllable::kSlurBeyondEndSyllable:
  // JMI       fOstream <<
   //         "__ " << " ";
          break;
  
        case msrSyllable::kLigatureSyllable:
          fOstream <<
            "%{ ligature " << "\"" << elt->getSyllableText () << "\"" << " %}" <<
            endl <<
            idtr;
          break;
  
        case msrSyllable::kLigatureBeyondEndSyllable:
  // JMI       fOstream <<
   //         "__ " << " ";
          break;
  
        case msrSyllable::kBarcheckSyllable:
          fOstream <<
     // JMI       "%{ | % " << elt->getSyllableText () << " %}" <<
            "| %{ " << elt->getSyllableText () << " %}" <<
            endl <<
            idtr;

          fStanzaOlec.resetToZero ();
          break;
    
        case msrSyllable::kBarNumberCheckSyllable:
          fOstream <<
     // JMI       "%{ | % " << elt->getSyllableText () << " %}" <<
            "%{ " << elt->getSyllableText () << " %}" <<
            endl <<
            idtr;

          fStanzaOlec.resetToZero ();
          break;
    
        case msrSyllable::kBreakSyllable:
          fOstream <<
            "%{ break " << "\"" << elt->getSyllableText () << "\"" << " %}" <<
            endl <<
            idtr;
          break;
    
        case msrSyllable::k_NoSyllable: // JMI
          break;
      } // switch

      fStanzaOlec++;

      switch (elt->getSyllableExtendKind ()) {
        case msrSyllable::kStandaloneSyllableExtend:
          // generate a lyric extender after this syllable
          fOstream <<
            "__" " ";
          fStanzaOlec++;            
          break;
        case msrSyllable::kStartSyllableExtend:
          break;
        case msrSyllable::kContinueSyllableExtend:
          break;
        case msrSyllable::kStopSyllableExtend:
          break;
        case msrSyllable::k_NoSyllableExtend:
          break;
      } // switch
      
      if (gLilypondOptions->fInputLineNumbers)
        // print the note line number as a comment
        fOstream <<
          "%{ " << elt->getInputLineNumber () << " %} ";  
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrSyllable '" <<
      elt->syllableAsString () <<
      "'" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrClef '" <<
      elt->clefAsString () <<
      "'" <<
      endl;

  msrClef::msrClefKind
    clefKind =
      elt->getClefKind ();

  if (clefKind != msrClef::k_NoClef) {
    fOstream << idtr <<
      "\\clef" " \"";
  
    switch (clefKind) {
      case msrClef::k_NoClef:
        break;
      case msrClef::kTrebleClef:
        fOstream << "treble";
        break;
      case msrClef::kSopranoClef:
        fOstream << "soprano";
        break;
      case msrClef::kMezzoSopranoClef:
        fOstream << "mezzosoprano";
        break;
      case msrClef::kAltoClef:
        fOstream << "alto";
        break;
      case msrClef::kTenorClef:
        fOstream << "tenor";
        break;
      case msrClef::kBaritoneClef:
        fOstream << "baritone";
        break;
      case msrClef::kBassClef:
        fOstream << "bass";
        break;
      case msrClef::kTrebleLine1Clef:
        fOstream << "french";
        break;
      case msrClef::kTrebleMinus15Clef:
        fOstream << "treble_15";
        break;
      case msrClef::kTrebleMinus8Clef:
        fOstream << "treble_8";
        break;
      case msrClef::kTreblePlus8Clef:
        fOstream << "treble^8";
        break;
      case msrClef::kTreblePlus15Clef:
        fOstream << "treble^15";
        break;
      case msrClef::kBassMinus15Clef:
        fOstream << "bass_15";
        break;
      case msrClef::kBassMinus8Clef:
        fOstream << "bass_8";
        break;
      case msrClef::kBassPlus8Clef:
        fOstream << "bass^8";
        break;
      case msrClef::kBassPlus15Clef:
        fOstream << "bass^15";
        break;
      case msrClef::kVarbaritoneClef:
        fOstream << "varbaritone";
        break;
      case msrClef::kTablature4Clef:
        if (gLilypondOptions->fModernTab)
          fOstream << "moderntab";
        else
          fOstream << "tab";
        break;
      case msrClef::kTablature5Clef:
        if (gLilypondOptions->fModernTab)
          fOstream << "moderntab";
        else
          fOstream << "tab";
        break;
      case msrClef::kTablature6Clef:
        if (gLilypondOptions->fModernTab)
          fOstream << "moderntab";
        else
          fOstream << "tab";
        break;
      case msrClef::kTablature7Clef:
        if (gLilypondOptions->fModernTab)
          fOstream << "moderntab";
        else
          fOstream << "tab";
        break;
      case msrClef::kPercussionClef:
        fOstream << "percussion";
        break;
    } // switch
  
  fOstream <<
    "\"" <<
    endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrClef '" <<
      elt->clefAsString () <<
      "'" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrKey '" <<
      elt->keyAsString () <<
      "'" <<
      endl;

  switch (elt->getKeyKind ()) {
    case msrKey::kTraditionalKind:
      fOstream <<
        idtr <<
        "\\key " <<
        msrQuartertonesPitchAsString (
          gLpsrOptions->fLpsrQuatertonesPitchesLanguage,
          elt->getKeyTonicQuartertonesPitch ()) <<
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
          fOstream <<
            endl <<
            idtr <<
            "\\set Staff.keyAlterations = #`(";

          vector<S_msrHumdrumScotKeyItem>::const_iterator
            iBegin = humdrumScotKeyItemsVector.begin(),
            iEnd   = humdrumScotKeyItemsVector.end(),
            i      = iBegin;
            
          for ( ; ; ) {
            S_msrHumdrumScotKeyItem item = (*i);
              
            if (elt->getKeyItemsOctavesAreSpecified ()) {
              //   JMI   "((octave . step) . alter) ((octave . step) . alter) ...)";
              //\set Staff.keyAlterations = #`(((3 . 3) . 7) ((3 . 5) . 3) ((3 . 6) . 3))"  \time 2/4
    
                                      
                fOstream <<
                  "(" <<
                    "(" <<
                    item->getKeyItemOctave () - 3 << // 3 is middle C in MusicXML
                    " . " <<
                    item->getKeyItemDiatonicPitch () <<
                    ")" <<
                  " . ," <<
                  alterationAsLilypondString (
                    item->getKeyItemAlteration ()) <<                
                  ")";
            }
  
            else {
              // Alternatively, for each item in the list, using the more concise format (step . alter) specifies that the same alteration should hold in all octaves.
                                      
                fOstream <<
                  "(" <<
                  item->getKeyItemDiatonicPitch () <<
                  " . ," <<
                  alterationAsLilypondString (
                    item->getKeyItemAlteration ()) <<                
                  ")";
            }
                
            if (++i == iEnd) break;
            
            fOstream << " ";
          } // for
  
          fOstream <<
            ")";
        }
        
        else {
            msrInternalError (
              elt->getInputLineNumber (),
              "Humdrum/Scot key items vector is empty");
        }
      }
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrKey '" <<
      elt->keyAsString () <<
      "'"  <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTime '" <<
      elt->timeAsString () <<
      "'"  <<
      endl;

  msrTime::msrTimeSymbolKind
    timeSymbolKind =
      elt->getTimeSymbolKind ();
      
  const vector<S_msrTimeItem>&
    timeItemsVector =
      elt->getTimeItemsVector ();

  // is this the end of a senza misura fragment?
  if (
    fVoiceIsCurrentlySenzaMisura
      &&
    timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
    fOstream <<
      idtr <<
        "\\undo\\omit Staff.TimeSignature" <<
      endl;

    fVoiceIsCurrentlySenzaMisura = false;
  }

  // handle the time
  if (timeSymbolKind == msrTime::kTimeSymbolSenzaMisura) {

    // senza misura time
    
    fOstream <<
      idtr <<
        "\\omit Staff.TimeSignature" <<
      endl;

    fVoiceIsCurrentlySenzaMisura = true;
  }

  else {

    // con misura time
    
    int timesItemsNumber =
      timeItemsVector.size ();
  
    if (timesItemsNumber) {
  
      if (! elt->getTimeIsCompound ()) {
  
        // simple time
        // \time "3/4" for 3/4
        // or senza misura
        
        fOstream <<
          idtr;
  
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
          timeSymbolKind == msrTime::k_NoTimeSymbol
            ||
          gLilypondOptions->fNumericalTime) {
          fOstream <<
            "\\numericTimeSignature ";
        }
  
        // should there be a single number?
        if (timeSymbolKind == msrTime::kTimeSymbolSingleNumber) {
          fOstream <<
            "\\once\\override Staff.TimeSignature.style = #'single-digit" <<
            endl <<
            idtr;
        }
  
        /* dotted-note: JMI
  \relative {
  \override Staff.TimeSignature.stencil = #(lambda (grob)
                                             (grob-interpret-markup grob #{
                                               \markup\override #'(baseline-skip . 0.5) {
                                                 \column { \number 2 \tiny\note #"4." #-1 }
                                                 \vcenter "="
                                                 \column { \number 6 \tiny\note #"8" #-1 }
                                                 \vcenter "="
                                                 \column\number { 6 8 }
                                               }
                                               #}))
                                               *
     * ou plus simle
     * #(define-public (format-time-sig-note grob)
  (let* ((frac (ly:grob-property grob 'fraction))
  (num (if (pair? frac) (car frac) 4))
  (den (if (pair? frac) (cdr frac) 4))
  (m (markup #:override '(baseline-skip . 0.5)
  #:center-column (#:number (number->string num)
  #'" "
  #:override '(style . default)
  #:note (number->string den) DOWN))))
  (grob-interpret-markup grob m)))
  
         */
        fOstream <<
          "\\time" " " <<
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
    
        fOstream <<
          idtr <<
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
          fOstream <<
            "(";
  
          // then generate all beats numbers in the vector
          for (int j = 0; j < beatsNumbersNumber; j++) {
            fOstream <<
              beatsNumbersVector [j] <<
              " ";
          } // for
      
          // then generate the beat type
          fOstream <<
            timeItem->getTimeBeatValue ();
  
          // and finally generate the closing parenthesis
          fOstream <<
            ")";
  
          if (i != timesItemsNumber - 1)
            fOstream <<
              " ";
        } // for
              
      fOstream <<
        ")" <<
        endl;
      }
    }
    
    else {
      if (timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
        msrInternalError (
          elt->getInputLineNumber (),
          "time items vector is empty");
      }
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTime '" <<
      elt->timeAsString () <<
      "'"  <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTempo" <<
      endl;

  string tempoIndication = elt->getTempoIndication ();
  
  int tempoUnit = elt->getTempoUnit ();
  int perMinute = elt->getPerMinute ();

  fOstream << idtr <<
    "\\tempo";

  if (tempoIndication.size ())
    fOstream <<
      " \"" << tempoIndication << "\"";

  if (tempoUnit)
    fOstream <<
      " " << tempoUnit << " = " << perMinute;

  fOstream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTempo" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrArticulation" <<
      endl;

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrArticulation" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTechnical" <<
      endl;

  // don't generate the technical here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTechnical" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTechnicalWithInteger" <<
      endl;

  // don't generate the technicalWithInteger here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTechnicalWithInteger" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTechnicalWithString" <<
      endl;

  // don't generate the technicalWithString here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTechnicalWithString" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrOrnament" <<
      endl;

  // don't generate the ornament here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrOrnament" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSingleTremolo" <<
      endl;

  // don't generate the singleTremolo here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrSingleTremolo" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrDoubleTremolo" <<
      endl;

  // fetch double tremolo divisions
  int doubleTremoloSoundingDivisions =
    elt->
      getDoubleTremoloSoundingDivisions ();

  // the marks number determines the duration of the two elements:
  // '8' for 1, '16' for 2, etc
  fCurrentDoubleTremoloElementsLpsrDuration =
    int (
      pow (
        2,
        elt->getDoubleTremoloMarksNumber () + 2));

  // fetch the current part's number of divisions per quarter element
  int partDivisionsPerQuarterNote =
    elt->
      getDoubleTremoloVoiceUplink ()->
        getVoiceDirectPartUplink ()->
          getPartDivisionsPerQuarterNote ();

  // fetch the number of divisions per double tremolo element
  int divisionsPerDoubleTremoloElement =
    partDivisionsPerQuarterNote
      *
    4 // quarter note
      /
    fCurrentDoubleTremoloElementsLpsrDuration;

  if (divisionsPerDoubleTremoloElement <= 0) {
    stringstream s;

    s <<
      "divisionsPerDoubleTremoloElement = " <<
      divisionsPerDoubleTremoloElement <<
      " while it should be positive" <<
      endl <<
      tab << "partDivisionsPerQuarterNote = " << 
      partDivisionsPerQuarterNote <<
      endl <<
      tab << "doubleTremoloSoundingDivisions = " <<
      doubleTremoloSoundingDivisions <<
      endl <<
      tab << "fCurrentDoubleTremoloElementsLpsrDuration = " <<
      fCurrentDoubleTremoloElementsLpsrDuration;
    
    msrInternalError (
      elt->getInputLineNumber (),
      s.str());
  }
    
  // the number of repeats is the quotient of the number of sounding divisions
  // by the duration of the elements
  int numberOfRepeats =
    doubleTremoloSoundingDivisions
      /
    (2 * divisionsPerDoubleTremoloElement); // to account for both elements

//  if (false && gGeneralOptions->fTraceTremolos) { // JMI
  if (gGeneralOptions->fTraceTremolos) { // JMI
    cerr <<
      "% visitStart (S_msrDoubleTremolo&)" <<
      endl <<
      tab << "% partDivisionsPerQuarterNote = " <<
      partDivisionsPerQuarterNote <<
      endl <<
      tab << "% doubleTremoloSoundingDivisions = " <<
      doubleTremoloSoundingDivisions <<
      endl <<
      tab << "% fCurrentDoubleTremoloElementsLpsrDuration = " <<
      fCurrentDoubleTremoloElementsLpsrDuration <<
      endl <<
      tab << "% divisionsPerDoubleTremoloElement = " <<
      divisionsPerDoubleTremoloElement <<
      endl <<
      tab << "% numberOfRepeats = " << numberOfRepeats <<
      endl;
  }
  
  fOstream <<
    idtr <<
    "\\repeat tremolo " << numberOfRepeats <<
    " {" <<
    endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrDoubleTremolo" <<
      endl;

  idtr--;
  
  fOstream <<
    endl <<
    idtr <<
    "}" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrDynamics" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrDynamics" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrOtherDynamics" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrOtherDynamics" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrWords" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrWords" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSlur" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrSlur" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrLigature" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrLigature" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrWedge" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrWedge" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrGraceNotes" <<
      endl;

  if (elt->getGraceNotesIsSlashed ())
    fOstream <<
      "\\slashedGrace"; // JMI "\\grace";
  else
    fOstream <<
      "\\grace"; // JMI "\\acciaccatura" \\appoggiatura
  fOstream << " { ";
}

void lpsr2LilypondTranslator::visitEnd (S_msrGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrGraceNotes" <<
      endl;

  fOstream <<
    "} ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrAfterGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrAfterGraceNotes" <<
      endl;

 // JMI exists? if (elt->getGraceNotesIsSlashed ())
  fOstream <<
    "\\afterGrace" " ";

/* JMI
  printNoteAsLilypondString (
    elt->getAfterGraceNotesNote ());

  fOstream <<
    " { ";
    */
}

void lpsr2LilypondTranslator::visitEnd (S_msrAfterGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrAfterGraceNotes" <<
      endl;

  fOstream <<
    "} ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fOstream << idtr <<
      "% --> Start visiting ";
      
    switch (elt->getNoteKind ()) {

      case msrNote::k_NoNoteKind:
        break;
        
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
        break;
        
      case msrNote::kDoubleTremoloMemberNote:
        fOstream << "double tremolo member note";
        break;
        
      case msrNote::kGraceNote:
        fOstream << "grace";
        break;
        
      case msrNote::kRestNote:        
        fOstream << "rest";
        break;
        
      case msrNote::kSkipNote:        
        fOstream << "skip";
        break;
        
      case msrNote::kChordMemberNote:
        fOstream << "chord member";
        break;
        
      case msrNote::kTupletMemberNote:
        fOstream << "tuplet member";
        break;
    } // switch
    
    fOstream << " msrNote" <<
      endl;
    fMusicOlec++;
  }

  // indent before the fist note of the msrSegment if needed
  if (++ fSegmentNotesAndChordsCountersStack.top () == 1)
    fOstream <<
      idtr;

  // print the note as a LilyPond string
  printNoteAsLilypondString (elt);

  fOnGoingNote = true;
}

void lpsr2LilypondTranslator::printNoteAsLilypondString (S_msrNote note)
{  
  // print the note ligatures if any
  list<S_msrLigature>
    noteLigatures =
      note->getNoteLigatures ();

  if (noteLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin();
      i!=noteLigatures.end();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kStartLigature:
          fOstream << "\\[" " ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
   // JMI       fOstream << "\\]" " ";
          break;
      } // switch
      fMusicOlec++;
    } // for
  }

  // get note stem kind 
  msrStem::msrStemKind
    stemKind =
      fCurrentStem
        ? fCurrentStem->getStemKind ()
        : msrStem::k_NoStem;
      
  switch (note->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kStandaloneNote:
      {
        // should the stem be omitted?
        if (note->getNoteIsStemless ()) {
          fOstream <<
            endl <<
            idtr <<
            "\\stemNeutral" " "; // JMI ""\\once\\omit Stem" " ";
        }

        // should stem direction be generated?
        if (gLilypondOptions->fStems) {
      
          if (fCurrentStem) {
            // should stem direction be generated?
            if (stemKind != fCurrentStemKind) {
              switch (stemKind) {
                case msrStem::k_NoStem:
                  fOstream << "\\stemNeutral" " ";
                  break;
                case msrStem::kStemUp:
                  fOstream << "\\stemUp" " ";
                  break;
                case msrStem::kStemDown:
                  fOstream << "\\stemDown" " ";
                  break;
                case msrStem::kStemNone:
                  break;
                case msrStem::kStemDouble: // JMI ???
                  break;
              } // switch
            }
            fMusicOlec++;
      
            // is there a stem kind change?
            if (stemKind != fCurrentStemKind)
              fCurrentStemKind = stemKind;
          }
        }
      }
      break;

    case msrNote::kDoubleTremoloMemberNote:
      {
        // should the stem be omitted?
        if (note->getNoteIsStemless ()) {
          fOstream <<
            endl <<
            idtr <<
            "\\stemNeutral"; // JMI ""\\once\\omit Stem" " ";
        }

        // should stem direction be generated?
        if (gLilypondOptions->fStems) {
      
          if (fCurrentStem) {
            // should stem direction be generated?
            if (stemKind != fCurrentStemKind) {
              switch (stemKind) {
                case msrStem::k_NoStem:
                  fOstream << "\\stemNeutral" " ";
                  break;
                case msrStem::kStemUp:
                  fOstream << "\\stemUp" " ";
                  break;
                case msrStem::kStemDown:
                  fOstream << "\\stemDown" " ";
                  break;
                case msrStem::kStemNone:
                  break;
                case msrStem::kStemDouble: // JMI ???
                  break;
              } // switch
            }
            fMusicOlec++;
      
            // is there a stem kind change?
            if (stemKind != fCurrentStemKind)
              fCurrentStemKind = stemKind;
          }
        }
      }
      break;

    case msrNote::kGraceNote:
      break;
      
    case msrNote::kRestNote:      
      break;
      
    case msrNote::kSkipNote:      
      break;
      
    case msrNote::kChordMemberNote:
     // don't omit stems for chord member note JMI
     break;
      
    case msrNote::kTupletMemberNote:
      break;
  } // switch

  // print the note
  switch (note->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kStandaloneNote:
      // print the note name
      fOstream <<
        noteAsLilypondString (note);
      
      // print the note duration
      fOstream <<
        lilypondizeDurationString (
          note->noteSoundingDivisionsAsMsrString ());

      // handle delayed ornaments if any
      if (note->getNoteHasADelayedOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        fOstream <<
          "*" <<
          gLilypondOptions->fDelayedOrnamentFractionNumerator <<
          "/" <<
          gLilypondOptions->fDelayedOrnamentFractionDenominator;
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fOstream << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kDoubleTremoloMemberNote:
      // print the note name
      fOstream <<
        noteAsLilypondString (note);
      
      // print the note duration, i.e. the double tremolo elements duration
      fOstream <<
        fCurrentDoubleTremoloElementsLpsrDuration; // JMI

      // handle delayed ornaments if any
      if (note->getNoteHasADelayedOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        fOstream <<
          "*" <<
          gLilypondOptions->fDelayedOrnamentFractionNumerator <<
          "/" <<
          gLilypondOptions->fDelayedOrnamentFractionDenominator;
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fOstream << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kGraceNote:
      // print the note name
      fOstream <<
        noteAsLilypondString (note);
      
      // print the grace note's graphic duration
      fOstream <<
        lilypondizeDurationString (
          note->noteGraphicDurationAsMsrString ());

      // print the dots if any JMI ???
      for (int i = 0; i < note->getNoteDotsNumber (); i++) {
        fOstream << ".";
      } // for
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fOstream << "~ ";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kRestNote:      
      // print the rest name
      fOstream <<
        string (
          note->getNoteOccupiesAFullMeasure ()
            ? "R"
            : "r");
      
      // print the rest duration
      fOstream <<
        lilypondizeDurationString (
          note->skipOrRestDivisionsAsMsrString ());

      // a rest is no relative octave reference,
      // the preceding one is kept
      break;
      
    case msrNote::kSkipNote:      
      // print the skip name
      fOstream << "s";
      
      // print the skip duration
      fOstream <<
        lilypondizeDurationString (
          note->skipOrRestDivisionsAsMsrString ());

      // a rest is no relative octave reference,
      // the preceding one is kept
      break;
      
    case msrNote::kChordMemberNote:
      // print the note name
      fOstream <<
        noteAsLilypondString (note);
      
      // don't print the note duration,
      // it will be printed for the chord itself

      // inside chords, a note is relative to the preceding one
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kTupletMemberNote:
      if (! gLilypondOptions->fTupletsOnALine) {
        fOstream <<
          endl <<
          idtr;
      }
        
      // print the note name
      if (note->getNoteIsARest ()) {
        fOstream <<
          string (
            note->getNoteOccupiesAFullMeasure ()
              ? "R"
              : "r");
      }
      else
        fOstream <<
          noteAsLilypondString (note);
      
      // print the note duration
      S_msrTuplet
        tuplet =
          note->getNoteTupletUplink ();
          
      fOstream << // JMI note display divisions are already known..., don't compute here
        tupletDivisionsAsLilypondString (
          note->getInputLineNumber (),
          note->getNoteDirectPartUplink (),
          note->getNoteSoundingDivisions (), // JMI
          tuplet->getTupletActualNotes (),
          tuplet->getTupletNormalNotes ());

      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fOstream << "~ ";
          }
        }
      }

      // a rest is no relative octave reference,
      if (! note->getNoteIsARest ())
        // this note is the new relative octave reference
        fRelativeOctaveReference = note;
      break;
  } // switch

  fOstream << " ";

  fMusicOlec++;

  // was there an unmetered (stemless) section?
  if (stemKind != fCurrentStemKind) {
    switch (stemKind) {
      case msrStem::kStemNone:
        break;
        
      case msrStem::k_NoStem:
      case msrStem::kStemUp:
      case msrStem::kStemDown:
      case msrStem::kStemDouble:
        fOnGoingStemNone =false;
        break;
    } // switch
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fOstream << idtr <<
      "% --> Start visiting " <<
      msrNote::noteKindAsString (elt->getNoteKind ()) <<
      " msrNote" <<
      endl;
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
      int
        inputLineNumber =
          noteSingleTremolo->getInputLineNumber ();
        
      int
        singleTremoloMarksNumber =
          noteSingleTremolo->
            getSingleTremoloMarksNumber ();
  
      S_msrNote
        singleTremoloNote =
          noteSingleTremolo->
            getSingleTremoloNoteUplink ();
    
      msrDuration
        singleTremoloNoteDuration =
          singleTremoloNote->getNoteGraphicDuration ();
  
      fOstream <<
        singleTremoloDurationAsLilypondString (
          inputLineNumber,
          singleTremoloNoteDuration,
          singleTremoloMarksNumber);
    }
  }

  // get note stem kind 
  msrStem::msrStemKind
    stemKind = // JMI
      fCurrentStem
        ? fCurrentStem->getStemKind ()
        : msrStem::k_NoStem;

  // has the stem been omitted?
  if (elt->getNoteIsStemless ()) {
    fOstream <<
      endl <<
      idtr;
  }
  
  if (gLilypondOptions->fInputLineNumbers)
    // print the note line number as a comment
    fOstream <<
      "%{ " << elt->getInputLineNumber () << " %} ";
  
  // print the note articulations if any
  list<S_msrArticulation>
    noteArticulations =
      elt->getNoteArticulations ();
      
  if (noteArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=noteArticulations.begin();
      i!=noteArticulations.end();
      i++) {
        
      switch ((*i)->getArticulationKind ()) {
        case msrArticulation::kAccent:
          fOstream << "->";
          break;
        case msrArticulation::kBreathMark:
          fOstream << "\\breathe";
          break;
        case msrArticulation::kCaesura:
          fOstream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl <<
            idtr;
          break;
        case msrArticulation::kSpiccato:
          fOstream << "%{\\spiccato???%}";
          break;
        case msrArticulation::kStaccato:
          fOstream << "\\staccato"; // JMI "-.";
          break;
        case msrArticulation::kStaccatissimo:
          fOstream << "-!";
          break;
        case msrArticulation::kStress:
          fOstream << "%{\\stress???%}";
          break;
        case msrArticulation::kUnstress:
          fOstream << "%{\\unstress???%}";
          break;
        case msrArticulation::kDetachedLegato:
          fOstream << "-_";
          break;
        case msrArticulation::kStrongAccent:
          fOstream << "\\accent %{strong???%}";
          break;
        case msrArticulation::kTenuto:
          fOstream << "--";
          break;
        case msrArticulation::kFermata:
          fOstream << "\\fermata";
          break;
        case msrArticulation::kArpeggiato:
          fOstream << "\\arpeggio";
          /* JMI
           in one of the voices add:
            \once \set StaffGroup.connectArpeggios = ##t
          */
          break;
        case msrArticulation::kDoit:
          fOstream << "\\bendAfter #+4";
          break;
        case msrArticulation::kFalloff:
          fOstream << "\\bendAfter #-4";
          break;
        case msrArticulation::kPlop:
          fOstream << "%{\\plop???%}";
          break;
        case msrArticulation::kScoop:
          fOstream << "%{\\scoop???%}";
          break;
      } // switch
      
      fOstream << " ";
      fMusicOlec++;
    } // for
  }

  // print the note technicals if any
  list<S_msrTechnical>
    noteTechnicals =
      elt->getNoteTechnicals ();
      
  if (noteTechnicals.size()) {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=noteTechnicals.begin();
      i!=noteTechnicals.end();
      i++) {
        
      fOstream <<
        technicalKindAsLilypondString ((*i));
      fMusicOlec++;

      switch ((*i)->getTechnicalPlacementKind ()) {
        case msrTechnical::k_NoTechnicalPlacement:
          break;
        case msrTechnical::kTechnicalPlacementAbove:
 // JMI         fOstream << "^";
          break;
        case msrTechnical::kTechnicalPlacementBelow:
 // JMI         fOstream << "_";
          break;
      } // switch

      fMusicOlec++;

      fOstream << " ";
      fMusicOlec++;
    } // for
  }

  // print the note technicals if any
  list<S_msrTechnicalWithInteger>
    noteTechnicalWithIntegers =
      elt->getNoteTechnicalWithIntegers ();
      
  if (noteTechnicalWithIntegers.size()) {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=noteTechnicalWithIntegers.begin();
      i!=noteTechnicalWithIntegers.end();
      i++) {
        
      fOstream <<
        technicalWithIntegerKindAsLilypondString ((*i));
      fMusicOlec++;

      switch ((*i)->getTechnicalWithIntegerPlacementKind ()) {
        case msrTechnicalWithInteger::k_NoTechnicalWithIntegerPlacement:
          break;
        case msrTechnicalWithInteger::kTechnicalWithIntegerPlacementAbove:
 // JMI         fOstream << "^";
          break;
        case msrTechnicalWithInteger::kTechnicalWithIntegerPlacementBelow:
 // JMI         fOstream << "_";
          break;
      } // switch

      fMusicOlec++;

      fOstream << " ";
      fMusicOlec++;
    } // for
  }

  // print the note technicals if any
  list<S_msrTechnicalWithString>
    noteTechnicalWithStrings =
      elt->getNoteTechnicalWithStrings ();
      
  if (noteTechnicalWithStrings.size()) {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=noteTechnicalWithStrings.begin();
      i!=noteTechnicalWithStrings.end();
      i++) {
        
      fOstream <<
        technicalWithStringKindAsLilypondString ((*i));
      fMusicOlec++;

      switch ((*i)->getTechnicalWithStringPlacementKind ()) {
        case msrTechnicalWithString::k_NoTechnicalWithStringPlacement:
          break;
        case msrTechnicalWithString::kTechnicalWithStringPlacementAbove:
 // JMI         fOstream << "^";
          break;
        case msrTechnicalWithString::kTechnicalWithStringPlacementBelow:
 // JMI         fOstream << "_";
          break;
      } // switch

      fMusicOlec++;

      fOstream << " ";
      fMusicOlec++;
    } // for
  }

  // print the note ornaments if any
  list<S_msrOrnament>
    noteOrnaments =
      elt->getNoteOrnaments ();
      
  if (noteOrnaments.size()) {
    list<S_msrOrnament>::const_iterator i;
    for (
      i=noteOrnaments.begin();
      i!=noteOrnaments.end();
      i++) {
      string
        noteDuration =
          (*i)->getOrnamentNoteUplink ()->
            noteSoundingDivisionsAsMsrString ();
        
      fOstream <<
        ornamentKindAsLilypondString (
          (*i)->getInputLineNumber (), // some ornaments are not yet supported
          (*i)->getOrnamentKind (),
          noteDuration);
          // <<
        // JMI " ";
      fMusicOlec++;

      switch ((*i)->getOrnamentPlacementKind ()) {
        case msrOrnament::k_NoOrnamentPlacement:
          break;
        case msrOrnament::kOrnamentPlacementAbove:
 // JMI         fOstream << "^";
          break;
        case msrOrnament::kOrnamentPlacementBelow:
 // JMI         fOstream << "_";
          break;
      } // switch

      fMusicOlec++;

      switch ((*i)->getOrnamentAccidentalMarkKind ()) {
        case msrOrnament::kDoubleFlat:
          fOstream << " ^" "\\markup { \\doubleflat }";
          break;
        case msrOrnament::kSesquiFlat:
          fOstream << " ^" "\\markup { \\sesquiflat }";
          break;
        case msrOrnament::kFlat:
          fOstream << " ^" "\\markup { \\flat }";
          break;
        case msrOrnament::kSemiFlat:
          fOstream << " ^" "\\markup { \\semiflat }";
          break;
        case msrOrnament::kNatural:
          fOstream << " ^" "\\markup { \\natural }";
          break;
        case msrOrnament::kSemiSharp:
          fOstream << " ^" "\\markup { \\semisharp }";
          break;
        case msrOrnament::kSharp:
          fOstream << " ^" "\\markup { \\sharp }";
          break;
        case msrOrnament::kSesquiSharp:
          fOstream << " ^" "\\markup { \\sesquisharp }";
          break;
        case msrOrnament::kDoubleSharp:
          fOstream << " ^" "\\markup { \\doublesharp }";
          break;
      } // switch

      fOstream << " ";
      fMusicOlec++;
    } // for
  }

  // print the note dynamics if any
  list<S_msrDynamics>
    noteDynamics =
      elt->getNoteDynamics ();
      
  if (noteDynamics.size()) {
    list<S_msrDynamics>::const_iterator i;
    for (
      i=noteDynamics.begin();
      i!=noteDynamics.end();
      i++) {
      fOstream <<
        "\\" << (*i)->dynamicsKindAsString () << " ";
      fMusicOlec++;
    } // for
  }
  
  // print the note other dynamics if any
  list<S_msrOtherDynamics>
    noteOtherDynamics =
      elt->getNoteOtherDynamics ();
      
  if (noteOtherDynamics.size()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (
      i=noteOtherDynamics.begin();
      i!=noteOtherDynamics.end();
      i++) {
      fOstream <<
        "-\\markup { "
        "\\dynamic \" " << (*i)->getOtherDynamicsString () << "\"} ";
      fMusicOlec++;
    } // for
  }

  // print the note words if any
  list<S_msrWords>
    noteWords =
      elt->getNoteWords ();
      
  if (noteWords.size()) {
    list<S_msrWords>::const_iterator i;
    for (
      i=noteWords.begin();
      i!=noteWords.end();
      i++) {
      msrWords::msrWordsPlacementKind
        wordsPlacementKind =
          (*i)->getWordsPlacementKind ();
    
      string wordsContents =
        (*i)->getWordsContents ();

      msrWords::msrWordsFontStyleKind
        wordsFontStyleKind =
          (*i)->getWordsFontStyleKind ();
        
      msrWords::msrWordsFontWeightKind
        wordsFontWeightKind =
          (*i)->getWordsFontWeightKind ();

      string markup;
      
      {
        // create markup apart to have its length available
        stringstream s;
        
        switch (wordsPlacementKind) {
          case msrWords::kWordsPlacementAbove:
            s << "^";
            break;
          case msrWords::kWordsPlacementBelow:
            s << "_";
            break;
        } // switch
      
        s <<
          "\\markup" << " { ";

        switch (wordsFontStyleKind) {
          case msrWords::kNormalStyle:
            // LilyPond produces 'normal style' text by default
            break;
          case msrWords::KItalicStyle:
            s <<
              "\\" "italic" " ";
            break;
        } // switch

        switch (wordsFontWeightKind) {
          case msrWords::kNormalWeight:
            // LilyPond produces 'normal weight' text by default
            break;
          case msrWords::kBoldWeight:
            s <<
              "\\" "bold" " ";
            break;
        } // switch

        s <<
          quoteStringIfNonAlpha (wordsContents) <<
          " } ";

        markup = s.str();
        }

      fOstream <<
        markup;

      // don't allow too long a line
      for (string::size_type i = 1; i <= markup.size () / 4; i++)
        fMusicOlec++;
    } // for
  }

  // print the note slurs if any
  list<S_msrSlur>
    noteSlurs =
      elt->getNoteSlurs ();
      
  if (noteSlurs.size()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=noteSlurs.begin();
      i!=noteSlurs.end();
      i++) {
        
      switch ((*i)->getSlurKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kStartSlur:
          fOstream << "(" " ";
          break;
        case msrSlur::kContinueSlur:
          break;
        case msrSlur::kStopSlur:
          fOstream << ")" " ";
          break;
      } // switch
      fMusicOlec++;
    } // for
  }

  // print the note ligatures if any
  list<S_msrLigature>
    noteLigatures =
      elt->getNoteLigatures ();
      
  if (noteLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=noteLigatures.begin();
      i!=noteLigatures.end();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kStartLigature:
   // JMI       fOstream << "\\[" " ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
          fOstream << "\\]" " ";
          break;
      } // switch
      fMusicOlec++;
    } // for
  }

  // print the note wedges if any
  list<S_msrWedge>
    noteWedges =
      elt->getNoteWedges ();
      
  if (noteWedges.size()) {
    list<S_msrWedge>::const_iterator i;
    for (
      i=noteWedges.begin();
      i!=noteWedges.end();
      i++) {
        
      switch ((*i)->getWedgeKind ()) {
        case msrWedge::kCrescendoWedge:
          fOstream << "\\<" " ";
          break;
        case msrWedge::kDecrescendoWedge:
          fOstream << "\\>" " ";
          break;
        case msrWedge::kStopWedge:
          fOstream << "\\!" " ";
          break;
      } // switch
      fMusicOlec++;
    } // for
  }

  if (elt->getNoteIsFollowedByGraceNotes ())
    fOstream << " { ";

  fOnGoingNote = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrOctaveShift" <<
      endl;

  int octaveShiftSize =
    elt->getOctaveShiftSize (); // 8 or 15
    
  fOstream <<
    "\\ottava #";
    
  switch (elt->getOctaveShiftKind ()) {
    case msrOctaveShift::kOctaveShiftUp:
      fOstream << "-" << (octaveShiftSize - 1) / 7; // 1 or 2
      break;
    case msrOctaveShift::kOctaveShiftDown:
      fOstream << (octaveShiftSize - 1) / 7; // 1 or 2
      break;
    case msrOctaveShift::kOctaveShiftStop:
      fOstream << 0;
      break;
  } // switch
  
  fOstream << " ";
  fMusicOlec++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrOctaveShift" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrStem" <<
      endl;

  fCurrentStem = elt;
}

void lpsr2LilypondTranslator::visitEnd (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrStem" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBeam& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrBeam" <<
      endl;

  // LilyPond will take care of multiple beams automatically,
  // so we need only generate code for the first number (level)
  switch (elt->getBeamKind ()) {
    
    case msrBeam::kBeginBeam:
      if (elt->getBeamNumber () == 1)
        fOstream << "[ ";
      break;
      
    case msrBeam::kContinueBeam:
      break;
      
    case msrBeam::kEndBeam:
      if (elt->getBeamNumber () == 1)
        fOstream << "] ";
      break;
      
    case msrBeam::kForwardHookBeam:
      break;
      
    case msrBeam::kBackwardHookBeam:
      break;
      
    case msrBeam::k_NoBeam:
      break;
  } // switch      
}

void lpsr2LilypondTranslator::visitEnd (S_msrBeam& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrBeam" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrChord" <<
      endl;

/* JMI ???
  // indent before the fist chord of the msrSegment if needed
  if (++ fSegmentNotesAndChordsCountersStack.top () == 1)
    fOstream << idtr;
    */

  // print the chord ligatures if any
  list<S_msrLigature>
    chordLigatures =
      elt->getChordLigatures ();
      
  if (chordLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin();
      i!=chordLigatures.end();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kStartLigature:
          fOstream << "\\[" " ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
  // JMI        fOstream << "\\]" " ";
          break;
      } // switch
      fMusicOlec++;
   } // for
  }

  // don't take the chord into account for line breaking ??? JMI
  
  fOstream << "<";
  fMusicOlec++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrChord" <<
      endl;

  fOstream <<
    ">";

  if (
    elt->getChordIsFirstChordInADoubleTremolo ()
      ||
    elt->getChordIsSecondChordInADoubleTremolo ()) {
      // print the note duration, i.e. the double tremolo elements duration
      fOstream <<
        fCurrentDoubleTremoloElementsLpsrDuration; // JMI
  }
  
  else {
    // print the chord duration
    fOstream <<
      divisionsAsLilypondString (
        elt->getInputLineNumber (),
        elt->getChordDirectPartUplink (),
        elt->getChordDisplayedDivisions ());
  }
   
  // fetch the chord single tremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      elt->getChordSingleTremolo ();

  if (chordSingleTremolo) {
    // generate code for the single tremolo
    int
      inputLineNumber =
        chordSingleTremolo->getInputLineNumber ();
      
    int
      singleTremoloMarksNumber =
        chordSingleTremolo->
          getSingleTremoloMarksNumber ();

    S_msrNote
      singleTremoloNote =
        chordSingleTremolo->
          getSingleTremoloNoteUplink ();

    msrDuration
      singleTremoloNoteDuration =
        singleTremoloNote->getNoteGraphicDuration ();

    fOstream <<
      singleTremoloDurationAsLilypondString (
        inputLineNumber,
        singleTremoloNoteDuration,
        singleTremoloMarksNumber);
  }

  fOstream <<
    " ";
    
  fMusicOlec++;

  // print the chord articulations if any
  list<S_msrArticulation>
    chordArticulations =
      elt->getChordArticulations ();
      
  if (chordArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin();
      i!=chordArticulations.end();
      i++) {
        
      switch ((*i)->getArticulationKind ()) {

        case msrArticulation::kAccent:
          fOstream << "->";
          break;
        case msrArticulation::kBreathMark:
          fOstream << "\\breathe";
          break;
        case msrArticulation::kCaesura:
          fOstream <<
            endl <<
            idtr <<
              "\\override BreathingSign.text = \\markup {"
              "\\musicglyph #\"scripts.caesura.curved\"}" <<
            endl <<
          idtr <<
            "\\breathe" <<
            endl;
          break;
        case msrArticulation::kSpiccato:
          fOstream << "spiccato";
          break;
        case msrArticulation::kStaccato:
          fOstream << "\\staccato"; // JMI "-.";
          break;
        case msrArticulation::kStaccatissimo:
          fOstream << "-!";
          break;
        case msrArticulation::kStress:
          fOstream << "stress";
          break;
        case msrArticulation::kUnstress:
          fOstream << "unstress";
          break;
        case msrArticulation::kDetachedLegato:
          fOstream << "-_"; // portato
          break;
        case msrArticulation::kStrongAccent:
          fOstream << "-^"; // marcato
          break;
        case msrArticulation::kTenuto:
          fOstream << "--";
          break;
        case msrArticulation::kFermata:
          fOstream << "\\fermata";
          break;
        case msrArticulation::kArpeggiato:
          fOstream << "\\arpeggio";
          break;
        case msrArticulation::kDoit:
          fOstream << "\\bendAfter #+4";
          break;
        case msrArticulation::kFalloff:
          fOstream << "\\bendAfter #-4";
          break;
        case msrArticulation::kPlop:
          fOstream << "plop";
          break;
        case msrArticulation::kScoop:
          fOstream << "scoop";
          break;
      } // switch
      
      fOstream << " ";
      fMusicOlec++;
    } // for
  }

  // print the chord technicals if any
  list<S_msrTechnical>
    chordTechnicals =
      elt->getChordTechnicals ();
      
  if (chordTechnicals.size()) {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=chordTechnicals.begin();
      i!=chordTechnicals.end();
      i++) {
      fOstream <<
        technicalKindAsLilypondString ((*i)) <<
        " "; // JMI
      fMusicOlec++;
    } // for
  }

  // print the chord technicals with integer if any
  list<S_msrTechnicalWithInteger>
    chordTechnicalWithIntegers =
      elt->getChordTechnicalWithIntegers ();
      
  if (chordTechnicalWithIntegers.size()) {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=chordTechnicalWithIntegers.begin();
      i!=chordTechnicalWithIntegers.end();
      i++) {
      fOstream <<
        technicalWithIntegerKindAsLilypondString ((*i)) <<
        " "; // JMI
      fMusicOlec++;
    } // for
  }

  // print the chord technicals with string if any
  list<S_msrTechnicalWithString>
    chordTechnicalWithStrings =
      elt->getChordTechnicalWithStrings ();
      
  if (chordTechnicalWithStrings.size()) {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=chordTechnicalWithStrings.begin();
      i!=chordTechnicalWithStrings.end();
      i++) {
      fOstream <<
        technicalWithStringKindAsLilypondString ((*i)) <<
        " "; // JMI
      fMusicOlec++;
    } // for
  }

  // print the chord ornaments if any
  list<S_msrOrnament>
    chordOrnaments =
      elt->getChordOrnaments ();
      
  if (chordOrnaments.size()) {
    list<S_msrOrnament>::const_iterator i;
    for (
      i=chordOrnaments.begin();
      i!=chordOrnaments.end();
      i++) {
      fOstream <<
        ornamentKindAsLilypondString (
          (*i)->getInputLineNumber (), // some ornaments are not yet supported
          (*i)->getOrnamentKind (),
          "") << // JMI
        " ";
      fMusicOlec++;
    } // for
  }

  // print the chord dynamics if any
  list<S_msrDynamics>
    chordDynamics =
      elt->getChordDynamics ();
      
  if (chordDynamics.size()) {
    list<S_msrDynamics>::const_iterator i;
    for (
      i=chordDynamics.begin();
      i!=chordDynamics.end();
      i++) {
      fOstream <<
        "\\" << (*i)->dynamicsKindAsString () << " ";
      fMusicOlec++;
    } // for
  }
  
  // print the chord other dynamics if any
  list<S_msrOtherDynamics>
    chordOtherDynamics =
      elt->getChordOtherDynamics ();
      
  if (chordOtherDynamics.size()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (
      i=chordOtherDynamics.begin();
      i!=chordOtherDynamics.end();
      i++) {
      fOstream <<
        "\\" << (*i)->otherDynamicsAsString () << " ";
      fMusicOlec++;
    } // for
  }

  // print the chord words if any
  list<S_msrWords>
    chordWords =
      elt->getChordWords ();
      
  if (chordWords.size()) {
    list<S_msrWords>::const_iterator i;
    for (
      i=chordWords.begin();
      i!=chordWords.end();
      i++) {
        
      msrWords::msrWordsPlacementKind
        wordsPlacementKind =
          (*i)->getWordsPlacementKind ();
    
      string wordsContents =
        (*i)->getWordsContents ();
    
      switch (wordsPlacementKind) {
        case msrWords::kWordsPlacementAbove:
          fOstream << "^";
          break;
        case msrWords::kWordsPlacementBelow:
          fOstream << "_";
          break;
      } // switch
    
      fOstream <<
        "\\markup" << " { " <<
        quoteStringIfNonAlpha (wordsContents) <<
        " } ";

      fMusicOlec++;      
    } // for
  }

  // print the chord slurs if any
  list<S_msrSlur>
    chordSlurs =
      elt->getChordSlurs ();
      
  if (chordSlurs.size()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=chordSlurs.begin();
      i!=chordSlurs.end();
      i++) {
        
      switch ((*i)->getSlurKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kStartSlur:
          fOstream << "(" " ";
          break;
        case msrSlur::kContinueSlur:
          break;
        case msrSlur::kStopSlur:
          fOstream << ")" " ";
          break;
      } // switch
      fMusicOlec++;
   } // for
  }

  // print the chord ligatures if any
  list<S_msrLigature>
    chordLigatures =
      elt->getChordLigatures ();
      
  if (chordLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin();
      i!=chordLigatures.end();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kStartLigature:
  // JMI        fOstream << "\\[" " ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
          fOstream << "\\]" " ";
          break;
      } // switch
      fMusicOlec++;
   } // for
  }

  // print the chord wedges if any
  list<S_msrWedge>
    chordWedges =
      elt->getChordWedges ();
      
  if (chordWedges.size()) {
    list<S_msrWedge>::const_iterator i;
    for (
      i=chordWedges.begin();
      i!=chordWedges.end();
      i++) {
        
      switch ((*i)->getWedgeKind ()) {
        case msrWedge::kCrescendoWedge:
          fOstream << "\\<" " ";
          break;
        case msrWedge::kDecrescendoWedge:
          fOstream << "\\>" " ";
          break;
        case msrWedge::kStopWedge:
          fOstream << "\\!" " ";
          break;
      } // switch
      fMusicOlec++;
    } // for
  }


/*
  // print the tie if any
  {
    S_msrTie chordTie = elt->getChordTie ();
  
    if (chordTie) {
      if (chordTie->getTieKind () == msrTie::kStartTie) {
        fOstream << "~ ";
      }
    }
  }
*/

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  fRelativeOctaveReference =
    elt->getChordNotes () [0];
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTuplet" <<
      endl;

  if (fTupletsStack.size ()) {
    // elt is a nested tuplet

    S_msrTuplet
      containingTuplet =
        fTupletsStack.top ();

    // unapply containing tuplet factor,
    // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
    elt->
      unapplyDisplayFactorToTupletMembers (
        containingTuplet->
          getTupletActualNotes (),
        containingTuplet->
          getTupletNormalNotes ());
  }

  if (gLilypondOptions->fTupletsOnALine) {
    fOstream <<
      "\\tuplet " <<
      elt->getTupletActualNotes () <<
      "/" <<
      elt->getTupletNormalNotes() << " { ";
  }
  
  else {
    fOstream <<
      endl <<
      idtr <<
      "\\tuplet " <<
      elt->getTupletActualNotes () <<
      "/" <<
      elt->getTupletNormalNotes() << " { ";
  }

  fTupletsStack.push (elt);

  fMusicOlec.resetToZero ();

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTuplet" <<
      endl;

  idtr--;

  if (gLilypondOptions->fTupletsOnALine) {
    fOstream <<
      "} ";
  }

  else {
    fOstream <<
      endl <<
      idtr <<
      "}" <<
      endl <<
      idtr;
  }

  fTupletsStack.pop ();

  fMusicOlec.resetToZero ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTie" <<
      endl;
  
  switch (elt->getTieKind ()) {
    case msrTie::k_NoTie:
      break;
    case msrTie::kStartTie:
      fOstream << "~" " ";
      break;
    case msrTie::kContinueTie:
      break;
    case msrTie::kStopTie:
      break;
   } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTie" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSegno& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSegno" <<
      endl;

  fOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.segno\" }" <<
    endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrCoda& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrCoda" <<
      endl;

  fOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.coda\" }" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrEyeGlasses& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting eyeGlasses" <<
      endl;

  fOstream <<
    "\\eyeglasses ";
  fMusicOlec++;
}

void lpsr2LilypondTranslator::visitStart (S_msrPedal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting pedal" <<
      endl;
      
  switch (elt->getPedalType ()) {
    case msrPedal::kPedalStart:
      fOstream << "<> \\sustainOn ";
      break;
    case msrPedal::kPedalContinue:
      fOstream << "\\continue pedal"; // JMI
      break;
    case msrPedal::kPedalChange:
      fOstream << "\\change pedal"; // JMI
      break;
    case msrPedal::kPedalStop:
      fOstream << "<> \\sustainOff ";
      break;
  } // switch

  fMusicOlec++;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream <<
      endl <<
      idtr << "% --> Start visiting msrBarline" <<
      endl;

  switch (elt->getBarlineCategory ()) {
    
    case msrBarline::kStandaloneBarline:
      fOstream <<
        endl <<
        idtr;
      
      switch (elt->getStyle ()) {
        case msrBarline::k_NoStyle:
          break;
        case msrBarline::kRegularStyle:
          fOstream << "\\bar \"|\"";
          break;
        case msrBarline::kDottedStyle:
          fOstream << "\\bar \";\"";
          break;
        case msrBarline::kDashedStyle:
          fOstream << "\\bar \"!\"";
          break;
        case msrBarline::kHeavyStyle:
          fOstream << "\\bar \".\"";
          break;
        case msrBarline::kLightLightStyle:
          fOstream << "\\bar \"||\"";
          break;
        case msrBarline::kLightHeavyStyle:
          fOstream << "\\bar \"|.\"";
          break;
        case msrBarline::kHeavyLightStyle:
          fOstream << "\\bar \".|\"";
          break;
        case msrBarline::kHeavyHeavyStyle:
          fOstream << "\\bar \"..\"";
          break;
        case msrBarline::kTickStyle:
          fOstream << "\\bar \"'\"";
          break;
        case msrBarline::kShortStyle:
          fOstream << "\\bar \"'\""; // JMI
          /* JMI ??? from gregorian.ly
          divisioMaior = {
            \once \override BreathingSign.stencil = #ly:breathing-sign::divisio-maior
            \once \override BreathingSign.Y-offset = #0
            \breathe
          }
          */
          break;
        case msrBarline::kNoneStyle:
          fOstream << "\\bar \"\"";
          break;
      } // switch

      fOstream <<
        endl <<
        endl;

      if (elt->getBarlineHasSegno ())
        fOstream <<
          "\\mark \\markup { \\musicglyph #\"scripts.segno\" } ";
      if (elt->getBarlineHasCoda ())
        fOstream <<
          "\\mark \\markup { \\musicglyph #\"scripts.coda\" } ";

      if (gLilypondOptions->fInputLineNumbers)
        // print the barline line number as a comment
        fOstream <<
          "%{ " << elt->getInputLineNumber () << " %} ";
      break;

    case msrBarline::kRepeatStartBarline:
    case msrBarline::kRepeatEndBarline:
    case msrBarline::kHookedEndingStartBarline:
    case msrBarline::kHookedEndingEndBarline:
    case msrBarline::kHooklessEndingStartBarline:
    case msrBarline::kHooklessEndingEndBarline:
      // should not occur, since
      // LilyPond will take care of displaying the repeat
      break;

  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream <<
      endl <<
      idtr << "% --> End visiting msrBarline" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrBarCheck" <<
      endl;
      
  int nextBarNumber =
    elt->getNextBarNumber ();

  // don't generate a bar check before the end of measure 1
 // JMI if (nextBarNumber > 1)

/*
  if (fMusicOlec == 0)
    fOstream <<
      idtr;
  */
      
  int saveIndent =
    idtr.getIndent ();
    
  fOstream << idtr <<
    "| % " << nextBarNumber << " % bar check" <<
    endl;

  if (saveIndent > 0) {
    idtr.resetToZero ();
    
    for (int i = 0; i < saveIndent; i++) {
      fOstream <<
        idtr;
        
      saveIndent++;
    }
  }

// JMI  fMusicOlec.resetToZero ();
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrBarCheck" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarNumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrBarNumberCheck" <<
      endl;

  fOstream << idtr <<
    "\\barNumberCheck #" << elt->getNextBarNumber () <<
    endl;

  fMusicOlec.resetToZero ();
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrBarNumberCheck" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrBreak" <<
      endl;

  fOstream << idtr <<
    "\\myBreak | % " << elt->getNextBarNumber () <<
    endl <<
    endl;

  fMusicOlec.resetToZero ();
  fStanzaOlec.resetToZero ();
}

void lpsr2LilypondTranslator::visitEnd (S_msrBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrBreak" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrRepeat" <<
      endl;

  fCurrentRepeatEndingsNumber =
    elt->getRepeatEndings ().size();
  
  if (fCurrentRepeatEndingsNumber == 0)
    fCurrentRepeatEndingsNumber = 2; // implicitly

  stringstream s;
  s <<
    "\\repeat volta " << fCurrentRepeatEndingsNumber << " {";
  
  if (gLilypondOptions->fComments) {
    fOstream << idtr <<
      setw(commentFieldWidth) <<
      s.str() << "% start of repeat" <<
      endl;
  }
  else {
    fOstream << idtr <<
      s.str() <<
      endl;
  }

  idtr++;

  fMusicOlec.resetToZero ();
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrRepeat" <<
      endl;

  /*
    CAUTION:
      the end of the repeat has to be generated
      before the endings are handled
  */
  
  if (elt->getRepeatEndings ().size() == 0) {
    // the end of the repeat has not been generated yet

    idtr--;

    if (gLilypondOptions->fComments) {      
      fOstream <<
        idtr <<
        setw(commentFieldWidth) <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fOstream <<
        endl <<
        idtr <<
        "}" <<
        endl;
    }
  }

  fCurrentRepeatEndingsNumber = 0;
  
  fMusicOlec.resetToZero ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeatEnding& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrRepeatEnding" <<
      endl;

  if (elt->getRepeatEndingInternalNumber () == 1) {
    
    idtr--;
    
    // first repeat ending is in charge of
    // outputting the end of the repeat
    if (gLilypondOptions->fComments) {      
      fOstream <<
        idtr <<
        setw(commentFieldWidth) <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fOstream <<
        endl <<
        idtr <<
        "}" <<
        endl;
    }

    // first repeat ending is in charge of
    // outputting the start of the alternative
    if (gLilypondOptions->fComments) {      
      fOstream <<
        endl <<
        idtr <<
        setw(commentFieldWidth) <<
        "\\alternative" " " "{" <<
        "% start of alternative" <<
        endl;
    }
    else {
      fOstream <<
        endl <<
        idtr <<
        "\\alternative" " " "{" <<
        endl;
    }

    idtr++;
  }

  // output the start of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gLilypondOptions->fComments) {  
        fOstream <<
          idtr <<
          setw(commentFieldWidth) <<
          "{" << "% start of repeat hooked ending" <<
          endl;
      }
      else {
        fOstream <<
          endl <<
          idtr <<
          "{" <<
          endl;
      }
      break;
      
    case msrRepeatEnding::kHooklessEnding:
      if (gLilypondOptions->fComments)    
        fOstream <<
          idtr <<
          setw(commentFieldWidth) <<
          "{" << "% start of repeat hookless ending" <<
          endl;
      else
        fOstream <<
          endl <<
          idtr <<
          "{" <<
          endl;
      break;
  } // switch

  idtr++;
  
  fMusicOlec.resetToZero ();
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrRepeatEnding" <<
      endl;

  // output the end of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gLilypondOptions->fComments) {
        fOstream <<
          idtr <<
          setw(commentFieldWidth) <<
          "}" << "% end of repeat hooked ending" <<
          endl;
      }
      else {
        fOstream <<
          endl <<
          idtr <<
          "}" <<
          endl;
      }
      break;
      
    case msrRepeatEnding::kHooklessEnding:
      if (gLilypondOptions->fComments)   { 
        fOstream <<
          idtr <<
          setw(commentFieldWidth) <<
          "}" << "% end of repeat hookless ending" <<
          endl;
      }
      else {
        fOstream <<
          endl <<
          idtr <<
          "}" <<
          endl;
      }
      break;
  } // switch

  if (
    elt->getRepeatEndingInternalNumber ()
      ==
    fCurrentRepeatEndingsNumber) {
      
    idtr--;
    
    // last repeat ending is in charge of
    // outputting the end of the alternative
    if (gLilypondOptions->fComments)   { 
      fOstream <<
        idtr <<
        setw(commentFieldWidth) <<
        "}" << "% end of alternative" <<
        endl;
    }
    else {
      fOstream <<
        endl <<
        idtr <<
        "}" <<
        endl;
    }
  }

  idtr--;
  
  fMusicOlec.resetToZero ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrComment" <<
      endl;

  fOstream << idtr <<
    "% " << elt->getContents () <<
    endl;
    
  if (elt->getCommentGapKind () == lpsrComment::kGapAfterwards)
    fOstream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrComment" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrSchemeFunction& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrSchemeFunction" <<
      endl;

  fOstream <<
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
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrSchemeFunction" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrRehearsal" <<
      endl;

  fOstream <<
    endl <<
    endl <<
    idtr <<
    "\\mark" "\\markup " "{" "\\box {"  "\""<< elt->getRehearsalText () << "\"" "}}" <<
    endl <<
    idtr;
  
}

void lpsr2LilypondTranslator::visitEnd (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrRehearsal" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMeasureRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrMeasureRepeat" <<
      endl;

  int repeatMeasuresNumber =
    elt->measureRepeatPatternMeasuresNumber ();

  int replicasMeasuresNumber =
    elt->measureRepeatReplicasMeasuresNumber ();
    
  int replicasNumber =
    elt->measureRepeatReplicasNumber ();

  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceRepeats) {
    cerr << idtr <<
      "% measure repeat, line " << elt->getInputLineNumber () << ":" <<
      endl;

    const int fieldWidth = 24;

    cerr << left <<
      idtr <<
        setw(fieldWidth) <<
        "% repeatMeasuresNumber" << " = " << repeatMeasuresNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "% replicasMeasuresNumber" << " = " << replicasMeasuresNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "% replicasNumber" << " = " << replicasNumber <<
        endl;
  }
  
  if (gLilypondOptions->fComments) {
    fOstream << idtr <<
      setw(commentFieldWidth) <<
      "% start of measure repeat" <<
      singularOrPlural (
        elt->measureRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOstream <<
    endl <<
    endl <<
    idtr <<
    "\\repeat" "percent " <<
    replicasNumber + 1 <<
     " { " <<
    endl <<
    idtr;

    idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrMeasureRepeat" <<
      endl;

  idtr--;
  
  fOstream <<
    endl <<
    endl <<
    idtr <<
    " }" <<
    endl <<
    idtr;

  if (gLilypondOptions->fComments) {
    if (fMusicOlec > 0) {
      fOstream <<
        endl <<
        idtr <<
          setw(commentFieldWidth) <<
          "% end of measure repeat " <<
          singularOrPlural (
            elt->measureRepeatReplicasNumber (),
            "replica",
              "replicas") <<
          ", line " << elt->getInputLineNumber () <<
          endl <<
          endl;
    }
    else {
      fOstream <<
        idtr <<
          setw(commentFieldWidth) <<
          "% end of measure repeat" <<
          singularOrPlural (
            elt->measureRepeatReplicasNumber (),
            "replica",
            "replicas") <<
          ", line " << elt->getInputLineNumber () <<
          endl <<
          endl;      
    }
  }
}

void lpsr2LilypondTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "%--> Start visiting msrMeasureRepeatPattern" <<
      endl;

  idtr++;

  // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "%--> End visiting msrMeasureRepeatPattern" <<
      endl;

  idtr--;
}
  
void lpsr2LilypondTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "%--> Start visiting msrMeasureRepeatReplicas" <<
      endl;

  idtr++;

  // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "%--> End visiting msrMeasureRepeatReplicas" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMultipleRest& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrMultipleRest" <<
      endl;

  int restMeasuresNumber =
    elt->getMultipleRestMeasuresNumber ();

  int restSegmentMeasuresNumber =
    elt->multipleRestContentsMeasuresNumber ();
    
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceRepeats) {
    cerr << idtr <<
      "% multiple rest, line " << elt->getInputLineNumber () << ":" <<
      endl;

    const int fieldWidth = 24;

    cerr << left <<
      idtr <<
        setw(fieldWidth) <<
        "% restMeasuresNumber" << " = " << restMeasuresNumber <<
        endl <<
      idtr <<
        setw(fieldWidth) <<
        "% restSegmentMeasuresNumber" << " = " << restSegmentMeasuresNumber <<
        endl <<
      endl;
  }
  
  if (gLilypondOptions->fComments) {
    fOstream << idtr <<
      setw(commentFieldWidth) <<
      "% start of multiple rest" <<
      singularOrPlural (
        elt->getMultipleRestMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fOstream <<
    endl <<
    endl <<
    idtr <<
    "R1" << // JMI
    "*" <<
    restMeasuresNumber <<
    " | ";
}

void lpsr2LilypondTranslator::visitEnd (S_msrMultipleRest& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrMultipleRest" <<
      endl;
    
  if (gLilypondOptions->fComments) {
    idtr--;

    if (fMusicOlec > 0) {
      fOstream <<
        endl <<
        idtr <<
          setw(commentFieldWidth) <<
          "% end of multiple rest " <<
          singularOrPlural (
            elt->getMultipleRestMeasuresNumber (),
            "measure",
            "measures") <<
          ", line " << elt->getInputLineNumber () <<
          endl <<
          endl;
    }
    else {
      fOstream <<
        idtr <<
          setw(commentFieldWidth) <<
          "% end of multiple rest" <<
          singularOrPlural (
            elt->getMultipleRestMeasuresNumber (),
            "measure",
            "measures") <<
          ", line " << elt->getInputLineNumber () <<
          endl <<
          endl;      
    }
  }
}

void lpsr2LilypondTranslator::visitStart (S_msrMultipleRestContents& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "%--> Start visiting msrMultipleRestContents" <<
      endl;

  idtr++;

  // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrMultipleRestContents& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "%--> End visiting msrMultipleRestContents" <<
      endl;

  idtr--;

  // JMI
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrMidi" <<
      endl;

  if (gLilypondOptions->fNoMidiCommand) {
    fOstream <<
      idtr <<
      "%{" <<
      endl;

     idtr++;
  }

  fOstream << idtr <<
    "\\midi" << " {" <<
    endl;
  
  idtr++;
  
  fOstream << idtr;

  if (gLilypondOptions->fNoMidiCommand)
    fOstream <<
      "% ";
  
  fOstream <<
    "\\tempo" " " <<
    elt->getMidiTempoDuration () <<
    " = " <<
    elt->getMidiTempoPerSecond () <<
    endl;
  
  idtr--;
  
  fOstream << idtr <<
    "}" <<
    endl;

  if (gLilypondOptions->fNoMidiCommand) {
    idtr--;

    fOstream <<
      idtr <<
      "%}" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrMidi" <<
      endl;
}


} // namespace




/* JMI
  // is there an unmetered (stemless) section?
  if (stemKind != fCurrentStemKind) {
    switch (stemKind) {
      case msrStem::kStemNone:
        if (! fOnGoingStemNone) {
          fOstream <<
            endl <<
            endl << //JMI
            idtr <<
              "\\temporary\\omit Stem" <<
              endl <<
            idtr <<
              "\\once\\omit Staff.TimeSignature" <<
              endl <<
            idtr <<
              "\\cadenzaOn" <<
              endl <<
            endl << //JMI
            idtr;

          fMusicOlec.resetToZero ();

          fOnGoingStemNone = true;
        }
        else {
          fOstream <<
            endl <<
            endl << //JMI
            idtr <<
              "\\cadenzaOff" <<
              endl <<
            idtr <<
              "\\bar \"|\"" <<
              endl <<
            idtr <<
              "\\undo\\omit Stem" <<
              endl <<
            endl << //JMI
            idtr;

          fMusicOlec.resetToZero ();
        }
        break;
        
      case msrStem::k_NoStem:
      case msrStem::kStemUp:
      case msrStem::kStemDown:
      case msrStem::kStemDouble:
        break;
    } // switch
  }
*/


  /*
    <notations>
      <ornaments>
        <tremolo type="single">3</tremolo>
      </ornaments>
    </notations>

The tremolo ornament can be used to indicate either single-note or double-note tremolos. Single-note tremolos use the single type, while double-note tremolos use the start and stop types. The default is "single" for compatibility with Version 1.1.

The text of the element indicates the number of tremolo marks and is an integer from 0 to 8.
Note that the number of attached beams is not included in this value, but is represented separately using the beam element.

When using double-note tremolos, the duration of each note in the tremolo should correspond to half of the notated type value.
A time-modification element should also be added with an actual-notes value of 2 and a normal-notes value of 1.
If used within a tuplet, this 2/1 ratio should be multiplied by the existing tuplet ratio.

Using repeater beams for indicating tremolos is deprecated as of MusicXML 3.0.

  */
