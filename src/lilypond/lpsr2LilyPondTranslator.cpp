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

#include "lpsr2LilyPondTranslator.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
lpsr2LilyPondTranslator::lpsr2LilyPondTranslator (
  S_msrOptions&  msrOpts,
  S_lpsrOptions& lpsrOpts,
  ostream&       os,
  S_lpsrScore    lpScore)
    : fOstream (os)
{
  fMsrOptions       = msrOpts;
  fLpsrOptions      = lpsrOpts;
  fVisitedLpsrScore = lpScore;

  fOnGoingHeader = false;
  
  fOnGoingStaff = false;

  fOnGoingVoice = false;
  fOnGoingHarmonyVoice = false;

  fOngoingNonEmptyStanza = false;

  fOnGoingNote = false;

  fCurrentStemKind = msrStem::k_NoStem;
//  fOnGoingStemNone = false; JMI

  fCurrentRepeatEndingsNumber = 0;
  
  fOnGoingScoreBlock = false;

  fMusicOlec.setMaxElementsPerLine (
    fLpsrOptions->fGenerateInputLineNumbers
      ?  5
      : 10);
      
  fStanzaOlec.setMaxElementsPerLine (10);
};
  
lpsr2LilyPondTranslator::~lpsr2LilyPondTranslator () {}

//________________________________________________________________________
void lpsr2LilyPondTranslator::generateLilyPondCodeFromLpsrScore ()
{
  if (fVisitedLpsrScore) {    
    // browse a msrScore browser
    msrBrowser<lpsrScore> browser (this);
    browser.browse (*fVisitedLpsrScore);
  }
}

//________________________________________________________________________
string lpsr2LilyPondTranslator::absoluteOctaveAsLilypondString (
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

//______________________________________________________________________________
string lpsr2LilyPondTranslator::lilypondizeDurationString (
  string msrDurationString)
{
  string result = msrDurationString;

  if (! isdigit (result [0])) {
    result [0] = tolower (result [0]);
    result = "\\" + result;
  }

  return result;
}

string lpsr2LilyPondTranslator::divisionsAsLilyPondString (
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

string lpsr2LilyPondTranslator::tupletDivisionsAsLilypondString (
  int inputLineNumber,
  S_msrPart part,
  int divisions,
  int actualNotes,
  int normalNotes)
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
string lpsr2LilyPondTranslator::noteAsLilyPondString (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();
    
  stringstream s;
  
  if (note->getNoteIsUnpitched ())
    s << "unpitched ";
  else
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
    fLpsrOptions->fGenerateAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;

  if (gGeneralOptions->fTraceNotes) {
    cerr << left <<
      endl <<
      idtr <<
          setw(33) << "% line" <<
          " = " <<
          note->getInputLineNumber () <<
          endl <<
      idtr <<
        setw(33) << "% msrQuartertonesPitch" <<
        " = - " <<
        msrQuartertonesPitchAsString (
          gLpsrOptions->fLpsrQuatertonesPitchesLanguage,
          noteQuartertonesPitch) <<
        " -" <<
        endl <<
      idtr <<
        setw(33) << "% noteAbsoluteOctave" <<
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
          getDiatonicPitch (
            inputLineNumber);

    msrDiatonicPitch
      referenceDiatonicPitch =
        fRelativeOctaveReference->
          getDiatonicPitch (
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
      cerr << left <<
/*
        idtr <<
          setw(33) <<
          "% referenceDiatonicPitch" <<
          " = " <<
          referenceDiatonicPitch <<
          endl <<
*/
        idtr <<
          setw(33) << "% referenceDiatonicPitchAsString" <<
          " = " <<
          referenceDiatonicPitchAsString <<
          endl <<
        idtr <<
          setw(33) << "% referenceAbsoluteOctave" <<
           " = " <<
           referenceAbsoluteOctave <<
           endl <<
        endl <<
        idtr <<
          setw(33) << "% referenceAboluteDiatonicOrdinal" <<
          " = " <<
          referenceAboluteDiatonicOrdinal <<
          endl <<
        idtr <<
          setw(33) << "% noteAboluteDiatonicOrdinal" <<
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
string noteDivisionsAsLpsrString (S_msrNote note)
{
  string
    noteDivisionsAsMsrString =
      note->noteDivisionsAsMsrString ();

  string
    lilyPondDivisionsAsString =
      noteDivisionsAsMsrString;

  if (! isdigit (lilyPondDivisionsAsString [0])) {
    if      (lilyPondDivisionsAsString == "Breve")
      lilyPondDivisionsAsString = "\\breve";
    else if (lilyPondDivisionsAsString == "Long")
      lilyPondDivisionsAsString = "\\long";
    else if (lilyPondDivisionsAsString == "Maxima")
      lilyPondDivisionsAsString = "\\maxima";
  }

  return lilyPondDivisionsAsString;
}
        
//________________________________________________________________________
string lpsr2LilyPondTranslator::technicalKindAsLilyPondString (
  S_msrTechnical technical)
{
  string result;

  /* JMI
  string
    noteDuration =
      (*i)->getTechnicalNoteUplink ()->
        noteDivisionsAsMsrString ();
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
string lpsr2LilyPondTranslator::technicalWithIntegerKindAsLilyPondString (
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
string lpsr2LilyPondTranslator::technicalWithStringKindAsLilyPondString (
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
    technicalWithString->
      getTechnicalWithStringValue ();
    
  return result;
}

//________________________________________________________________________
string lpsr2LilyPondTranslator::ornamentKindAsLilyPondString (
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
          gLpsrOptions->fDelayedOrnamentFractionDenominator
            -
          gLpsrOptions->fDelayedOrnamentFractionNumerator <<
          "/" <<
          gLpsrOptions->fDelayedOrnamentFractionDenominator <<
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
string lpsr2LilyPondTranslator::harmonyAsLilyPondString (
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  stringstream s;

  s <<
    msrDiatonicPitchAsString (
      gMsrOptions->fMsrQuatertonesPitchesLanguage,
      msrDiatonicPitchFromQuatertonesPitch (
        inputLineNumber,
        harmony->getHarmonyRootQuartertonesPitch ())) <<
           
    harmony->getHarmonyDirectPartUplink ()->
      divisionsAsMsrString (
        inputLineNumber,
        harmony->getHarmonyDivisions ());
    
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
      msrDiatonicPitchAsString (
        gMsrOptions->fMsrQuatertonesPitchesLanguage,
        msrDiatonicPitchFromQuatertonesPitch (
          inputLineNumber,
          harmonyBassQuartertonesPitch));

  return s.str();
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrScore" <<
      endl;

  // initial empty line in LilyPond code
  // to help copy/paste it
  fOstream << endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_lpsrScore& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_lpsrLilypondVarValAssoc& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrLilypondVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrLilypondVarValAssoc" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrSchemeVarValAssoc& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrSchemeVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrSchemeVarValAssoc" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrHeader& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrHeader& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_lpsrPaper& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrPaper" <<
      endl;

  fOstream << idtr <<
    "\\paper" << " {" <<
    endl;

  idtr++;
  
  if (elt->getPaperWidth () > 0) {
    fOstream << idtr <<
      setw(13) << left << "paper-width" << " = " <<
      setprecision(4) << elt->getPaperWidth () << "\\cm" <<
      endl;
  }
  if (elt->getPaperHeight () > 0) {
    fOstream << idtr << 
      setw(13) << left << "paper-height" << " = " <<
      setprecision(4) << elt->getPaperHeight () << "\\cm" <<
      endl;
  }
  
  if (elt->getTopMargin () > 0) {
    fOstream << idtr << 
      setw(13) << left << "top-margin" << " = " <<
      setprecision(4) << elt->getTopMargin () << "\\cm" <<
      endl;
  }
  if (elt->getBottomMargin () > 0) {
    fOstream << idtr << 
      setw(13) << left << "bottom-margin" << " = " <<
      setprecision(4) << elt->getBottomMargin () << "\\cm" <<
      endl;
  }
  if (elt->getLeftMargin () > 0) {
    fOstream << idtr << 
      setw(13) << left << "left-margin" << " = " <<
      setprecision(4) << elt->getLeftMargin () << "\\cm" <<
      endl;
  }
  if (elt->getRightMargin () > 0) {
    fOstream << idtr << 
      setw(13) << left << "right-margin" << " = " <<
    setprecision(4) << elt->getRightMargin () << "\\cm" <<
    endl;
  }

  fOstream << endl;

  // generate the default 'indent' setting ready for the user
  fOstream << idtr << 
    setw(13) << left << "indent" << " = " <<
  setprecision(4) << 1.5 << "\\cm" <<
  endl;

  // generate the default 'short-indent' setting ready for the user
  fOstream << idtr << 
    setw(13) << left << "short-indent" << " = " <<
  setprecision(4) << 1.0 << "\\cm" <<
  endl;

  fOstream << endl;

  // generate a 'page-count' comment ready for the user
  fOstream << idtr << 
    setw(13) << left << "%" "page-count" << " = " <<
  setprecision(4) << 1 <<
  endl;

  // generate a 'system-count' comment ready for the user
  fOstream << idtr << 
    setw(13) << left << "%" "system-count" << " = " <<
  setprecision(4) << 1 <<
  endl;

/*
 * 
  if (fBetweenSystemSpace > 0) {
    os << idtr << "between-system-space = " << setprecision(4) << fBetweenSystemSpace << "\\cm" <<
      endl;
  }

  if (elt->getPageTopSpace > 0) {
    os << idtr << "page-top-space = " << setprecision(4) << elt->getPageTopSpace << "\\cm" <<
      endl;
  }
*/
}

void lpsr2LilyPondTranslator::visitEnd (S_lpsrPaper& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_lpsrLayout& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrLayout& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrLayout" <<
      endl;

  if (gLpsrOptions->fRepeatBrackets) {
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
void lpsr2LilyPondTranslator::visitStart (S_lpsrScoreBlock& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrScoreBlock& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_lpsrParallelMusic& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrParallelMusic& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_lpsrPartgroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrPartgroupBlock" <<
      endl;

  // fetch part group
  S_msrPartgroup
    partgroup =
      elt->getPartgroup ();
      
  msrPartgroup::msrPartgroupSymbolKind
    partgroupSymbolKind =
      partgroup->getPartgroupSymbolKind ();

  bool
    partgroupBarline =
      partgroup->getPartgroupBarline ();
      
  string
    partgroupInstrumentName =
      partgroup->getPartgroupInstrumentName ();
      
  string partGroupContextName;

  // LPNR, page 567

  // don't generate code for a 1-element part group block
  if (elt->getPartgroupBlockElements ().size () > 1) {
    switch (partgroupSymbolKind) {
      case msrPartgroup::k_NoPartgroupSymbol:
        partGroupContextName = "";
        break;
        
      case msrPartgroup::kBracePartgroupSymbol: // JMI
      /*
       *
       * check whether individual part have instrument names
       * 
        if (partgroupInstrumentName.size ())
          partGroupContextName = "\\new PianoStaff";
        else
          partGroupContextName = "\\new GrandStaff";
          */
        partGroupContextName =
          partgroupBarline // only partgroup has an instrument name
            ? "\\new PianoStaff"
            : "\\new GrandStaff";
        break;
        
      case msrPartgroup::kBracketPartgroupSymbol:
        partGroupContextName =
          partgroupBarline
            ? "\\new StaffGroup"
            : "\\new ChoirStaff";
        break;
        
      case msrPartgroup::kLinePartgroupSymbol:
        partGroupContextName = "\\new StaffGroup";
        break;
        
      case msrPartgroup::kSquarePartgroupSymbol:
        partGroupContextName = "\\new StaffGroup";
        break;
    } // switch
  
    fOstream << idtr <<
      setw(30) << partGroupContextName + " " "<<";
      
    if (fLpsrOptions->fGenerateComments)
      fOstream <<
        "% part group " <<
        partgroup->getPartgroupCombinedName ();
        
    fOstream <<
      endl;
  }

  if (partgroupInstrumentName.size ())
    fOstream << idtr <<
      "\\set PianoStaff.instrumentName = #\"" <<
      partgroupInstrumentName <<
      "\"" <<
      endl;

    bool doConnectArpeggios = true; // JMI
    
    if (doConnectArpeggios)
      fOstream << idtr <<
        "\\set PianoStaff.connectArpeggios = ##t" <<
        endl;
  
    if (partgroupSymbolKind == msrPartgroup::kSquarePartgroupSymbol) {
      idtr++;
      fOstream << idtr <<
        "\\set StaffGroup.systemStartDelimiter = #'SystemStartSquare" <<
        endl;
      idtr--;
    }
       
  fOstream <<
    endl;

  if (elt->getPartgroupBlockElements ().size () > 1)
    idtr++;
}

void lpsr2LilyPondTranslator::visitEnd (S_lpsrPartgroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrPartgroupBlock" <<
      endl;

  // fetch part group
  S_msrPartgroup
    partgroup =
      elt->getPartgroup ();
      
  if (elt->getPartgroupBlockElements ().size () > 1)
    idtr--;

  // don't generate code for a 1-element part group block
  if (elt->getPartgroupBlockElements ().size () > 1) {
    fOstream <<
      idtr <<
      setw(30) << ">>";
    if (fLpsrOptions->fGenerateComments)
      fOstream <<
         "% part group " <<
        partgroup->getPartgroupCombinedName ();
    fOstream <<
      endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrPartBlock& elt)
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
  
    if (fLpsrOptions->fGenerateComments) {
      fOstream << left <<
        idtr <<
        setw(30) << "\\new PianoStaff <<" <<
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
        "\\set PianoStaff.instrumentName = #\"" <<
        partName <<
        "\"" <<
        endl;
    }
    
    if (partAbbreviation.size ()) {
      fOstream << idtr <<
        "\\set PianoStaff.shortInstrumentName = #\"" <<
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrPartBlock& elt)
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
  
    if (fLpsrOptions->fGenerateComments) {
      fOstream <<
        idtr <<
        setw(30) << ">>" <<    
        "% part " <<
        part->getPartCombinedName ();
    }
    else {
      fOstream <<
        idtr <<
        ">>";
    }
    
    fOstream <<
      endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrStaffBlock& elt)
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
      
  if (fLpsrOptions->fGenerateComments) {
    fOstream << left <<
      idtr <<
      setw(30) << newContext <<
      "% staff \"" << staff->getStaffName () << "\"";
  }
  else {
    fOstream << idtr <<
      newContext;   
  }
      
  fOstream <<
    endl;

  idtr++;

  // fetch part
  S_msrPart
    part =
      staff->getStaffDirectPartUplink ();

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

  list<S_msrStafftuning>
    stafftuningsList =
      staff->getStafftuningsList ();
      
  if (stafftuningsList.size ()) {
    // \set Staff.stringTunings = \stringTuning <c' g' d'' a''>

    fOstream << idtr <<
      "\\set Staff.stringTunings = \\stringTuning <";

    list<S_msrStafftuning>::const_iterator
      iBegin = stafftuningsList.begin(),
      iEnd   = stafftuningsList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      fOstream <<
        msrQuartertonesPitchAsString (
          gLpsrOptions->fLpsrQuatertonesPitchesLanguage,
 // JMI            elt->getInputLineNumber (),
          ((*i)->getStafftuningQuartertonesPitch ())) <<        
 // JMI       char (tolower ((*i)->getStafftuningStep ())) <<
        absoluteOctaveAsLilypondString (
          (*i)->getStafftuningOctave ());
      if (++i == iEnd) break;
      fOstream << " ";
    } // for

    fOstream <<
      ">" <<
      endl;
  }
}

void lpsr2LilyPondTranslator::visitEnd (S_lpsrStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrStaffBlock" <<
      endl;

  idtr--;

  if (fLpsrOptions->fGenerateComments) {
    fOstream <<
      idtr <<
      setw(30) << ">>" <<    
      "% staff " <<
      elt->getStaff ()->getStaffName ();
  }
  else {
    fOstream <<
      idtr <<
      ">>";
  }

  fOstream <<
    endl << endl;
}

/*
//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrNewStaffgroupBlock& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrNewStaffgroupBlock& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_lpsrNewStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffBlock" <<
      endl;

  idtr++;
}

void lpsr2LilyPondTranslator::visitEnd (S_lpsrNewStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffBlock" <<
      endl;

  idtr--;
}
*/

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrUseVoiceCommand& elt)
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
  
  string voiceContextName; // JMI ???
  
  switch (staffKind) {
    case msrStaff::kRegularStaff:
      voiceContextName = "\\context Staff";
      break;
      
    case msrStaff::kTablatureStaff:
      voiceContextName = "\\context TabStaff";
      break;
      
    case msrStaff::kPercussionStaff:
      voiceContextName = "\\context DrumVoice";
      break;
      
    case msrStaff::kHarmonyStaff:
      voiceContextName = "\\context ChordNames";
      break;
  } // switch

 // if (voice->getStaffRelativeVoiceNumber () > 0) { JMI
    // don't include the master voice in the staff by default
    
    fOstream << idtr <<
      "\\context " "Voice" " = " "\"" <<
      voice->getVoiceName () << "\"" << " <<" <<
       endl;
  
    idtr++;
  
    if (fLpsrOptions->fNoAutoBeaming)
      fOstream << idtr <<
        "\\set " << voiceContextName << ".autoBeaming = ##f" <<
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrUseVoiceCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrUseVoiceCommand" <<
      endl;
}
  
//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrNewLyricsBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewLyricsBlock" <<
      endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrNewLyricsBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrNewLyricsBlock" <<
      endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
    // JMI
  }
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrVariableUseCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrVariableUseCommand" <<
      endl;

  idtr++;
}

void lpsr2LilyPondTranslator::visitEnd (S_lpsrVariableUseCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrVariableUseCommand" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrContext& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrContext" <<
      endl;
}
  
//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrBarCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrBarCommand" <<
      endl;

  idtr++;
}

void lpsr2LilyPondTranslator::visitEnd (S_lpsrBarCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrBarCommand" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrMelismaCommand& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrMelismaCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrMelismaCommand" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrScore" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrScore" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrCredit" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrCredit" <<
      endl;
}

void lpsr2LilyPondTranslator::visitStart (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrCreditWords" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrCreditWords" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrPartgroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrPartgroup" <<
      elt->getPartgroupCombinedName () <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrPartgroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrPartgroup" <<
      elt->getPartgroupCombinedName () <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrPart" <<
      elt->getPartCombinedName () <<
      endl;

  fCurrentPart = elt;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrPart" <<
      elt->getPartCombinedName () <<
      endl;

  fCurrentPart = 0;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrStaff& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      endl;
  
  fOnGoingStaff = true;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrStaff& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_msrVoice& elt)
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

  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      if (! fLpsrOptions->fGenerateAbsoluteOctaves)
        fOstream << "\\relative ";
      break;
      
    case msrVoice::kHarmonyVoice:
      fOstream << "\\chordmode ";
      break;
      
    case msrVoice::kMasterVoice:
      // ?? JMI
      break;
  } // switch

  fOstream <<
    "{" <<
    endl;

  idtr++;

  if (gLpsrOptions->fGenerateGlobal) {
    fOstream <<
      idtr <<
        "\\global" <<
        endl <<
        endl;
  }
    
  if (gLpsrOptions->fDisplayMusic) {
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

  if (gLpsrOptions->fShowAllBarNumbers)
    fOstream <<
      idtr <<
        "\\set Score.barNumberVisibility = #all-bar-numbers-visible" <<
        endl <<
      idtr <<
        "\\override Score.BarNumber.break-visibility = ##(#f #t #t)" <<
        endl <<
      endl;

// JMI   \set Score.alternativeNumberingStyle = #'numbers-with-letters


  if (gLpsrOptions->fCompressFullBarRests)
    fOstream <<
      idtr <<
        "\\compressFullBarRests" <<
        endl <<
      endl;

  if (gLpsrOptions->fAccidentalStyle.size ())
    fOstream <<
      idtr <<
        "\\accidentalStyle Score." << gLpsrOptions->fAccidentalStyle <<
        endl <<
      endl;

  fRelativeOctaveReference = 0;

  fMusicOlec.resetToZero ();

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

void lpsr2LilyPondTranslator::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      endl;
  
  if (gLpsrOptions->fDisplayMusic) {
    fOstream <<
      idtr <<
        "}" <<
        endl;

    idtr--;
  }
    
  fOstream << idtr <<
    "}" <<
    endl <<
    endl;

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
void lpsr2LilyPondTranslator::visitStart (S_msrHarmony& elt)
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
      harmonyAsLilyPondString (elt) <<
      " ";
  }

/* JMI
  else if (fOnGoingChord) {
    // don't generate code for the code harmony,
    // this will be done after the chord itself JMI
  }
  */
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSegment" <<
      endl;

  if (fLpsrOptions->fGenerateComments) {
    fOstream << idtr <<
      setw(30) << "" << "% start of segment" <<
      endl;

    idtr++;
  }

  fSegmentNotesAndChordsCountersStack.push (0);
}

void lpsr2LilyPondTranslator::visitEnd (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrSegment" <<
      endl;

  if (fLpsrOptions->fGenerateComments) {
    idtr--;
    
    fOstream <<
      idtr <<
      setw(30) << "" << "% end of segment" <<
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
void lpsr2LilyPondTranslator::visitStart (S_msrMeasure& elt)
{    
  int
    inputLineNumber =
      elt->getInputLineNumber (),
    measureNumber =
      elt->getMeasureNumber ();
    
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrMeasure " <<
      measureNumber <<
      ", line " << inputLineNumber<<
      endl;

  if (fLpsrOptions->fGenerateComments) {
    fOstream << idtr <<
      setw(30) << "" << "% start of measure " <<
      measureNumber <<
      ", line " << inputLineNumber<<
      endl;

    idtr++;
  }

  fSegmentNotesAndChordsCountersStack.push (0);

  switch (elt->getMeasureKind ()) {
    case msrMeasure::kRegularMeasure:
      break;
      
    case msrMeasure::kIncompleteLeftMeasure:
      {
        int measureLength =
          elt->getMeasureLength ();

        if (measureLength <= 0) { // TEMP JMI ???
          // IGNORE
        }

        else {
          string
            partialDuration =
              divisionsAsLilyPondString (
                inputLineNumber,
                elt->getMeasureDirectPartUplink (),
                measureLength);
  
          fOstream << idtr <<
            "\\partial" " " << partialDuration <<
            endl;
        }
      }
      break;
      
    case msrMeasure::kIncompleteRightMeasure:
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
          if (gLpsrOptions->fBreakLinesAtIncompleteRightMeasures)
            fOstream << idtr <<
              "\\break" <<
              endl;
        }
      }
      break;

    case msrMeasure::kOverfullMeasure:
      {
        fOstream << idtr <<
          "\\cadenzaOn" <<
          endl;
      }
      break;
      
    case msrMeasure::kEmptyMeasure:
      {
        int
          measureDivisionsPerFullMeasure =
            elt->getMeasureDivisionsPerFullMeasure ();

        // generate a rest the duration of the measure
        fOstream <<
          "R" <<
            divisionsAsLilyPondString (
              inputLineNumber,
              elt->getMeasureDirectPartUplink (),
              measureDivisionsPerFullMeasure) <<
          " | ";
      }
      break;
  } // switch
}

void lpsr2LilyPondTranslator::visitEnd (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber (),
    measureNumber =
      elt->getMeasureNumber ();
    
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrMeasure " <<
      measureNumber <<
      ", line " << elt->getInputLineNumber () <<
      endl;

  switch (elt->getMeasureKind ()) {
    case msrMeasure::kRegularMeasure:
      break;
      
    case msrMeasure::kIncompleteLeftMeasure:
      break;
      
    case msrMeasure::kIncompleteRightMeasure:
      break;

    case msrMeasure::kOverfullMeasure:
      fOstream <<
        endl <<
        idtr <<
          "\\cadenzaOff" <<
          endl <<
        idtr<<
          "\\bar \"|\"" <<
          endl;
/* JMI
      idtr--;
      fOstream <<
        "}" <<
        endl <<
        "\\bar \"|\"" <<
        endl;
        */
      break;

    case msrMeasure::kEmptyMeasure:
      break;
  } // switch
    
  if (fLpsrOptions->fGenerateComments) {
    idtr--;

    if (fMusicOlec > 0) {
      fOstream <<
        endl <<
        idtr <<
          setw(30) << "" << "% end of measure " <<
          measureNumber <<
          ", line " << inputLineNumber<<
          endl << endl;
    }
    else {
      fOstream <<
        idtr <<
          setw(30) << "" << "% end of msrMeasure " <<
          measureNumber <<
          ", line " << inputLineNumber<<
          endl << endl;      
    }
  }

  if (gLpsrOptions->fSeparatorLineEveryNMeasures) {
    if (measureNumber % gLpsrOptions->fSeparatorLineEveryNMeasuresValue == 0)
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
void lpsr2LilyPondTranslator::visitStart (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
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

void lpsr2LilyPondTranslator::visitEnd (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
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
void lpsr2LilyPondTranslator::visitStart (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSyllable '" <<
      elt->syllableAsString () <<
      "'" <<
      endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
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
    
        case msrSyllable::kBarnumberCheckSyllable:
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
      
      if (fLpsrOptions->fGenerateInputLineNumbers)
        // print the note line number as a comment
        fOstream <<
          "%{ " << elt->getInputLineNumber () << " %} ";  
    }
  }
}

void lpsr2LilyPondTranslator::visitEnd (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrSyllable '" <<
      elt->syllableAsString () <<
      "'" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrClef '" <<
      elt->clefAsString () <<
      "'" <<
      endl;

  msrClef::msrClefKind clefKind = elt->getClefKind ();

  if (clefKind != msrClef::k_NoClef) {
    fOstream << idtr <<
      "\\clef" " \"";
  
    switch (clefKind) {
      case msrClef::k_NoClef:
        break;
      case msrClef::kTrebleClef:
        fOstream << "treble";
        break;
      case msrClef::kAltoClef:
        fOstream << "alto";
        break;
      case msrClef::kTenorClef:
        fOstream << "tenor";
        break;
      case msrClef::kBassClef:
        fOstream << "bass";
        break;
      case msrClef::kTrebleLine1Clef:
        fOstream << "treble line 1";
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
      case msrClef::kTablature4Clef:
        if (gLpsrOptions->fModernTab)
          fOstream << "moderntab";
        else
          fOstream << "tab";
        break;
      case msrClef::kTablature5Clef:
        if (gLpsrOptions->fModernTab)
          fOstream << "moderntab";
        else
          fOstream << "tab";
        break;
      case msrClef::kTablature6Clef:
        if (gLpsrOptions->fModernTab)
          fOstream << "moderntab";
        else
          fOstream << "tab";
        break;
      case msrClef::kTablature7Clef:
        if (gLpsrOptions->fModernTab)
          fOstream << "moderntab";
        else
          fOstream << "tab";
        break;
      case msrClef::kPercussionClef:
        fOstream << "perc";
        break;
    } // switch
  }
  
  fOstream <<
    "\"" <<
    endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrClef '" <<
      elt->clefAsString () <<
      "'" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrKey '" <<
      elt->keyAsString () <<
      "'" <<
      endl;

  fOstream << idtr <<
    "\\key " <<
    msrQuartertonesPitchAsString (
      gLpsrOptions->fLpsrQuatertonesPitchesLanguage,
      elt->getKeyTonicQuartertonesPitch ()) <<
    " \\" <<
    msrKey::keyModeKindAsString (
      elt->getKeyModeKind ()) <<
    endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrKey '" <<
      elt->keyAsString () <<
      "'"  <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTime '" <<
      elt->timeAsString () <<
      "'"  <<
      endl;

  if (fLpsrOptions->fGenerateNumericalTime)
    fOstream << "\\numericTimeSignature ";
    
  fOstream << idtr <<
    "\\time" " " <<
    elt->getBeatsNumber () <<
    "/" <<
    elt->getBeatsValue () <<
    endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTime '" <<
      elt->timeAsString () <<
      "'"  <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrTempo& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTempo" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrArticulation" <<
      endl;

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilyPondTranslator::visitEnd (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrArticulation" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTechnical" <<
      endl;

  // don't generate the technical here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilyPondTranslator::visitEnd (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTechnical" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTechnicalWithInteger" <<
      endl;

  // don't generate the technicalWithInteger here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilyPondTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTechnicalWithInteger" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrTechnicalWithString" <<
      endl;

  // don't generate the technicalWithString here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilyPondTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTechnicalWithString" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrOrnament" <<
      endl;

  // don't generate the ornament here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilyPondTranslator::visitEnd (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrOrnament" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSingleTremolo" <<
      endl;

  // don't generate the singleTremolo here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilyPondTranslator::visitEnd (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrSingleTremolo" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrDoubleTremolo" <<
      endl;

  // fetch double tremolo divisions
  int doubleTremoloDivisions =
    elt->
      getDoubleTremoloDivisions ();

  // the marks number determines the duration of the two elements:
  // '8' for 1, '16' for 2, etc
  fCurrentTremoloElementsLpsrDuration =
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
    fCurrentTremoloElementsLpsrDuration;

  if (divisionsPerDoubleTremoloElement <= 0) {
    stringstream s;

    s <<
      "divisionsPerDoubleTremoloElement = " <<
      divisionsPerDoubleTremoloElement <<
      " while it should be positive" <<
      endl <<
      tab << "partDivisionsPerQuarterNote = " << partDivisionsPerQuarterNote <<
      endl <<
      tab << "doubleTremoloDivisions = " << doubleTremoloDivisions <<
      endl <<
      tab << "fCurrentTremoloElementsLpsrDuration = " << fCurrentTremoloElementsLpsrDuration;
    
    msrInternalError (
      elt->getInputLineNumber (),
      s.str());
  }
    
  // the number of repeats is the quotient of the number of divisions
  // by the duration of the elements
  int numberOfRepeats =
    doubleTremoloDivisions
      /
    (2 * divisionsPerDoubleTremoloElement); // to account for both elements

//  if (false && gGeneralOptions->fTraceTremolos) { // JMI
  if (gGeneralOptions->fTraceTremolos) { // JMI
    cerr <<
      "% visitStart (S_msrDoubleTremolo&)" <<
      endl <<
      tab << "% partDivisionsPerQuarterNote = " << partDivisionsPerQuarterNote <<
      endl <<
      tab << "% doubleTremoloDivisions = " << doubleTremoloDivisions <<
      endl <<
      tab << "% fCurrentTremoloElementsLpsrDuration = " << fCurrentTremoloElementsLpsrDuration <<
      endl <<
      tab << "% divisionsPerDoubleTremoloElement = " << divisionsPerDoubleTremoloElement <<
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

void lpsr2LilyPondTranslator::visitEnd (S_msrDoubleTremolo& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrDynamics" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrDynamics" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrOtherDynamics" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrOtherDynamics" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrWords" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrWords" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSlur" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrSlur" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrLigature" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrLigature" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrWedge" <<
      endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrWedge" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrGracenotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrGracenotes" <<
      endl;

  if (elt->getGracenotesIsSlashed ())
    fOstream <<
      "\\slashedGrace"; // JMI "\\grace";
  else
    fOstream <<
      "\\grace"; // JMI "\\acciaccatura" \\appoggiatura
  fOstream << " { ";
}

void lpsr2LilyPondTranslator::visitEnd (S_msrGracenotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrGracenotes" <<
      endl;

  fOstream <<
    "} ";
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrAftergracenotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrAftergracenotes" <<
      endl;

 // JMI exists? if (elt->getGracenotesIsSlashed ())
  fOstream <<
    "\\afterGrace" " ";

/* JMI
  printNoteAsLilyPondString (
    elt->getAftergracenotesNote ());

  fOstream <<
    " { ";
    */
}

void lpsr2LilyPondTranslator::visitEnd (S_msrAftergracenotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrAftergracenotes" <<
      endl;

  fOstream <<
    "} ";
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrNote& elt)
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
  printNoteAsLilyPondString (elt);

  fOnGoingNote = true;
}

void lpsr2LilyPondTranslator::printNoteAsLilyPondString (S_msrNote note)
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
            "\\stemNeutral"; // JMI ""\\once\\omit Stem" " ";
        }

        // should stem direction be generated?
        if (fLpsrOptions->fGenerateStems) {
      
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
        if (fLpsrOptions->fGenerateStems) {
      
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

  // fetch the note single tremolo
  S_msrSingleTremolo
    noteSingleTremolo =
      note->getNoteSingleTremolo ();

  // print the note
  switch (note->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kStandaloneNote:
      // print the note name
      fOstream <<
        noteAsLilyPondString (note);
      
      // print the note duration
      fOstream <<
        lilypondizeDurationString (
          note->noteDivisionsAsMsrString ());

      // handle delayed ornaments if any
      if (note->getNoteHasADelayedOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        fOstream <<
          "*" <<
          gLpsrOptions->fDelayedOrnamentFractionNumerator <<
          "/" <<
          gLpsrOptions->fDelayedOrnamentFractionDenominator;
      
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
        noteAsLilyPondString (note);
      
      // print the note duration, i.e. the double tremolo elements duration
      fOstream <<
        fCurrentTremoloElementsLpsrDuration; // JMI

      // handle delayed ornaments if any
      if (note->getNoteHasADelayedOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        fOstream <<
          "*" <<
          gLpsrOptions->fDelayedOrnamentFractionNumerator <<
          "/" <<
          gLpsrOptions->fDelayedOrnamentFractionDenominator;
      
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
        noteAsLilyPondString (note);
      
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
        noteAsLilyPondString (note);
      
      // don't print the note duration,
      // it will be printed for the chord itself

      // inside chords, a note is relative to the preceding one
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kTupletMemberNote:
      if (! gLpsrOptions->fTupletsOnALine) {
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
          noteAsLilyPondString (note);
      
      // print the note duration
      S_msrTuplet
        tuplet =
          note->getNoteTupletUplink ();
          
      fOstream <<
        tupletDivisionsAsLilypondString (
          note->getInputLineNumber (),
          note->getNoteDirectPartUplink (),
          note->getNoteDivisions (),
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

void lpsr2LilyPondTranslator::visitEnd (S_msrNote& elt)
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
/* doubleTremolo JMI
  if (noteSingleTremolo) {
    // print the single tremolo repeat end
    fOstream <<
      "}"; // JMI
      */

  if (noteSingleTremolo) {
    int
      inputLineNumber =
        noteSingleTremolo-> getInputLineNumber ();
      
    int
      singleTremoloMarksNumber =
        noteSingleTremolo->
          getSingleTremoloMarksNumber ();

    S_msrNote
      singleTremoloNote =
        noteSingleTremolo->
          getSingleTremoloNoteUplink ();

    int
      singleTremoloNoteDivisions =
        singleTremoloNote->getNoteDisplayDivisions ();

    msrDuration
      singleTremoloNoteDuration =
        singleTremoloNote->getNoteGraphicDuration ();
    
    int durationToUse =
      singleTremoloMarksNumber; // JMI / singleTremoloNoteDivisions;
      
    if (gGeneralOptions->fTraceTremolos) {
      fOstream << idtr <<
        "% Generating single tremolo " <<
        noteSingleTremolo->singleTremoloAsString () <<
        " for note " <<
        elt->noteAsShortString () <<
        ", line = " << inputLineNumber <<
        ", singleTremoloMarksNumber = " << singleTremoloMarksNumber <<
        ", singleTremoloNoteDivisions = " << singleTremoloNoteDivisions <<
        ", durationToUse = " << durationToUse <<
        endl;
    }

    fOstream <<
      ":";

    /*
    The same output can be obtained by adding :N after the note,
    where N indicates the duration of the subdivision (it must be at least 8).
    If N is 8, one beam is added to the note’s stem.
    */
    
    if (singleTremoloNoteDuration >= kEighth)
      durationToUse +=
        1 + (singleTremoloNoteDuration - kEighth);

    fOstream <<
      int (pow (2, durationToUse + 2)) <<
      " ";
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
  
  if (fLpsrOptions->fGenerateInputLineNumbers)
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
        technicalKindAsLilyPondString ((*i));
      fMusicOlec++;

      switch ((*i)->getTechnicalPlacementKind ()) {
        case msrTechnical::k_NoPlacementKind:
          break;
        case msrTechnical::kAbove:
 // JMI         fOstream << "^";
          break;
        case msrTechnical::kBelow:
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
        technicalWithIntegerKindAsLilyPondString ((*i));
      fMusicOlec++;

      switch ((*i)->getTechnicalWithIntegerPlacementKind ()) {
        case msrTechnicalWithInteger::k_NoPlacementKind:
          break;
        case msrTechnicalWithInteger::kAbove:
 // JMI         fOstream << "^";
          break;
        case msrTechnicalWithInteger::kBelow:
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
        technicalWithStringKindAsLilyPondString ((*i));
      fMusicOlec++;

      switch ((*i)->getTechnicalWithStringPlacementKind ()) {
        case msrTechnicalWithString::k_NoPlacementKind:
          break;
        case msrTechnicalWithString::kAbove:
 // JMI         fOstream << "^";
          break;
        case msrTechnicalWithString::kBelow:
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
            noteDivisionsAsMsrString ();
        
      fOstream <<
        ornamentKindAsLilyPondString (
          (*i)->getInputLineNumber (), // some ornaments are not yet supported
          (*i)->getOrnamentKind (),
          noteDuration);
          // <<
        // JMI " ";
      fMusicOlec++;

      switch ((*i)->getOrnamentPlacementKind ()) {
        case msrOrnament::k_NoPlacementKind:
          break;
        case msrOrnament::kAbove:
 // JMI         fOstream << "^";
          break;
        case msrOrnament::kBelow:
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

      string wordsFontStyle =
        (*i)->getWordsFontStyle ();
        
      string wordsFontWeight =
        (*i)->getWordsFontWeight ();

      string markup;
      
      {
        // create markup apart to have its length available
        stringstream s;
        
        switch (wordsPlacementKind) {
          case msrWords::kAbove:
            s << "^";
            break;
          case msrWords::kBelow:
            s << "_";
            break;
        } // switch
      
        s <<
          "\\markup" << " { ";
        if (wordsFontStyle.size ())
          s <<
            "\\" << wordsFontStyle << " ";
        if (wordsFontWeight.size () && wordsFontWeight != "normal") // JMI
          s <<
            "\\" << wordsFontWeight << " ";
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

  if (elt->getNoteIsFollowedByGracenotes ())
    fOstream << " { ";

  fOnGoingNote = false;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrOctaveShift& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrOctaveShift" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrStem" <<
      endl;

  fCurrentStem = elt;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrStem" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrBeam& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_msrBeam& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrBeam" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrChord" <<
      endl;

  // indent before the fist chord of the msrSegment if needed
  if (++ fSegmentNotesAndChordsCountersStack.top () == 1)
    fOstream << idtr;

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

  // fetch the chord singleTremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      elt->getChordSingleTremolo ();
      
  if (chordSingleTremolo) {
    // print the single tremolo repeat start
    fOstream <<
      "\\repeat tremolo " << 8 << // JMI
      " {"; 
  }
  
  // don't take the chord into account for line breaking ??? JMI
  
  fOstream << "<";
  fMusicOlec++;
}

void lpsr2LilyPondTranslator::visitEnd (S_msrChord& elt)
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
        fCurrentTremoloElementsLpsrDuration; // JMI
  }
  else {
    // print the chord duration
    fOstream <<
      divisionsAsLilyPondString (
        elt->getInputLineNumber (),
        elt->getChordDirectPartUplink (),
        elt->getChordDivisions ());
  }
   
  fOstream <<
    " ";
    
  fMusicOlec++;

  // fetch the chord single tremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      elt->getChordSingleTremolo ();
      
  if (chordSingleTremolo) {
    // print the single tremolo repeat end
    fOstream <<
      "}"; // JMI
  }
  
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
        technicalKindAsLilyPondString ((*i)) <<
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
        technicalWithIntegerKindAsLilyPondString ((*i)) <<
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
        technicalWithStringKindAsLilyPondString ((*i)) <<
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
        ornamentKindAsLilyPondString (
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
        case msrWords::kAbove:
          fOstream << "^";
          break;
        case msrWords::kBelow:
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
void lpsr2LilyPondTranslator::visitStart (S_msrTuplet& elt)
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

  if (gLpsrOptions->fTupletsOnALine) {
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

void lpsr2LilyPondTranslator::visitEnd (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTuplet" <<
      endl;

  idtr--;

  if (gLpsrOptions->fTupletsOnALine) {
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
void lpsr2LilyPondTranslator::visitStart (S_msrTie& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrTie" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrSegno& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrSegno" <<
      endl;

  fOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.segno\" }" <<
    endl;
}

void lpsr2LilyPondTranslator::visitStart (S_msrCoda& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_msrEyeglasses& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting eyeglasses" <<
      endl;

  fOstream <<
    "\\eyeglasses ";
  fMusicOlec++;
}

void lpsr2LilyPondTranslator::visitStart (S_msrPedal& elt)
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
void lpsr2LilyPondTranslator::visitStart (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream <<
      endl <<
      idtr << "% --> Start visiting msrBarline" <<
      endl;

  switch (elt->getBarlineCategory ()) {
    
    case msrBarline::kStandaloneBar:
      fOstream <<
        endl <<
        idtr;
      
      switch (elt->getStyle ()) {
        case msrBarline::k_NoStyle:
          break;
        case msrBarline::kRegular:
          fOstream << "\\bar \"|\"";
          break;
        case msrBarline::kDotted:
          fOstream << "\\bar \";\"";
          break;
        case msrBarline::kDashed:
          fOstream << "\\bar \"!\"";
          break;
        case msrBarline::kHeavy:
          fOstream << "\\bar \".\"";
          break;
        case msrBarline::kLightLight:
          fOstream << "\\bar \"||\"";
          break;
        case msrBarline::kLightHeavy:
          fOstream << "\\bar \"|.\"";
          break;
        case msrBarline::kHeavyLight:
          fOstream << "\\bar \".|\"";
          break;
        case msrBarline::kHeavyHeavy:
          fOstream << "\\bar \"..\"";
          break;
        case msrBarline::kTick:
          fOstream << "\\bar \"'\"";
          break;
        case msrBarline::kShort:
          fOstream << "\\bar \"|short???\"";
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

      if (fLpsrOptions->fGenerateInputLineNumbers)
        // print the barline line number as a comment
        fOstream <<
          "%{ " << elt->getInputLineNumber () << " %} ";
      break;

    case msrBarline::kRepeatStart:
    case msrBarline::kRepeatEnd:
    case msrBarline::kHookedEndingStart:
    case msrBarline::kHookedEndingEnd:
    case msrBarline::kHooklessEndingStart:
    case msrBarline::kHooklessEndingEnd:
      // should not occur, since
      // LilyPond will take care of displaying the repeat
      break;

  } // switch
}

void lpsr2LilyPondTranslator::visitEnd (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream <<
      endl <<
      idtr << "% --> End visiting msrBarline" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrBarCheck" <<
      endl;
      
  int nextBarNumber =
    elt->getNextBarNumber ();

  // don't generate a bar check before the end of measure 1
 // JMI if (nextBarNumber > 1)

  if (fMusicOlec == 0)
    fOstream <<
      idtr;
      
  fOstream <<
    "| % " << nextBarNumber << " % bar check" <<
    endl;

  fMusicOlec.resetToZero ();
}

void lpsr2LilyPondTranslator::visitEnd (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrBarCheck" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrBarnumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrBarnumberCheck" <<
      endl;

  fOstream << idtr <<
    "\\barNumberCheck #" << elt->getNextBarNumber () <<
    endl;

  fMusicOlec.resetToZero ();
}

void lpsr2LilyPondTranslator::visitEnd (S_msrBarnumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrBarnumberCheck" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrBreak& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_msrBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrBreak" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrRepeat& elt)
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
  
  if (fLpsrOptions->fGenerateComments) {
    fOstream << idtr <<
      setw(30) << s.str() << "% start of repeat" <<
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

void lpsr2LilyPondTranslator::visitEnd (S_msrRepeat& elt)
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

    if (fLpsrOptions->fGenerateComments) {      
      fOstream <<
        idtr <<
        setw(30) << "}" << "% end of repeat" <<
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
void lpsr2LilyPondTranslator::visitStart (S_msrRepeatending& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrRepeatending" <<
      endl;

  if (elt->getRepeatendingInternalNumber () == 1) {
    
    idtr--;
    
    // first repeat ending is in charge of
    // outputting the end of the repeat
    if (fLpsrOptions->fGenerateComments) {      
      fOstream <<
        idtr <<
        setw(30) << "}" << "% end of repeat" <<
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
    if (fLpsrOptions->fGenerateComments) {      
      fOstream <<
        endl <<
        idtr <<
        setw(30) << "\\alternative" " " "{" <<
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
  switch (elt->getRepeatendingKind ()) {
    case msrRepeatending::kHookedEnding:
      if (fLpsrOptions->fGenerateComments) {  
        fOstream <<
          idtr <<
          setw(30) << "{" << "% start of repeat hooked ending" <<
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
      
    case msrRepeatending::kHooklessEnding:
      if (fLpsrOptions->fGenerateComments)    
        fOstream <<
          idtr <<
          setw(30) << "{" << "% start of repeat hookless ending" <<
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

void lpsr2LilyPondTranslator::visitEnd (S_msrRepeatending& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrRepeatending" <<
      endl;

  // output the end of the ending
  switch (elt->getRepeatendingKind ()) {
    case msrRepeatending::kHookedEnding:
      if (fLpsrOptions->fGenerateComments) {
        fOstream <<
          idtr <<
          setw(30) << "}" << "% end of repeat hooked ending" <<
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
      
    case msrRepeatending::kHooklessEnding:
      if (fLpsrOptions->fGenerateComments)   { 
        fOstream <<
          idtr <<
          setw(30) << "}" << "% end of repeat hookless ending" <<
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
    elt->getRepeatendingInternalNumber ()
      ==
    fCurrentRepeatEndingsNumber) {
      
    idtr--;
    
    // last repeat ending is in charge of
    // outputting the end of the alternative
    if (fLpsrOptions->fGenerateComments)   { 
      fOstream <<
        idtr <<
        setw(30) << "}" << "% end of alternative" <<
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
void lpsr2LilyPondTranslator::visitStart (S_lpsrComment& elt)
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

void lpsr2LilyPondTranslator::visitEnd (S_lpsrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrComment" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_lpsrSchemeFunction& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting lpsrSchemeFunction" <<
      endl;

  fOstream <<
    endl <<
    "% Scheme function: \"" << elt->getFunctionName () << "\"" <<
    // endl is in the decription
    elt->getFunctionDescription () <<
    // endl is in the decription
    elt->getFunctionCode () <<
    endl <<
    endl;
}

void lpsr2LilyPondTranslator::visitEnd (S_lpsrSchemeFunction& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting lpsrSchemeFunction" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrRehearsal" <<
      endl;

  fOstream <<
    endl <<
    endl <<
    idtr <<
    "\\mark" " \""<< elt->getRehearsalText () << "\"" <<
    endl <<
    idtr;
  
}

void lpsr2LilyPondTranslator::visitEnd (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> End visiting msrRehearsal" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilyPondTranslator::visitStart (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fOstream << idtr <<
      "% --> Start visiting msrMidi" <<
      endl;

  if (gLpsrOptions->fDontGenerateMidiCommand) {
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

  if (gLpsrOptions->fDontGenerateMidiCommand)
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

  if (gLpsrOptions->fDontGenerateMidiCommand) {
    idtr--;

    fOstream <<
      idtr <<
      "%}" <<
      endl;
  }
}

void lpsr2LilyPondTranslator::visitEnd (S_msrMidi& elt)
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
