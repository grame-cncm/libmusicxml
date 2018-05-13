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

#include "traceOptions.h"
#include "musicXMLOptions.h"
#include "lilypondOptions.h"

#include "xml2lyOptionsHandling.h"

#include "lpsr2LilypondTranslator.h"


using namespace std;

namespace MusicXML2
{

// for comments highlighting in the generated Lilypond code
const int commentFieldWidth = 30;

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

//________________________________________________________________________
lpsr2LilypondTranslator::lpsr2LilypondTranslator (
  S_msrOptions&    msrOpts,
  S_lpsrOptions&   lpsrOpts,
  indentedOstream& logIOstream,
  indentedOstream& lilypondOutputStream,
  S_lpsrScore      lpsrScore)
    : fLogOutputStream (
        logIOstream),
      fLilypondCodeIOstream (
        lilypondOutputStream)
{
  fMsrOptions  = msrOpts;
  fLpsrOptions = lpsrOpts;
  
  // the LPSR score we're visiting
  fVisitedLpsrScore = lpsrScore;

  // inhibit the browsing of measure repeats replicas,
  // since Lilypond only needs the repeat measure
  fVisitedLpsrScore->
    getMsrScore ()->
      setInhibitMeasuresRepeatReplicasBrowsing ();

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
  fMeasuresCounter = 0;
  
  // durations
  fLastMetWholeNotes = rational (0, 1);

  // notes
  fOnGoingNote = false;

  // articulations
  fCurrentArpeggioDirectionKind = kDirectionNone;

  // stems
  fCurrentStemKind = msrStem::k_NoStem;
//  fOnGoingStemNone = false; JMI

  // double tremolos

  // chords
  fOnGoingChord = false; // JMI

  // trills
  fOnGoingTrillSpanner = false;

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

  if (gTraceOptions->fTraceNotes) {
    fLilypondCodeIOstream <<
      endl <<
      "%{ absoluteOctave = " << absoluteOctave << " %} " <<
      endl;
  }

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
string lpsr2LilypondTranslator::alterationKindAsLilypondAccidentalMark (
  msrAlterationKind alterationKind)
{
  string result;
  
  switch (alterationKind) {
    case kTripleFlat:
      result = " \\markup { \\tripleflat } ";
      break;
    case kDoubleFlat:
      result = " \\markup { \\doubleflat } ";
      break;
    case kSesquiFlat:
      result = " \\markup { \\sesquiflat } ";
      break;
    case kFlat:
      result = " \\markup { \\flat } ";
      break;
    case kSemiFlat:
      result = " \\markup { \\semiflat } ";
      break;
    case kNatural:
      result = " \\markup { \\natural } ";
      break;
    case kSemiSharp:
      result = " \\markup { \\semisharp } ";
      break;
    case kSharp:
      result = " \\markup { \\sharp } ";
      break;
    case kSesquiSharp:
      result = " \\markup { \\sesquisharp } ";
      break;
    case kDoubleSharp:
      result = " \\markup { \\doublesharp } ";
      break;
    case kTripleSharp:
      result = " \\markup { \\triplesharp } ";
      break;
    case k_NoAlteration:
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::lilypondRelativeOctave (
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

  if (gTraceOptions->fTraceNotes) {
    const int fieldWidth = 28;

    fLilypondCodeIOstream << left <<
/*
      setw (fieldWidth) <<
      "% referenceDiatonicPitch" <<
      " = " <<
      referenceDiatonicPitch <<
      endl <<
*/
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

void lpsr2LilypondTranslator::printNoteAsLilypondString ( // JMI
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
        case msrLigature::k_NoLigature:
          break;
          
        case msrLigature::kLigatureStart:
          // generate ligature line end type if any
          switch (ligature->getLigatureLineEndKind ()) {
            case msrLigature::kLigatureLineEndUp:
              fLilypondCodeIOstream <<
                endl <<
                "\\once \\override Staff.LigatureBracket.edge-height = #'(.7 . .7)" <<
                endl;
              break;
            case msrLigature::kLigatureLineEndDown:
              fLilypondCodeIOstream <<
                endl <<
                "\\once \\override Staff.LigatureBracket.edge-height = #'(-.7 . .7)" <<
                endl;
              break;
            case msrLigature::kLigatureLineEndBoth:
              fLilypondCodeIOstream <<
                "%{ligatureLineEndBoth???%} ";
              break;
            case msrLigature::kLigatureLineEndArrow:
              fLilypondCodeIOstream <<
                "%{ligatureLineEndArrow???%} ";
              break;
            case msrLigature::kLigatureLineEndNone:
              fLilypondCodeIOstream <<
                endl <<
                "\\once \\override Staff.LigatureBracket.edge-height = #'(0 . 0)" <<
                endl;
              break;
            case msrLigature::k_NoLigatureLineEnd:
              fLilypondCodeIOstream <<
                "%{k_NoLigatureLineEnd???%} ";
              break;
          } // switch
          
          // generate ligature line type if any
          switch (ligature->getLigatureLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeIOstream <<
                endl <<
               // JMI "%\\once\\override Ligature.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fLilypondCodeIOstream <<
                endl <<
               // JMI "%\\once\\override Ligature.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fLilypondCodeIOstream <<
                endl <<
               // JMI "%\\once\\override Ligature.style = #'zigzag" <<
                endl;
              break;
          } // switch
          
          fLilypondCodeIOstream << "\\[ ";
          break;
          
        case msrLigature::kLigatureContinue:
          break;
          
        case msrLigature::kLigatureStop:
   // JMI       fLilypondCodeIOstream << "\\] ";
          break;
      } // switch
    } // for
  }

  // get note stem kind 
  msrStem::msrStemKind
    stemKind =
      fCurrentStem
        ? fCurrentStem->getStemKind ()
        : msrStem::k_NoStem;

  // handle note kind before printing note itself
  switch (note->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kRestNote:      
      break;
      
    case msrNote::kSkipNote:      
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
              fLilypondCodeIOstream <<
                endl <<
                "\\stemNeutral "; // JMI ""\\once\\omit Stem ";
            }
    
            // should stem direction be generated?
            if (gLilypondOptions->fStems) {
          
              if (fCurrentStem) {
                // should stem direction be generated?
                if (stemKind != fCurrentStemKind) {
                  switch (stemKind) {
                    case msrStem::k_NoStem:
                      fLilypondCodeIOstream << "\\stemNeutral ";
                      break;
                    case msrStem::kStemUp:
                      fLilypondCodeIOstream << "\\stemUp ";
                      break;
                    case msrStem::kStemDown:
                      fLilypondCodeIOstream << "\\stemDown ";
                      break;
                    case msrStem::kStemNone:
                      break;
                    case msrStem::kStemDouble: // JMI ???
                      break;
                  } // switch
                }
          
                // is there a stem kind change?
                if (stemKind != fCurrentStemKind)
                  fCurrentStemKind = stemKind;
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
          fLilypondCodeIOstream <<
            endl <<
            "\\stemNeutral"; // JMI ""\\once\\omit Stem ";
        }

        // should stem direction be generated?
        if (gLilypondOptions->fStems) {
      
          if (fCurrentStem) {
            // should stem direction be generated?
            if (stemKind != fCurrentStemKind) {
              switch (stemKind) {
                case msrStem::k_NoStem:
                  fLilypondCodeIOstream << "\\stemNeutral ";
                  break;
                case msrStem::kStemUp:
                  fLilypondCodeIOstream << "\\stemUp ";
                  break;
                case msrStem::kStemDown:
                  fLilypondCodeIOstream << "\\stemDown ";
                  break;
                case msrStem::kStemNone:
                  break;
                case msrStem::kStemDouble: // JMI ???
                  break;
              } // switch
            }
      
            // is there a stem kind change?
            if (stemKind != fCurrentStemKind)
              fCurrentStemKind = stemKind;
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
      break;
  } // switch

  // print the note itself
  switch (note->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kRestNote:
      {
        // get pitched rest status
        bool noteIsAPitchedRest =
          note->noteIsAPitchedRest ();
  
        if (noteIsAPitchedRest) {
          fLilypondCodeIOstream <<
            pitchedRestAsLilypondString (note);
        }
  
        else {
          // get the note sounding whole notes
          rational
            noteSoundingWholeNotes =
              note->getNoteSoundingWholeNotes ();
              
          // print the rest name and duration
          if (note->getNoteOccupiesAFullMeasure ()) {
            fLilypondCodeIOstream <<
              "R" <<
              multipleRestWholeNoteAsLilypondString (
                inputLineNumber,
                noteSoundingWholeNotes);
          }
          else {
            fLilypondCodeIOstream <<
              "r" <<
              durationAsLilypondString (
                inputLineNumber,
                noteSoundingWholeNotes);
          }
        }
            
        // is the rest pitched?
        if (noteIsAPitchedRest) {
          fLilypondCodeIOstream <<
            "\\rest ";

          // this note is the new relative octave reference
          // (the display quarter tone pitch and octave
          // have been copied to the note octave
          // in the msrNote::msrNote() constructor,
          // since the note octave is used in relative code generation)
          fRelativeOctaveReference = note;
        }
        else {
          // an unpitched rest is no relative octave reference,
          // the preceding one is kept
        }
      }
      break;
      
    case msrNote::kSkipNote:      
      // print the skip name
      fLilypondCodeIOstream << "s";
      
      // print the skip duration
      fLilypondCodeIOstream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteSoundingWholeNotes ());

      // a rest is no relative octave reference,
      // the preceding one is kept
      break;
      
    case msrNote::kStandaloneNote:
      {
        // print the note name
        fLilypondCodeIOstream <<
          notePitchAsLilypondString (note);
  
        rational
          noteSoundingWholeNotes =
            note->
              getNoteSoundingWholeNotes ();
          
        // print the note duration
        fLilypondCodeIOstream <<
          durationAsLilypondString (
            inputLineNumber,
            noteSoundingWholeNotes);
  
        // handle delayed ornaments if any
        if (note->getNoteDelayedTurnOrnament ())
          // c2*2/3 ( s2*1/3\turn) JMI
          // we need the explicit duration in all cases,
          // regardless of gGeneralOptions->fAllDurations
          fLilypondCodeIOstream <<
            wholeNotesAsLilypondString (
              inputLineNumber,
              noteSoundingWholeNotes) <<
            "*" <<
            gLilypondOptions->fDelayedOrnamentsFraction;
        
        // print the tie if any
        {
          S_msrTie noteTie = note->getNoteTie ();
        
          if (noteTie) {
            if (noteTie->getTieKind () == msrTie::kTieStart) {
              fLilypondCodeIOstream << " ~";
            }
          }
        }
  
        // this note is the new relative octave reference
        fRelativeOctaveReference = note;
      }
      break;

    case msrNote::kDoubleTremoloMemberNote:
      // print the note name
      fLilypondCodeIOstream <<
        notePitchAsLilypondString (note);
      
      // print the note duration
      fLilypondCodeIOstream <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          note->getNoteSoundingWholeNotes ());

      // handle delayed ornaments if any
      if (note->getNoteDelayedTurnOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        fLilypondCodeIOstream <<
          "*" <<
          gLilypondOptions->fDelayedOrnamentsFraction;
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeIOstream << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      // print the note name
      fLilypondCodeIOstream <<
        notePitchAsLilypondString (note);
      
      // print the grace note's graphic duration
      fLilypondCodeIOstream <<
        msrDurationKindAsString (
          note->
            getNoteGraphicDurationKind ());

      // print the dots if any JMI ???
      for (int i = 0; i < note->getNoteDotsNumber (); i++) {
        fLilypondCodeIOstream << ".";
      } // for
      
      // don't print the tie if any, 'acciacattura takes care of it
      /*
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeIOstream << "~ ";
          }
        }
      }
      */

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kChordMemberNote:
      {
        // print the note name
        fLilypondCodeIOstream <<
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
            i++) {
            S_msrTechnicalWithInteger
              technicalWithInteger = (*i);

            switch (technicalWithInteger->getTechnicalWithIntegerKind ()) {
              case msrTechnicalWithInteger::kBend:
                break;
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
      if (gLilypondOptions->fIndentTuplets) {
        fLilypondCodeIOstream <<
          endl;
      }
        
      // print the note name
      if (note->getNoteIsARest ()) {
        fLilypondCodeIOstream <<
          string (
            note->getNoteOccupiesAFullMeasure ()
              ? "R"
              : "r");
      }
      else {
        fLilypondCodeIOstream <<
          notePitchAsLilypondString (note);
      }
      
      // print the note (display) duration
      fLilypondCodeIOstream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kTieStart) {
            fLilypondCodeIOstream << "~ ";
          }
        }
      }

      // a rest is no relative octave reference,
      if (! note->getNoteIsARest ())
        // this note is the new relative octave reference
        fRelativeOctaveReference = note;
      break;
  } // switch

  fLilypondCodeIOstream << " ";

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

//________________________________________________________________________
string lpsr2LilypondTranslator::notePitchAsLilypondString (
  S_msrNote note)
{
  stringstream s;

  // is the note unpitched?
  if (note->getNoteIsUnpitched ()) {
    s <<
      "\\once \\override NoteHead #'style = #'cross ";
  }

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
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguageKind,
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
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguageKind,
        noteQuarterTonesDisplayPitchKind);
      
  // generate the pitch
  s <<
    quarterTonesPitchKindAsString;
    
  // in MusicXML, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();
    
  int noteAbsoluteDisplayOctave =
    note->getNoteDisplayOctave ();

  // should an absolute octave be generated?
  bool generateAbsoluteOctave =
    gLilypondOptions->fAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;

  if (gTraceOptions->fTraceNotes) {
    const int fieldWidth = 28;

    fLilypondCodeIOstream << left <<
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

  if (generateAbsoluteOctave) {
    // generate LilyPond absolute octave
    s <<
      absoluteOctaveAsLilypondString (
        noteAbsoluteOctave);
  }

  else {
    // generate LilyPond octave relative to fRelativeOctaveReference
    s <<
      lilypondRelativeOctave (note);
  }

  // should an accidental be generated? JMI this can be fine tuned with cautionary
  switch (note->getNoteAccidentalKind ()) {
    case msrNote::k_NoNoteAccidental:
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
      gLilypondOptions->fAllDurations;
  }
  
  if (generateExplicitDuration)
    result =
      wholeNotesAsLilypondString (
        inputLineNumber,
        wholeNotes);

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::durationAsExplicitLilypondString (
  int      inputLineNumber,
  rational wholeNotes)
{
  string result;
  
  result =
    wholeNotesAsLilypondString (
      inputLineNumber,
      wholeNotes);

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
    quarterTonesPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguageKind,
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
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguageKind,
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
    
  int noteAbsoluteDisplayOctave =
    note->getNoteDisplayOctave ();

  // should an absolute octave be generated?
  bool generateAbsoluteOctave =
    gLilypondOptions->fAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;

  if (gTraceOptions->fTraceNotes) {
    const int fieldWidth = 28;

    fLilypondCodeIOstream << left <<
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

  if (generateAbsoluteOctave) {
    // generate LilyPond absolute octave
    s <<
      absoluteOctaveAsLilypondString (
        noteAbsoluteDisplayOctave);
  }

  else {
    // generate LilyPond octave relative to fRelativeOctaveReference
    s <<
      lilypondRelativeOctave (note);
  }

  return s.str ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::writeNoteArticulationAsLilyponString (
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
      // this is handled in visitStart (S_msrFermata&)
      doGeneratePlacement = false;
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
        fLilypondCodeIOstream << "-";
        break;
      case kPlacementAbove:
        fLilypondCodeIOstream << "^";
        break;
      case kPlacementBelow:
        fLilypondCodeIOstream << "_";
        break;
    } // switch
  }

  // generate note articulation itself
  switch (articulation->getArticulationKind ()) {
    case msrArticulation::kAccent:
      fLilypondCodeIOstream << ">";
      break;
    case msrArticulation::kBreathMark:
      fLilypondCodeIOstream << "\\breathe";
      break;
    case msrArticulation::kCaesura:
    /* JMI
          fLilypondCodeIOstream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl;
     */
      fLilypondCodeIOstream <<
        endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        endl <<
        "\\breathe" <<
        endl;
      break;
    case msrArticulation::kSpiccato:
      fLilypondCodeIOstream <<
        "%{spiccato???%}";
      break;
    case msrArticulation::kStaccato:
      fLilypondCodeIOstream <<
        ".";
      break;
    case msrArticulation::kStaccatissimo:
      fLilypondCodeIOstream <<
        "!";
      break;
    case msrArticulation::kStress:
      fLilypondCodeIOstream <<
        "%{stress???%}";
      break;
    case msrArticulation::kUnstress:
      fLilypondCodeIOstream <<
        "%{unstress???%}";
      break;
    case msrArticulation::kDetachedLegato:
      fLilypondCodeIOstream <<
        "_"; // portato
      break;
    case msrArticulation::kStrongAccent:
      fLilypondCodeIOstream <<
        "^"; // marcato
      break;
    case msrArticulation::kTenuto:
      fLilypondCodeIOstream <<
        "-";
      break;
      
    case msrArticulation::kFermata:
      // this is handled in visitStart (S_msrFermata&)
      break;
      
    case msrArticulation::kArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      break;
    case msrArticulation::kNonArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      break;
    case msrArticulation::kDoit:
      fLilypondCodeIOstream <<
        "\\bendAfter #+4";
      break;
    case msrArticulation::kFalloff:
      fLilypondCodeIOstream <<
        "\\bendAfter #-4";
      break;
    case msrArticulation::kPlop:
      fLilypondCodeIOstream <<
        "%{plop???%}";
      break;
    case msrArticulation::kScoop:
      fLilypondCodeIOstream <<
        "%{scoop???%}";
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::writeChordArticulationAsLilyponString (
  S_msrArticulation articulation)
{  
  switch (articulation->getArticulationPlacementKind ()) {
    case kPlacementNone:
      fLilypondCodeIOstream << "-";
      break;
    case kPlacementAbove:
      fLilypondCodeIOstream << "^";
      break;
    case kPlacementBelow:
      fLilypondCodeIOstream << "_";
      break;
  } // switch

  switch (articulation->getArticulationKind ()) {
    case msrArticulation::kAccent:
      fLilypondCodeIOstream << ">";
      break;
    case msrArticulation::kBreathMark:
      fLilypondCodeIOstream << "\\breathe";
      break;
    case msrArticulation::kCaesura:
    /* JMI
          fLilypondCodeIOstream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl;
     */
      fLilypondCodeIOstream <<
        endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        endl <<
      "\\breathe" <<
        endl;
      break;
    case msrArticulation::kSpiccato:
      fLilypondCodeIOstream << "%{spiccato???%}";
      break;
    case msrArticulation::kStaccato:
      fLilypondCodeIOstream << "\\staccato"; // JMI "-.";
      break;
    case msrArticulation::kStaccatissimo:
      fLilypondCodeIOstream << "!";
      break;
    case msrArticulation::kStress:
      fLilypondCodeIOstream << "%{stress???%}";
      break;
    case msrArticulation::kUnstress:
      fLilypondCodeIOstream << "%{unstress%}";
      break;
    case msrArticulation::kDetachedLegato:
      fLilypondCodeIOstream << "_"; // portato
      break;
    case msrArticulation::kStrongAccent:
      fLilypondCodeIOstream << "^"; // marcato
      break;
    case msrArticulation::kTenuto:
      fLilypondCodeIOstream << "-";
      break;
      
    case msrArticulation::kFermata:
      // this is handled in visitStart (S_msrFermata&)
      break;
      
    case msrArticulation::kArpeggiato:
      fLilypondCodeIOstream << "\\arpeggio";
      break;
    case msrArticulation::kNonArpeggiato:
      fLilypondCodeIOstream << "\\arpeggio";
      break;
    case msrArticulation::kDoit:
      fLilypondCodeIOstream << "\\bendAfter #+4";
      break;
    case msrArticulation::kFalloff:
      fLilypondCodeIOstream << "\\bendAfter #-4";
      break;
    case msrArticulation::kPlop:
      fLilypondCodeIOstream << "%{plop%}";
      break;
    case msrArticulation::kScoop:
      fLilypondCodeIOstream << "%{scoop%}";
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
    case msrTechnicalWithInteger::kBend:
      s <<
        "\\bendAfter " <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
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
string lpsr2LilypondTranslator::ornamentAsLilypondString (
  S_msrOrnament ornament)
{
  string result;

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
        result = "\\trill ";
      }
      else {
        result = "\\startTrillSpan ";
      }
      break;
      
    case msrOrnament::kOrnamentDashes:
      if (! ornamentNoteUplink->getNoteWavyLineSpannerStart ()) {      
        result = "%{\\dashes%} ";
      }
      break;
      
    case msrOrnament::kOrnamentTurn:
      result = "\\turn ";
      break;
      
    case msrOrnament::kOrnamentInvertedTurn:
      result = "\\reverseturn ";
      break;
      
    case msrOrnament::kOrnamentDelayedTurn:
      {
        // c2*2/3 ( s2*1/3\turn
        int
          numerator =
            gLilypondOptions->
              fDelayedOrnamentsFraction.
                getDenominator (),
          denominator =
            gLilypondOptions->
              fDelayedOrnamentsFraction.
                getDenominator ();
              
        stringstream s;

        s <<
          "s" <<
          noteUplinkDuration <<
          "*" <<
            denominator
            -
            numerator <<
          "/" <<
            denominator <<
          "\\turn ";
          
        result = s.str ();
      }
      break;
      
    case msrOrnament::kOrnamentDelayedInvertedTurn:
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
          "*1/3" "\\reverseturn ";
          
        result = s.str ();
      }
      break;
      
    case msrOrnament::kOrnamentVerticalTurn:
      result =
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
      result = "\\mordent ";
      break;
      
    case msrOrnament::kOrnamentInvertedMordent:
      result = "%{\\inverted mordent???%} ";
      break;
      \
    case msrOrnament::kOrnamentSchleifer:
      result = "%{\\schleifer???%} ";
      break;
      
    case msrOrnament::kOrnamentShake:
      result = "%{\\shake???%} ";
      break;
      
    case msrOrnament::kOrnamentAccidentalMark:
      result =
        alterationKindAsLilypondAccidentalMark (
          ornament->
            getOrnamentAccidentalMark ());
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::spannerAsLilypondString (
  S_msrSpanner spanner)
{
  string result;
  
  switch (spanner->getSpannerKind ()) {     
    case msrSpanner::kSpannerTrill:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
     // JMI     result = "\\startTrillSpan ";
          fOnGoingTrillSpanner = true;
          break;
        case kSpannerTypeStop:
    // JMI      result = "\\stopTrillSpan ";
          fOnGoingTrillSpanner = false;
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          break;
      } // switch
      break;
         
    case msrSpanner::kSpannerDashes:
      switch (spanner->getSpannerTypeKind ()) {
        case kSpannerTypeStart:
          result = "\\startTextSpan ";
          fOnGoingTrillSpanner = true;
          break;
        case kSpannerTypeStop:
          result = "\\stopTextSpan ";
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
            result = "\\startTrillSpan";
            fOnGoingTrillSpanner = true;
          }
          else {
      // JMI      result = "-\\tweak style #'trill \\startTextSpan";
            stringstream s;

            s <<
              "\\once \\override TextSpanner #'style = #'trill" <<
            endl;

            result = s.str ();
          }
          break;
        case kSpannerTypeStop:
          if (fOnGoingTrillSpanner) {
      // JMI      result = "\\stopTrillSpan";
            fOnGoingTrillSpanner = false;
          }
          else {
        // JMI    result = "\\stopTextSpan";
          }
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
          break;
      } // switch
      break;      
  } // switch

  return result;
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
    gXml2lyOptions->fInputSourceName,
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

  S_msrNote
    singleTremoloNoteUplink =
      singleTremolo->
        getSingleTremoloNoteUplink ();

  msrDurationKind
    singleTremoloNoteDurationKind =
      singleTremoloNoteUplink->
        getNoteGraphicDurationKind ();
  
  /*
  The same output can be obtained by adding :N after the note,
  where N indicates the duration of the subdivision (it must be at least 8).
  If N is 8, one beam is added to the note’s stem.
  */
  
  int durationToUse =
    singleTremoloMarksNumber; // JMI / singleTremoloNoteSoundingWholeNotes;
        
  if (singleTremoloNoteDurationKind >= kEighth)
    durationToUse +=
      1 + (singleTremoloNoteDurationKind - kEighth);
  
  stringstream s;

  s <<
    ":" <<
    int (pow (2, durationToUse + 2)) <<
    " ";
    
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
  
  // print harmony pitch and duration
  s <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->
        fMsrQuarterTonesPitchesLanguageKind,
      harmony->
        getHarmonyRootQuarterTonesPitchKind ()) <<
    wholeNotesAsLilypondString (
      inputLineNumber,
      harmony->
        getHarmonySoundingWholeNotes ());
    
  // print harmony kind
  switch (harmony->getHarmonyKind ()) {
    case k_NoHarmony:
      s << "Harmony???";
      break;

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
      
  if (harmonyBassQuarterTonesPitchKind != k_NoQuarterTonesPitch_QTP)
    s <<
      "/" <<
      msrQuarterTonesPitchKindAsString (
        gMsrOptions->
          fMsrQuarterTonesPitchesLanguageKind,
        harmonyBassQuarterTonesPitchKind);


  // print harmony degrees if any
  list<S_msrHarmonyDegree>
    harmonyDegreesList =
      harmony->getHarmonyDegreesList ();

  if (harmonyDegreesList.size ()) {
    bool thereAreDegreesToBeRemoved = false;
    
    // print degrees to be added first
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
        case msrHarmonyDegree::kHarmonyDegreeAddType:
          {
            s <<
              "." <<
              harmonyDegreeValue <<
              harmonyDegreeAlterationKindAsLilypondString (
                harmonyDegreeAlterationKind);
          }
          break;
          
        case msrHarmonyDegree::kHarmonyDegreeAlterType:
          s <<
            "." <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationKindAsLilypondString (
              harmonyDegreeAlterationKind);
          break;
          
        case msrHarmonyDegree::kHarmonyDegreeSubtractType:
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
          case msrHarmonyDegree::kHarmonyDegreeAddType:
          case msrHarmonyDegree::kHarmonyDegreeAlterType:
            break;
            
          case msrHarmonyDegree::kHarmonyDegreeSubtractType:
            if (counter > 1)
              s << ".";
    
            s <<
              harmonyDegreeValue <<
              harmonyDegreeAlterationKindAsLilypondString (
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

//________________________________________________________________________
string lpsr2LilypondTranslator::generateMultilineName (string theString)
{
  stringstream s;

  s <<
     "\\markup { \\center-column { ";
     
  list<string> chunksList;

  splitStringContainingEndOfLines (
    theString,
    chunksList);

  if (chunksList.size ()) {
    /*
      \markup { \center-column {
        \line {"Long"} \line {"Staff"} \line {"Name"}
        } }
    */

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
      endl;
  }

  return s.str ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrScore" <<
      endl;
  }
  
  // initial empty line in LilyPond code
  // to help copy/paste it
// JMI  fLilypondCodeIOstream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrScore" <<
      endl;
  }

  // final empty line in LilyPond code
  // to help copy/paste it
  fLilypondCodeIOstream <<
    endl;
}

string lpsr2LilypondTranslator::lpsrVarValAssocKindAsLilypondString (
  lpsrVarValAssoc::lpsrVarValAssocKind
    lilyPondVarValAssocKind)
{
  string result;
  
  switch (lilyPondVarValAssocKind) {
    case lpsrVarValAssoc::kVersion:
      fLilypondCodeIOstream << "version";
      break;
    case lpsrVarValAssoc::kWorkNumber:
      fLilypondCodeIOstream << "workNumber";
      break;
    case lpsrVarValAssoc::kWorkTitle:
      fLilypondCodeIOstream << "title";
      break;
    case lpsrVarValAssoc::kMovementNumber:
      fLilypondCodeIOstream << "movementNumber";
      break;
    case lpsrVarValAssoc::kMovementTitle:
      fLilypondCodeIOstream << "subtitle";
      break;
    case lpsrVarValAssoc::kEncodingDate:
      fLilypondCodeIOstream << "encodingDate";
      break;
    case lpsrVarValAssoc::kScoreInstrument:
      fLilypondCodeIOstream << "scoreInstrument";
      break;
    case lpsrVarValAssoc::kMiscellaneousField:
      fLilypondCodeIOstream << "miscellaneousField";
      break;
    case lpsrVarValAssoc::kMyBreak:
      fLilypondCodeIOstream << "myBreak";
      break;
    case lpsrVarValAssoc::kMyPageBreak:
      fLilypondCodeIOstream << "myPageBreak";
      break;
    case lpsrVarValAssoc::kGlobal:
      fLilypondCodeIOstream << "global";
      break;
  } // switch

  return result;
}

string lpsr2LilypondTranslator::lpsrVarValsListAssocKindAsLilypondString (
  lpsrVarValsListAssoc::lpsrVarValsListAssocKind
    lilyPondVarValsListAssocKind)
{
  string result;
  
  switch (lilyPondVarValsListAssocKind) {
    case lpsrVarValsListAssoc::kRights:
      result = "copyright";
      break;
    case lpsrVarValsListAssoc::kComposer:
      result = "composer";
      break;
    case lpsrVarValsListAssoc::kArranger:
      result = "arranger";
      break;
    case lpsrVarValsListAssoc::kPoet:
      result = "poet";
      break;
    case lpsrVarValsListAssoc::kLyricist:
      result = "lyricist";
      break;
    case lpsrVarValsListAssoc::kSoftware:
      result = "software";
      break;
  } // switch

  return result;
}

void lpsr2LilypondTranslator::writeLpsrVarValsListAssocValuesAsLilyPondString (
  S_lpsrVarValsListAssoc varValsListAssoc,
  ostream&               os)
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
      os <<
        "\"" << variableValuesList.front () << "\"";
      break;

    default:
      // generate a markup containing the chunks
      os <<
        endl <<
        "\\markup {" <<
        endl;

      gIndenter++;
      
      os <<
        "\\column {" <<
        endl;
       
      gIndenter++;

      list<string>::const_iterator
        iBegin = variableValuesList.begin (),
        iEnd   = variableValuesList.end (),
        i      = iBegin;
      
      for ( ; ; ) {
        os <<
          "\"" << (*i) << "\"";
        if (++i == iEnd) break;
        os << endl;
      } // for
  
      os <<
        endl;
        
      gIndenter--;

      os <<
        "}" <<
        endl <<

      gIndenter--;

      os <<
        "}" <<
        endl;
  } // switch
}


//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrVarValAssoc" <<
      endl;
  }

  if (elt->getComment ().size ())
    fLilypondCodeIOstream <<
      "% " << elt->getComment () <<
      endl;

  if (elt->getCommentedKind () == lpsrVarValAssoc::kCommented)
    fLilypondCodeIOstream << "\%";
  
  switch (elt->getBackSlashKind ()) {
    case lpsrVarValAssoc::kWithBackSlash:
      fLilypondCodeIOstream << "\\";
      break;
    case lpsrVarValAssoc::kWithoutBackSlash:
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

  if (fOnGoingHeader)
    fieldWidth = 18;
  else
    fieldWidth = lilyPondVarValAssocKindAsLilypondString.size ();

  // generate the field name
  fLilypondCodeIOstream << left<<
    setw (fieldWidth) <<
    lilyPondVarValAssocKindAsLilypondString;

  if (elt->getVarValSeparator () == lpsrVarValAssoc::kEqualSign)
    fLilypondCodeIOstream << " = ";
  else
    fLilypondCodeIOstream << " ";
  
  if (elt->getQuotesKind () == lpsrVarValAssoc::kQuotesAroundValue)
    fLilypondCodeIOstream << "\"";

  // generate the value and unit if any
  if (elt->getUnit ().size ()) {
    fLilypondCodeIOstream <<
      setprecision (2) <<
      elt->getVariableValue ();

    fLilypondCodeIOstream <<
      "\\" <<
      elt->getUnit ();
  }
  else {
  fLilypondCodeIOstream <<
    elt->getVariableValue ();
  }
  
  if (elt->getQuotesKind () == lpsrVarValAssoc::kQuotesAroundValue)
    fLilypondCodeIOstream << "\"";
  
  fLilypondCodeIOstream << endl;

  switch (elt->getEndlKind ()) {
    case lpsrVarValAssoc::kWithEndl:
      fLilypondCodeIOstream <<
        endl;
      break;
      
    case lpsrVarValAssoc::kWithEndlTwice:
      fLilypondCodeIOstream <<
        endl <<
        endl;
      break;
      
    case lpsrVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrVarValAssoc" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrVarValsListAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrVarValsListAssoc" <<
      endl;
  }

  // the largest variable name length in a header is 18 JMI
  int fieldWidth;

  string
    lilyPondVarValsListAssocKindAsString =
      elt->lilyPondVarValsListAssocKindAsString ();
      
  if (fOnGoingHeader)
    fieldWidth = 18;
  else
    fieldWidth = lilyPondVarValsListAssocKindAsString.size ();
    
  fLilypondCodeIOstream << left<<
    setw (fieldWidth) <<
    lpsrVarValsListAssocKindAsLilypondString (
      elt->getVarValsListAssocKind ());
  
  fLilypondCodeIOstream << " = ";
      
  writeLpsrVarValsListAssocValuesAsLilyPondString (
    elt,
    fLilypondCodeIOstream);

  fLilypondCodeIOstream <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrVarValsListAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrVarValsListAssoc" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrSchemeVariable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrSchemeVariable" <<
      endl;
  }

  if (elt->getComment ().size ())
    fLilypondCodeIOstream <<
      "% " << elt->getComment () <<
      endl;

  if (elt->getCommentedKind () == lpsrSchemeVariable::kCommented)
    fLilypondCodeIOstream << "\% ";
  
  fLilypondCodeIOstream <<
    "#(" <<
    elt->getVariableName () <<
    " " <<
    elt->getVariableValue () <<
    ")";
    
  switch (elt->getEndlKind ()) {
    case lpsrSchemeVariable::kWithEndl:
      fLilypondCodeIOstream << endl;
      break;
    case lpsrSchemeVariable::kWithEndlTwice:
      fLilypondCodeIOstream << endl << endl;
      break;
    case lpsrSchemeVariable::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrSchemeVariable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrSchemeVariable" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrHeader& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrHeader" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\header" << " {" <<
    endl;

  gIndenter++;

  fOnGoingHeader = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrHeader& elt)
{
  fOnGoingHeader = false;

  gIndenter--;

  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrHeader" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPaper& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrPaper" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\paper" << " {" <<
    endl;

  gIndenter++;
  
  const int fieldWidth = 20;

  // page width, height, margins and indents

  {
    float paperWidth =
      elt->getPaperWidth ();
      
    if (paperWidth > 0) {
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
        setw (fieldWidth) <<
        "indent" << " = " <<
      setprecision (3) << indent << "\\cm" <<
      endl;
    }
    else {
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
        setw (fieldWidth) <<
        "short-indent" << " = " <<
      setprecision (3) << shortIndent << "\\cm" <<
      endl;
    }
    else {
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
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
      fLilypondCodeIOstream << left <<
        setw (fieldWidth) <<
        "evenFooterMarkup" << " = " <<
        evenFooterMarkup <<
        endl;
    }
  }

  fLilypondCodeIOstream <<
    endl;

  // generate a 'page-count' comment ready for the user
  fLilypondCodeIOstream << left <<
    setw (fieldWidth) <<
    "%" "page-count" << " = " <<
    setprecision (2) << 1 <<
    endl;

  // generate a 'system-count' comment ready for the user
  fLilypondCodeIOstream << left <<
    setw (fieldWidth) <<
    "%" "system-count" << " = " <<
    setprecision (2) << 1 <<
    endl;

  // fonts
  if (gLilypondOptions->fJazzFonts) {
    fLilypondCodeIOstream <<
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

void lpsr2LilypondTranslator::visitEnd (S_lpsrPaper& elt)
{
  gIndenter--;

  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrPaper" <<
      endl;
  }
      
  fLilypondCodeIOstream <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrLayout& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrLayout" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\layout {" <<
    endl;

  gIndenter++;

  fLilypondCodeIOstream <<
    "\\context {" <<
    endl <<
    gTab << "\\Score" <<
    endl;

  if (gLilypondOptions->fCompressMultiMeasureRests) { // JMI
    fLilypondCodeIOstream <<
      gTab << "skipBars = ##t % to compress multiple measure rests" <<
      endl;
  }

  fLilypondCodeIOstream <<
    gTab << "autoBeaming = ##f % to display tuplets brackets" <<
    endl <<

    "}" <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrLayout& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrLayout" <<
      endl;
  }

  if (gLilypondOptions->fRepeatBrackets) {
    fLilypondCodeIOstream <<
      "\\context " "{" <<
      endl;
    
    gIndenter++;

    fLilypondCodeIOstream <<
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
        
    fLilypondCodeIOstream <<
      "}" <<
      endl;
  }

  if (false) { // JMI XXL
    fLilypondCodeIOstream <<
      "\\context {" <<
      endl;
  
    gIndenter++;
  
    fLilypondCodeIOstream <<
      "\\Staff" <<
      endl <<
      "\\consists \"Span_arpeggio_engraver\"" <<
      endl;

    gIndenter--;

    fLilypondCodeIOstream <<
      "}" <<
      endl;
  }
    
  gIndenter--;

  fLilypondCodeIOstream <<
    "}" <<
    endl <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrScoreBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrScoreBlock" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\score {" <<
    endl;

  gIndenter++;
/* JMI
  if (elt->getScoreBlockElements ().size ()) {
    fLilypondCodeIOstream <<
      "<<" <<
      endl;
  
    gIndenter++;
  }
*/
  fOnGoingScoreBlock = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrScoreBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrScoreBlock" <<
      endl;
  }

/* JMI
  if (elt->getScoreBlockElements ().size ()) {
    gIndenter--;
    
    fLilypondCodeIOstream <<
      ">>" <<
      endl <<
      * endl;
  }
*/
  gIndenter--;
  
  fLilypondCodeIOstream <<
    "}" <<
    endl;

  fOnGoingScoreBlock = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrParallelMusicBLock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrParallelMusicBLock" <<
      endl;
  }
  
  fNumberOfPartGroupBlocks =
    elt->
      getParallelMusicBLockPartGroupBlocks ().size ();
    
  if (fNumberOfPartGroupBlocks) {
    if (gLilypondOptions->fComments) {
      fLilypondCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "<<" <<
        "% parallel music";
    }
    
    else {
      fLilypondCodeIOstream <<
        "<<";
    }
    
    fLilypondCodeIOstream <<
      endl;
  
    gIndenter++;
  }

  fCurrentParallelMusicBLock = elt;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrParallelMusicBLock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrParallelMusicBLock" <<
      endl;
  }
    
  if (fNumberOfPartGroupBlocks) {
    gIndenter--;
    
    if (gLilypondOptions->fComments) {
      fLilypondCodeIOstream << left <<
        setw (commentFieldWidth) <<
        ">>" <<
        "% parallel music";
    }

    else {
      fLilypondCodeIOstream <<
        ">>";
    }
    
    fLilypondCodeIOstream <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPartGroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrPartGroupBlock" <<
      endl;
  }

  fPartGroupBlocksCounter++;

  fNumberOfPartGroupBlockElements =
    elt -> getPartGroupBlockElements ().size ();
    
  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();

// JMI  fLilypondCodeIOstream << endl << endl << partGroup << endl << endl;

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
      if (gLilypondOptions->fComments) {
        fLilypondCodeIOstream << left <<
          setw (commentFieldWidth);
      }
      
      switch (partGroupSymbolKind) {
        case msrPartGroup::k_NoPartGroupSymbol:
          break;
          
        case msrPartGroup::kBracePartGroupSymbol: // JMI
          switch (partGroupBarlineKind) {
            case msrPartGroup::kPartGroupBarlineYes:
              fLilypondCodeIOstream <<
                "\\new PianoStaff";
              break;
            case msrPartGroup::kPartGroupBarlineNo:
              fLilypondCodeIOstream <<
                "\\new GrandStaff";
              break;
          } // switch
          break;
          
        case msrPartGroup::kBracketPartGroupSymbol:
          switch (partGroupBarlineKind) {
            case msrPartGroup::kPartGroupBarlineYes:
              fLilypondCodeIOstream <<
                "\\new StaffGroup";
              break;
            case msrPartGroup::kPartGroupBarlineNo:
              fLilypondCodeIOstream <<
                "\\new ChoirStaff";
              break;
          } // switch
          break;
          
        case msrPartGroup::kLinePartGroupSymbol:
          fLilypondCodeIOstream <<
            "\\new StaffGroup";
          break;
        
        case msrPartGroup::kSquarePartGroupSymbol:
          fLilypondCodeIOstream <<
            "\\new StaffGroup";
          break;
      } // switch

      // generate the '\with' block beginning
      fLilypondCodeIOstream <<
        endl <<
        "\\with {" <<
        endl;

      gIndenter++;

      if (partGroupName.size ()) {
        fLilypondCodeIOstream <<
          "instrumentName = \"" <<
          partGroupName <<
          "\"" <<
          endl;
      }
      if (partGroupAbbreviation.size ()) {
        fLilypondCodeIOstream <<
          "shortInstrumentName = \"" <<
          partGroupAbbreviation <<
          "\"" <<
          endl;
      }
            
      switch (partGroupSymbolKind) {
        case msrPartGroup::k_NoPartGroupSymbol:
          break;
          
        case msrPartGroup::kBracePartGroupSymbol: // JMI
          /*
           *
           * check whether individual part have instrument names JMI
           * 
            if (partGroupInstrumentName.size ())
              fLilypondCodeIOstream << = "\\new PianoStaff";
            else
              fLilypondCodeIOstream << = "\\new GrandStaff";
              */
          break;
          
        case msrPartGroup::kBracketPartGroupSymbol:
          break;
          
        case msrPartGroup::kLinePartGroupSymbol:
          fLilypondCodeIOstream <<
            "%kLinePartGroupSymbol" <<
            endl <<
            "systemStartDelimiter = #'SystemStartBar" <<
            endl;
          break;
        
        case msrPartGroup::kSquarePartGroupSymbol:
          fLilypondCodeIOstream <<
            "%kSquarePartGroupSymbol" <<
            endl <<
            "systemStartDelimiter = #'SystemStartSquare" <<
            endl;
          break;
      } // switch

      gIndenter--;

      // generate the '\with' block ending      
      fLilypondCodeIOstream <<
        endl <<
        "}" <<
        endl;

      if (gLilypondOptions->fComments) {
        fLilypondCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "<<" << "% part group " <<
          partGroup->getPartGroupCombinedName ();
      }
      else {
        fLilypondCodeIOstream <<
          "<<";
      }
          
      fLilypondCodeIOstream <<
        endl;
      break;
  } // switch

  if (partGroupInstrumentName.size ()) { // JMI
    fLilypondCodeIOstream <<
      "instrumentName = \"" <<
      partGroupInstrumentName <<
      "\"" <<
      endl;
  }

  bool doConnectArpeggios = true; // JMI
  
  if (doConnectArpeggios)
    fLilypondCodeIOstream <<
      "\\set PianoStaff.connectArpeggios = ##t" <<
      endl;

  if (partGroupSymbolKind == msrPartGroup::kSquarePartGroupSymbol) { // JMI
    gIndenter++;
    fLilypondCodeIOstream <<
      "systemStartDelimiter = #'SystemStartSquare" <<
      endl;
    gIndenter--;
  }
       
  fLilypondCodeIOstream <<
    endl;

  if (elt->getPartGroupBlockElements ().size () > 1)
    gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPartGroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrPartGroupBlock" <<
      endl;
  }

  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();
      
  if (elt->getPartGroupBlockElements ().size () > 1)
    gIndenter--;

  switch (partGroup->getPartGroupImplicitKind ()) {
    case msrPartGroup::kPartGroupImplicitYes:
      // don't generate code for an implicit top-most part group block
      break;
      
    case msrPartGroup::kPartGroupImplicitNo:
      if (gLilypondOptions->fComments) {
        fLilypondCodeIOstream << left <<
          setw (commentFieldWidth) << ">>" <<      
          "% part group " <<
          partGroup->getPartGroupCombinedName ();
      }
      else {
        fLilypondCodeIOstream <<
          ">>";        
      }
          
      fLilypondCodeIOstream <<
        endl;

      if (fPartGroupBlocksCounter != fNumberOfPartGroupBlocks) {
        fLilypondCodeIOstream <<
          endl;
      }
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPartBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrPartBlock" <<
      endl;
  }

  fPartGroupBlockElementsCounter++;

  fNumberOfStaffBlocksElements =
    elt->getPartBlockElementsList ().size ();
    
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
      fLilypondCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "\\new PianoStaff" <<
        "% part " << part->getPartCombinedName ();
    }
    else {
      fLilypondCodeIOstream <<
        "\\new PianoStaff";      
    }
    fLilypondCodeIOstream <<
      endl;
    
    // generate the 'with' block beginning
    fLilypondCodeIOstream <<
      "\\with {" <<  
      endl;

    gIndenter++;

    if (partName.size ()) {
      fLilypondCodeIOstream <<
        "instrumentName = \"" <<
        partName <<
        "\"" <<
        endl;
    }
    
    if (partAbbreviation.size ()) {
      fLilypondCodeIOstream <<
        "shortInstrumentName = \"" <<
        partAbbreviation <<
        "\"" <<
        endl;
    }

    bool doConnectArpeggios = true; // JMI
    
    if (doConnectArpeggios) {
      fLilypondCodeIOstream <<
        "connectArpeggios = ##t" <<
        endl;
    }
       
    gIndenter--;
  
    // generate the 'with' block ending
    fLilypondCodeIOstream <<
      "}" <<  
      endl;

    fLilypondCodeIOstream <<
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
      
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrPartBlock" <<
      endl;
  }

  if (part->getPartStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave

 // JMI ???   gIndenter--;
  
    if (gLilypondOptions->fComments) {
      fLilypondCodeIOstream << left <<
        setw (commentFieldWidth) << ">>" <<    
        "% part " <<
        part->getPartCombinedName ();
    }
    else {
      fLilypondCodeIOstream <<
        ">>";
    }
    
    fLilypondCodeIOstream <<
      endl;

    if (fPartGroupBlockElementsCounter != fNumberOfPartGroupBlockElements) {
      fLilypondCodeIOstream <<
        endl;
    }
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrStaffBlock" <<
      endl;
  }

  fStaffBlocksCounter++;
  
  S_msrStaff
    staff =
      elt->getStaff ();

  // generate the staff context command
  switch (staff->getStaffKind ()) {
    case msrStaff::kRegularStaff:
      if (gLilypondOptions->fJianpu)
        fLilypondCodeIOstream << "\\new JianpuStaff";
      else
        fLilypondCodeIOstream << "\\new Staff";
      break;
      
    case msrStaff::kTablatureStaff:
      fLilypondCodeIOstream << "\\new TabStaff";
      break;
      
    case msrStaff::kHarmonyStaff:
      fLilypondCodeIOstream << "\\new kHarmonyStaff???";
      break;
      
    case msrStaff::kFiguredBassStaff:
      fLilypondCodeIOstream << "\\new FiguredBassStaff???";
      break;

    case msrStaff::kDrumStaff:
      fLilypondCodeIOstream << "\\new DrumStaff";
      break;
      
    case msrStaff::kRythmicStaff:
      fLilypondCodeIOstream << "\\new RhythmicStaff";
      break;
    } // switch

  fLilypondCodeIOstream <<
    " = \"" <<
    staff->getStaffName () <<
    "\"";

  fLilypondCodeIOstream <<
    endl;
    
  // generate the 'with' block beginning
  fLilypondCodeIOstream <<
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
      fLilypondCodeIOstream <<
        "instrumentName = ";
  
      // does the name contain hexadecimal end of lines?
      std::size_t found =
        partName.find ("&#xd");
  
      if (found == string::npos) {
        // no, escape quotes if any and generate the result
        fLilypondCodeIOstream <<
          "\"" <<
          escapeQuotes (partName) <<
          "\"" <<
          endl;
      }
  
      else {
        // yes, split the name into a chunks list
        // and generate a \markup{}
        fLilypondCodeIOstream <<
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
      fLilypondCodeIOstream <<
        "shortInstrumentName = ";
        
      // does the name contain hexadecimal end of lines?
      std::size_t found =
        partAbbreviation.find ("&#xd");
  
      if (found == string::npos) {
        // no, merely generate the name
        fLilypondCodeIOstream <<
          "\"" <<
          partAbbreviation <<
          "\"" <<
          endl;
      }
  
      else {
        // yes, split the name into a chunks list
        // and generate a \markup{}
        fLilypondCodeIOstream <<
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
      fLilypondCodeIOstream <<
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
          
        fLilypondCodeIOstream <<
          msrQuarterTonesPitchKindAsString (
            gLpsrOptions->
              fLpsrQuarterTonesPitchesLanguageKind,
            staffTuning->
              getStaffTuningQuarterTonesPitchKind ()) <<
          absoluteOctaveAsLilypondString (
            staffTuning->getStaffTuningOctave ());
              
        if (++i == iEnd) break;
        
        fLilypondCodeIOstream << " ";
      } // for
        
      fLilypondCodeIOstream <<
        ">" <<
        endl;

      gIndenter--;

      // should letters be used for frets?
      switch (staffDetails->getShowFretsKind ()) {
        case msrStaffDetails::kShowFretsNumbers:
          break;
        case msrStaffDetails::kShowFretsLetters:
          fLilypondCodeIOstream <<
            "tablatureFormat = #fret-letter-tablature-format" <<
            endl;
          break;
      } // switch
    }
  }

  // generate the 'with' block ending
  fLilypondCodeIOstream <<
    "}" <<  
    endl;

  // generate the comment if relevant
  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "<<" <<
        " % staff \"" << staff->getStaffName () << "\"";
  }
  else {
    fLilypondCodeIOstream <<
      "<<";    
  }

  fLilypondCodeIOstream <<
    endl;
 
  if (gLilypondOptions->fJianpu) {
    fLilypondCodeIOstream <<
      " \\jianpuMusic" <<
      endl;
  }

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrStaffBlock" <<
      endl;
  }

  gIndenter--;

  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream << left <<
      setw (commentFieldWidth) << ">>" <<    
      "% staff " <<
      elt->getStaff ()->getStaffName ();
  }
  else {
    fLilypondCodeIOstream <<
      ">>";
  }

  fLilypondCodeIOstream <<
    endl;

  if (fStaffBlocksCounter != fNumberOfStaffBlocksElements) {
    fLilypondCodeIOstream <<
      endl;
  }
}

/*
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewStaffgroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrNewStaffgroupBlock" <<
      endl;
  }

   fLilypondCodeIOstream <<
     "\\new StaffGroup" << " " << "{" <<
      endl;

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewStaffgroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrNewStaffgroupBlock" <<
      endl;
  }

  gIndenter--;

  fLilypondCodeIOstream <<
    " }" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrNewStaffBlock" <<
      endl;
  }

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrNewStaffBlock" <<
      endl;
  }

  gIndenter--;
}
*/

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrUseVoiceCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrUseVoiceCommand" <<
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
      voiceContextName = "DrumVoiceVoice";
        // the "DrumVoice" alias exists, use it
      break;
      
    case msrStaff::kRythmicStaff:
      staffContextName = "\\context RhythmicStaff";
      voiceContextName = "Voice";
        // no "RhythmicVoice" alias exists
      break;
      
  } // switch

 // if (voice->getStaffRelativeVoiceNumber () > 0) { JMI    
    fLilypondCodeIOstream <<
      "\\context " << voiceContextName << " = " "\"" <<
      voice->getVoiceName () << "\"" << " <<" <<
       endl;
  
    gIndenter++;
  
    if (gLilypondOptions->fNoAutoBeaming)
      fLilypondCodeIOstream <<
        "\\set " << staffContextName << ".autoBeaming = ##f" <<
        endl;

    switch (staffKind) {
      case msrStaff::kRegularStaff:
        {
          int staffRegularVoicesCounter =
            staff->getStaffRegularVoicesCounter ();
            
          if (staffRegularVoicesCounter > 1) {
            switch (voice->getRegularVoiceStaffSequentialNumber ()) {
              case 1:
                fLilypondCodeIOstream << "\\voiceOne ";
                break;
              case 2:
                fLilypondCodeIOstream << "\\voiceTwo ";
                break;
              case 3:
                fLilypondCodeIOstream << "\\voiceThree ";
                break;
              case 4:
                fLilypondCodeIOstream << "\\voiceFour ";
                break;
              default:
                {}
            } // switch
    
            fLilypondCodeIOstream <<
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
  
    fLilypondCodeIOstream <<
      "\\" << voice->getVoiceName () << endl;
  
    gIndenter--;
    
    fLilypondCodeIOstream <<
      ">>" <<
      endl;
 // }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrUseVoiceCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrUseVoiceCommand" <<
      endl;
  }
}
  
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewLyricsBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrNewLyricsBlock" <<
      endl;
  }

  if (! gLilypondOptions->fNoLilypondLyrics) {
    S_msrStanza stanza = elt->getStanza ();
    
    fLilypondCodeIOstream <<
      "\\new Lyrics" <<
      endl;

    gIndenter++;
    
    fLilypondCodeIOstream <<
      "\\with {" <<
      endl <<
      gTab << "associatedVoice = " <<
      "\""  << elt->getVoice ()->getVoiceName () << "\"" <<
      endl;

    if (gMsrOptions->fAddStanzasNumbers) {
      fLilypondCodeIOstream <<
        gTab << "stanza = \"" <<
        stanza->getStanzaNumber () <<
        ".\"" <<
        endl;
    }
    
    fLilypondCodeIOstream <<
      "}" <<
      endl <<

      "\\" << stanza->getStanzaName () <<
      endl;

    gIndenter--;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewLyricsBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrNewLyricsBlock" <<
      endl;
  }

  if (! gLilypondOptions->fNoLilypondLyrics) {
    // JMI
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrVariableUseCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrVariableUseCommand" <<
      endl;
  }

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrVariableUseCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrVariableUseCommand" <<
      endl;
  }

  gIndenter--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrChordNamesContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrChordNamesContext" <<
      endl;
  }

  string
    contextTypeKindAsString =
      elt->getContextTypeKindAsString (),
    contextName =
      elt->getContextName ();
    
  fLilypondCodeIOstream <<
    "\\context " << contextTypeKindAsString <<
    " = \"" << contextName << "\"" <<
    endl;

/* JMI
  if (false) { //option JMI
    fLilypondCodeIOstream <<
      "\\with {" <<
      endl;

    gIndenter++;
    
    fLilypondCodeIOstream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      endl <<
      "\\consists \"Bar_engraver\"" <<
      endl;
        
    gIndenter--;
        
    fLilypondCodeIOstream <<
      "}" <<
      endl;
  }
      */
        
  fLilypondCodeIOstream <<
    "\\" << contextName <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrChordNamesContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrChordNamesContext" <<
      endl;
  }
}
  
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrFiguredBassContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrFiguredBassContext" <<
      endl;
  }

  string
    contextTypeKindAsString =
      elt->getContextTypeKindAsString (),
    contextName =
      elt->getContextName ();
    
  fLilypondCodeIOstream <<
    "\\context " << contextTypeKindAsString <<
    " = \"" << contextName << "\"" <<
    endl;

/* JMI
  if (false) { //option JMI
    fLilypondCodeIOstream <<
      "\\with {" <<
      endl;

    gIndenter++;
    
    fLilypondCodeIOstream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      endl <<
      "\\consists \"Bar_engraver\"" <<
      endl;
        
    gIndenter--;
        
    fLilypondCodeIOstream <<
      "}" <<
      endl;
  }
  */
        
  fLilypondCodeIOstream <<
    "\\" << contextName <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrFiguredBassContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrFiguredBassContext" <<
      endl;
  }
}
  
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrBarCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrBarCommand" <<
      endl;
  }

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrBarCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrBarCommand" <<
      endl;
  }

  gIndenter--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrMelismaCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrMelismaCommand" <<
      endl;
  }

  switch (elt->getMelismaKind ()) {
    case lpsrMelismaCommand::kMelismaStart:
// JMI      fLilypondCodeIOstream << "\\melisma ";
      break;
    case lpsrMelismaCommand::kMelismaEnd:
// JMI      fLilypondCodeIOstream << "\\melismaEnd ";
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrMelismaCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrMelismaCommand" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrScore" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrScore" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrComment" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "%{" <<
    endl;

  gIndenter++;
  
  fLilypondCodeIOstream <<
    elt->getCommentText () <<
    endl;
    
  gIndenter--;

  fLilypondCodeIOstream <<
    "%}" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrCredit" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrCredit" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitStart (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrCreditWords" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrCreditWords" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPartGroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrPartGroup" <<
      elt->getPartGroupCombinedName () <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrPartGroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrPartGroup" <<
      elt->getPartGroupCombinedName () <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPart& elt)
{
  string
    partCombinedName =
      elt->getPartCombinedName ();
      
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrPart" <<
      partCombinedName <<
      endl;
  }

  if (gTraceOptions->fTraceParts) {
    fLogOutputStream <<
      endl <<
      "<!--=== part \"" << partCombinedName << "\"" <<
      ", line " << elt->getInputLineNumber () << " ===-->" <<
      endl;
  }

  fCurrentPart = elt;

  fRemainingMultipleRestMeasuresNumber = 0; // JMI
  fOnGoingMultipleRestMeasures = false; // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrPart" <<
      elt->getPartCombinedName () <<
      endl;
  }

  fCurrentPart = nullptr;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStaff& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      endl;
  }
  
  fOnGoingStaff = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrStaff& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      endl;
  }
  
  fOnGoingStaff = false;
}

//________________________________________________________________________
/* JMI
void lpsr2LilypondTranslator::visitStart (S_msrStaffLinesNumber& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> Start visiting msrStaffLinesNumber" <<
      endl;
  }

  // fetch staff lines number
  int
    linesNumber =
      elt->getLinesNumber ();

  fLilypondCodeIOstream <<
    endl <<
    "\\stopStaff " <<
    "\\override Staff.StaffSymbol.line-count = " <<
    linesNumber <<
    " \\startStaff" <<
    endl;
}
*/

void lpsr2LilypondTranslator::visitStart (S_msrStaffTuning& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> Start visiting msrStaffTuning" <<
      endl;
  }

/* JMI
  list<S_msrStaffTuning>
    staffTuningsList =
      staff->getStaffTuningsList ();
      
  if (staffTuningsList.size ()) {
    // \set TabStaff.stringTunings = \stringTuning <c' g' d'' a''>

    fLilypondCodeIOstream <<
      "\\set TabStaff.stringTunings = \\stringTuning <";

    list<S_msrStaffTuning>::const_iterator
      iBegin = staffTuningsList.begin (),
      iEnd   = staffTuningsList.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      fLilypondCodeIOstream <<
        msrQuarterTonesPitchAsString (
          gLpsrOptions->fLpsrQuarterTonesPitchesLanguage,
 // JMI            elt->getInputLineNumber (),
          ((*i)->getStaffTuningQuarterTonesPitch ())) <<        
 // JMI       char (tolower ((*i)->getStaffTuningStep ())) <<
        absoluteOctaveAsLilypondString (
          (*i)->getStaffTuningOctave ());
      if (++i == iEnd) break;
      fLilypondCodeIOstream << " ";
    } // for

    fLilypondCodeIOstream <<
      ">" <<
      endl;
  }
 */
}

void lpsr2LilypondTranslator::visitStart (S_msrStaffDetails& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> Start visiting msrStaffDetails" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrVoice& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      endl;
  }

  fCurrentVoice = elt;
  
  fLilypondCodeIOstream <<
    fCurrentVoice->getVoiceName () << " = ";

  // generate the beginning of the voice definition
  switch (fCurrentVoice->getVoiceKind ()) {
    
    case msrVoice::kRegularVoice:
      if (gLilypondOptions->fAbsoluteOctaves)
        fLilypondCodeIOstream <<
          "{" <<
          endl;
      else
        fLilypondCodeIOstream <<
          "\\relative {" <<
          endl;
      break;
      
    case msrVoice::kHarmonyVoice:
      fLilypondCodeIOstream <<
        "\\chordmode {" <<
        endl;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fLilypondCodeIOstream <<
        "\\figuremode {" <<
        endl;
      break;
  } // switch

  gIndenter++;

  if (gLilypondOptions->fGlobal) {
    fLilypondCodeIOstream <<
      "\\global" <<
      endl <<
      endl;
  }
    
  if (gLilypondOptions->fDisplayMusic) {
    fLilypondCodeIOstream <<
      "\\displayMusic {" <<
      endl;

    gIndenter++;
  }
    
  fLilypondCodeIOstream <<
    "\\language \"" <<
    msrQuarterTonesPitchesLanguageKindAsString (
      gLpsrOptions->
        fLpsrQuarterTonesPitchesLanguageKind) <<
    "\"" <<
    endl;

  if (gLpsrOptions->fLpsrChordsLanguageKind != k_IgnatzekChords)
    fLilypondCodeIOstream <<
      "\\" <<
      lpsrChordsLanguageKindAsString (
        gLpsrOptions->
          fLpsrChordsLanguageKind) <<
      "Chords" <<
      endl;

  if (gLilypondOptions->fShowAllBarNumbers)
    fLilypondCodeIOstream <<
      "\\set Score.barNumberVisibility = #all-bar-numbers-visible" <<
      endl <<
      "\\override Score.BarNumber.break-visibility = ##(#f #t #t)" <<
      endl <<
      endl;

// JMI   \set Score.alternativeNumberingStyle = #'numbers-with-letters


  if (
    fCurrentVoice->getVoiceContainsMultipleRests ()
      ||
    gLilypondOptions->fCompressMultiMeasureRests) {
    fLilypondCodeIOstream <<
      "\\compressMMRests {" <<
      endl;

    gIndenter++;
  }

  if (gLilypondOptions->fAccidentalStyleKind != kDefaultStyle) {
    fLilypondCodeIOstream <<
      "\\accidentalStyle Score." <<
      lpsrAccidentalStyleKindAsString (
        gLilypondOptions->fAccidentalStyleKind) <<
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

  // force durations to be displayed explicitly
  // at the beginning of the voice
  fLastMetWholeNotes = rational (0, 1);
}

void lpsr2LilypondTranslator::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      endl;
  }
  
  if (
    fCurrentVoice->getVoiceContainsMultipleRests ()
      ||
    gLilypondOptions->fCompressMultiMeasureRests) {
    fLilypondCodeIOstream <<
      "}" <<
      endl;

    gIndenter--;
  }

  if (gLilypondOptions->fDisplayMusic) {
    fLilypondCodeIOstream <<
      "}" <<
      endl;

    gIndenter--;
  }
    
  // generate the end of the voice definition
  switch (elt->getVoiceKind ()) {
    case msrVoice::kRegularVoice:
      fLilypondCodeIOstream <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kHarmonyVoice:
      fLilypondCodeIOstream <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fLilypondCodeIOstream <<
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
void lpsr2LilypondTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrVoiceStaffChange '" <<
      elt->asString () << "'" <<
      endl;
  }

  fLilypondCodeIOstream <<
    endl <<
    "\\change Staff=\"" <<
    elt->getNewStaff ()->getStaffNumber () <<
    "\"" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrHarmony& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      ", fOnGoingNote = " << booleanAsString (fOnGoingNote) <<
      ", fOnGoingChord = " << booleanAsString (fOnGoingChord) <<
      ", fOnGoingHarmonyVoice = " << booleanAsString (fOnGoingHarmonyVoice) <<
      endl;
  }

  if (fOnGoingNote) {
    if (gTraceOptions->fTraceHarmonies) {
      fLilypondCodeIOstream <<
        "%{ " << elt->asString () << " %}" <<
        endl;
    }
  }
  
  else if (fOnGoingChord) { // JMI
    /*
    // register the harmony in the current chord clone
    fCurrentChord->
      setChordHarmony (elt); // JMI
      */
  }

  else if (fOnGoingHarmonyVoice) {
    fLilypondCodeIOstream <<
      harmonyAsLilypondString (elt) <<
      " ";
      
    if (gLilypondOptions->fNoteInputLineNumbers) {
      // print the harmony line number as a comment
      fLilypondCodeIOstream <<
        "%{ " << elt->getInputLineNumber () << " %} ";
    }
  }

/* JMI
  else if (fOnGoingChord) {
    // don't generate code for the code harmony,
    // this will be done after the chord itself JMI
  }
  */
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrFrame& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrHarmony '" <<
      elt->asString () <<
      "'" <<
      endl;
  }
  
  if (fOnGoingNote) {
    if (gTraceOptions->fTraceFrames) {
      fLilypondCodeIOstream <<
        "%{ " << elt->asString () << " %}" <<
        endl;
    }

  fLilypondCodeIOstream <<
    frameAsLilypondString (elt) <<
    endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrFiguredBass& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      endl;
  }

  fCurrentFiguredBass = elt;
  
  if (fOnGoingNote) {
    fLilypondCodeIOstream <<
      "%{ " << fCurrentFiguredBass->asString () << " %}" <<
      endl;
  }
  
  else if (fOnGoingChord) { // JMI
    /*
    // register the figured bass in the current chord clone
    fCurrentChord->
      setChordFiguredBass (elt); // JMI
      */
  }

  else if (fOnGoingFiguredBassVoice) {
    fLilypondCodeIOstream <<
      "<";
      
    if (gLilypondOptions->fNoteInputLineNumbers) {
      // print the figured bass line number as a comment
      fLilypondCodeIOstream <<
        " %{ " << fCurrentFiguredBass->getInputLineNumber () << " %} ";
    }
  }

/* JMI
  else if (fOnGoingChord) {
    // don't generate code for the code figured bass,
    // this will be done after the chord itself JMI
  }
  */

  fCurrentFiguredBassFiguresCounter = 0;
}

void lpsr2LilypondTranslator::visitStart (S_msrFigure& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrFigure '" <<
      elt->asString () <<
      "'" <<
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
      fLilypondCodeIOstream << "[";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      break;
  } // switch

  fLilypondCodeIOstream <<
    elt->getFigureNumber ();
    
  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      fLilypondCodeIOstream << "]";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      break;
  } // switch

  // handle the figure prefix
  switch (elt->getFigurePrefixKind ()) {
    case msrFigure::k_NoFigurePrefix:
      break;
    case msrFigure::kDoubleFlatPrefix:
      fLilypondCodeIOstream << "--";
      break;
    case msrFigure::kFlatPrefix:
      fLilypondCodeIOstream << "-";
      break;
    case msrFigure::kFlatFlatPrefix:
      fLilypondCodeIOstream << "flat flat";
      break;
    case msrFigure::kNaturalPrefix:
      fLilypondCodeIOstream << "!";
      break;
    case msrFigure::kSharpSharpPrefix:
      fLilypondCodeIOstream << "sharp sharp";
      break;
    case msrFigure::kSharpPrefix:
      fLilypondCodeIOstream << "+";
      break;
    case msrFigure::kDoubleSharpPrefix:
      fLilypondCodeIOstream << "++";
      break;
  } // switch

  // handle the figure suffix
  switch (elt->getFigureSuffixKind ()) {
    case msrFigure::k_NoFigureSuffix:
      break;
    case msrFigure::kDoubleFlatSuffix:
      fLilypondCodeIOstream << "double flat";
      break;
    case msrFigure::kFlatSuffix:
      fLilypondCodeIOstream << "flat";
      break;
    case msrFigure::kFlatFlatSuffix:
      fLilypondCodeIOstream << "flat flat";
      break;
    case msrFigure::kNaturalSuffix:
      fLilypondCodeIOstream << "natural";
      break;
    case msrFigure::kSharpSharpSuffix:
      fLilypondCodeIOstream << "sharp sharp";
      break;
    case msrFigure::kSharpSuffix:
      fLilypondCodeIOstream << "sharp";
      break;
    case msrFigure::kDoubleSharpSuffix:
      fLilypondCodeIOstream << "souble sharp";
      break;
    case msrFigure::kSlashSuffix:
      fLilypondCodeIOstream << "/";
      break;
  } // switch

  // generate a space if not last figure in figured bass
  if (
    fCurrentFiguredBassFiguresCounter
      <
    fCurrentFiguredBass->getFiguredBassFiguresList ().size ()) {
    fLilypondCodeIOstream << " ";
  }
  }

void lpsr2LilypondTranslator::visitEnd (S_msrFiguredBass& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrFiguredBass '" <<
      elt->asString () <<
      "'" <<
      endl;
  }

  fLilypondCodeIOstream <<
    ">" <<
    wholeNotesAsLilypondString (
      elt->getInputLineNumber (),
      elt->
        getFiguredBassSoundingWholeNotes ()) <<
    " ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;
  }

  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% start of segment" <<
      endl;

    gIndenter++;
  }

  fSegmentNotesAndChordsCountersStack.push (0);
}

void lpsr2LilypondTranslator::visitEnd (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;
  }

  if (gLilypondOptions->fComments) {
    gIndenter--;
    
    fLilypondCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% end of segment" <<
      endl;
  }
  /* JMI
  else
    fLilypondCodeIOstream <<
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

  if (gTraceOptions->fTraceMeasures) {
    fLogOutputStream <<
      endl <<
      "% <!--=== measure " << measureNumber <<
      ", line " << inputLineNumber << " ===-->" <<
      endl;
  }

  // take this measure into account
  fMeasuresCounter++;
  
  // force durations to be displayed explicitly
  // at the beginning of the measure
  fLastMetWholeNotes = rational (0, 1);

  // get measure kind
  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();
      
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrMeasure " <<
      measureNumber <<
      ", measureKind:" <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream << left <<
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
    fLilypondCodeIOstream <<
      "\\cadenzaOff";

    if (gLilypondOptions->fComments)
      fLilypondCodeIOstream <<
        " % kOverfullMeasureKind End";

    fLilypondCodeIOstream<<
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
/* JMI
        rational
          measureLength =
            elt->getMeasureLength ();

        fLilypondCodeIOstream <<
          "%{ measureLength: " << measureLength << " %}";
*/

        string
          upbeatDuration =
            wholeNotesAsLilypondString (
              inputLineNumber,
              elt->getMeasureLength ());

        fLilypondCodeIOstream <<
          "\\partial " << upbeatDuration <<
          endl;
      }
      break;
      
    case msrMeasure::kUnderfullMeasureKind:
      {
        rational
          measureLength =
            elt->getMeasureLength ();

        rational
          fullMeasureLength =
            elt->getFullMeasureLength ();

        // we should set the score measure length in this case
        rational
          ratioToFullLength =
            measureLength / fullMeasureLength;
        ratioToFullLength.rationalise ();
  
        if (gTraceOptions->fTraceMeasures) {
          const int fieldWidth = 27;
          
          fLilypondCodeIOstream << left <<
            "% Setting the measure length for measure " <<
            setw (fieldWidth) <<
            measureNumber <<
            ", line = " << inputLineNumber <<
            endl <<
            setw (fieldWidth) <<
            "% measureLength" << " = " << measureLength <<
            endl <<
            setw (fieldWidth) <<
            "% fullMeasureLength" << " = " << fullMeasureLength <<
            endl <<
            setw (fieldWidth) <<
            "% ratioToFullLength" << " = " << ratioToFullLength <<
            endl <<
            endl;
        }

        if (ratioToFullLength == rational (1, 1)) {
          msrInternalError (
            gXml2lyOptions->fInputSourceName,
            inputLineNumber,
            __FILE__, __LINE__,
            "underfull measure is actually the full measure length");
        }

        else {
          fLilypondCodeIOstream <<
            "\\set Score.measureLength = #(ly:make-moment " <<
            measureLength.toString () <<
            ")" <<
            endl;
    
          // should we generate a break?
          if (gLilypondOptions->fBreakLinesAtIncompleteRightMeasures)
            fLilypondCodeIOstream <<
              "\\break" <<
              endl;
        }
      }
      break;

    case msrMeasure::kOverfullMeasureKind:
      if (! fOnGoingVoiceCadenza) {
        fLilypondCodeIOstream <<
          "\\cadenzaOn";

        if (gLilypondOptions->fComments) {
          fLilypondCodeIOstream << " % kOverfullMeasureKind Start";
        }

        fLilypondCodeIOstream <<
          endl;

        fOnGoingVoiceCadenza = true;
      }
      break;
      
    case msrMeasure::kSenzaMisuraMeasureKind:
      if (! fOnGoingVoiceCadenza) {
        fLilypondCodeIOstream <<
          "\\cadenzaOn";

        if (gLilypondOptions->fComments) {
          fLilypondCodeIOstream << " % kSenzaMisuraMeasureKind Start";
        }

        fLilypondCodeIOstream <<
          endl;

        fOnGoingVoiceCadenza = true;
      }
      break;
      
    case msrMeasure::kEmptyMeasureKind:
      {
        // generate a rest the duration of the measure
        // followed by a bar check
        fLilypondCodeIOstream <<
          "R" <<
          wholeNotesAsLilypondString (
            inputLineNumber,
            elt->
              getFullMeasureLength ()) <<
          " | " <<
          endl;
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
  string
    nextMeasureNumber =
      elt->getNextMeasureNumber ();
    
  // get measure kind
  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();
      
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrMeasure " <<
      measureNumber <<
      ", measureKind:" <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", line " << inputLineNumber <<
      endl;
  }

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
          fLilypondCodeIOstream <<
            "|";
    
          if (nextMeasureNumber.size ()) {
            fLilypondCodeIOstream <<
              " % " <<
              nextMeasureNumber;
          }
          
          fLilypondCodeIOstream <<
            endl;
        }
      }
      break;
      
    case msrMeasure::kUpbeatMeasureKind:
      fLilypondCodeIOstream <<
        "|";

      if (nextMeasureNumber.size ()) {
        fLilypondCodeIOstream <<
          " % " <<
          nextMeasureNumber;
      }
      
      fLilypondCodeIOstream <<
        endl;
      break;
      
    case msrMeasure::kUnderfullMeasureKind:
      fLilypondCodeIOstream <<
        endl <<
        "\\unset Score.measureLength" <<
        endl;
      break;

    case msrMeasure::kOverfullMeasureKind:
      fLilypondCodeIOstream <<
        endl <<
        "\\cadenzaOff" <<
        endl;

      fOnGoingVoiceCadenza = false;
      
/* JMI
      gIndenter--;
      fLilypondCodeIOstream <<
        "}" <<
        endl <<
        "\\bar \"|\"" <<
        endl;
        */
      break;

    case msrMeasure::kSenzaMisuraMeasureKind:
      fLilypondCodeIOstream <<
          "\\cadenzaOff" <<
          endl <<
          "\\bar \"|\"" <<
          endl;

      fOnGoingVoiceCadenza = false;
      break;

    case msrMeasure::kEmptyMeasureKind:
      break;
  } // switch
    
  if (gLilypondOptions->fComments) {
    gIndenter--;

    fLilypondCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% end of measure " <<
      measureNumber <<
      ", line " << inputLineNumber <<
      endl <<
      endl;      
  }

  if (gLilypondOptions->fSeparatorLineEveryNMeasures > 0) {
    if (
      fMeasuresCounter
        %
      gLilypondOptions->fSeparatorLineEveryNMeasures
        ==
      0)
      fLilypondCodeIOstream <<
        endl <<
        "% ============================= " <<
        endl <<
        endl;
  }
  
  fSegmentNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      endl;
  }

  if (! gLilypondOptions->fNoLilypondLyrics) {
    // don't generate code for the stanza inside the code for the voice
    fGenerateCodeForOngoingNonEmptyStanza =
      ! fOnGoingVoice
        &&
      elt->getStanzaTextPresent ();

    if (fGenerateCodeForOngoingNonEmptyStanza) {
      fLilypondCodeIOstream <<
        elt->getStanzaName () << " = " << "\\lyricmode {" <<
        endl;
        
      gIndenter++;
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      endl;
  }

  if (! gLilypondOptions->fNoLilypondLyrics) {
    if (fGenerateCodeForOngoingNonEmptyStanza) {
      gIndenter--;
    
      fLilypondCodeIOstream <<
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
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrSyllable '" <<
      elt->asString () <<
      "'" <<
      endl;
  }

  if (! gLilypondOptions->fNoLilypondLyrics) {
    if (fGenerateCodeForOngoingNonEmptyStanza) {
      switch (elt->getSyllableKind ()) {
        case msrSyllable::kSyllableSingle:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeIOstream);
            
          fLilypondCodeIOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableSingle %} ";
          }
          break;
          
        case msrSyllable::kSyllableBegin:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeIOstream);
            
          fLilypondCodeIOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            " -- ";
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableBegin %} ";
          }
          break;
          
        case msrSyllable::kSyllableMiddle:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeIOstream);
            
          fLilypondCodeIOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            " -- ";
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableMiddle %} ";
          }
          break;
          
        case msrSyllable::kSyllableEnd:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fLilypondCodeIOstream);
            
          fLilypondCodeIOstream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableEnd %} ";
          }
          break;
                  
        case msrSyllable::kSyllableSkip:
          // LilyPond ignores the skip duration
          // when \lyricsto is used
          fLilypondCodeIOstream <<
            "\\skip" <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableSkip %} ";
          }
          break;
           
        case msrSyllable::kSyllableMeasureEnd:
          fLilypondCodeIOstream <<
            "%{ measure end, line " <<
            elt->getInputLineNumber () <<
            " %}" <<
            endl;
          break;
    
        case msrSyllable::kSyllableLineBreak:
          fLilypondCodeIOstream <<
            "%{ line break, line " <<
            elt->getInputLineNumber () <<
            " %}" <<
            endl;
          break;
    
        case msrSyllable::kSyllablePageBreak:
          fLilypondCodeIOstream <<
            "%{ page break, line " <<
            elt->getInputLineNumber () <<
            " %}" <<
            endl;
          break;
    
        case msrSyllable::k_NoSyllable: // JMI
          break;
      } // switch

      switch (elt->getSyllableExtendKind ()) {
        case msrSyllable::kSyllableExtendSingle:
          // generate a lyric extender, i.e. a melisma,
          // after this syllable
          fLilypondCodeIOstream <<
            "__ ";
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableExtendSingle %} ";
          }
          break;
          
        case msrSyllable::kSyllableExtendStart:
          // generate a lyric extender, i.e. a melisma,
          // after this syllable
          fLilypondCodeIOstream <<
            "__ ";
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableExtendStart %} ";
          }
          break;
          
        case msrSyllable::kSyllableExtendContinue:
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableExtendContinue %} ";
          }
          break;
          
        case msrSyllable::kSyllableExtendStop:
          if (gTraceOptions->fTraceLyrics) {
            fLilypondCodeIOstream <<
              "%{ kSyllableExtendStop %} ";
          }
          break;
          
        case msrSyllable::k_NoSyllableExtend:
          break;
      } // switch
      
      if (gLilypondOptions->fNoteInputLineNumbers) {
        // print the note line number as a comment
        fLilypondCodeIOstream <<
          "%{ " << elt->getInputLineNumber () << " %} ";
      } 
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrSyllable '" <<
      elt->asString () <<
      "'" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrClef '" <<
      elt->asString () <<
      "'" <<
      endl;
  }

  msrClef::msrClefKind
    clefKind =
      elt->getClefKind ();

  if (clefKind != msrClef::k_NoClef) {
    fLilypondCodeIOstream <<
      "\\clef ";
  
    switch (clefKind) {
      case msrClef::k_NoClef:
        break;
      case msrClef::kTrebleClef:
        fLilypondCodeIOstream << "\"treble\"";
        break;
      case msrClef::kSopranoClef:
        fLilypondCodeIOstream << "\"soprano\"";
        break;
      case msrClef::kMezzoSopranoClef:
        fLilypondCodeIOstream << "\"mezzosoprano\"";
        break;
      case msrClef::kAltoClef:
        fLilypondCodeIOstream << "\"alto\"";
        break;
      case msrClef::kTenorClef:
        fLilypondCodeIOstream << "\"tenor\"";
        break;
      case msrClef::kBaritoneClef:
        fLilypondCodeIOstream << "\"baritone\"";
        break;
      case msrClef::kBassClef:
        fLilypondCodeIOstream << "\"bass\"";
        break;
      case msrClef::kTrebleLine1Clef:
        fLilypondCodeIOstream << "\"french\"";
        break;
      case msrClef::kTrebleMinus15Clef:
        fLilypondCodeIOstream << "\"treble_15\"";
        break;
      case msrClef::kTrebleMinus8Clef:
        fLilypondCodeIOstream << "\"treble_8\"";
        break;
      case msrClef::kTreblePlus8Clef:
        fLilypondCodeIOstream << "\"treble^8\"";
        break;
      case msrClef::kTreblePlus15Clef:
        fLilypondCodeIOstream << "\"treble^15\"";
        break;
      case msrClef::kBassMinus15Clef:
        fLilypondCodeIOstream << "\"bass_15\"";
        break;
      case msrClef::kBassMinus8Clef:
        fLilypondCodeIOstream << "\"bass_8\"";
        break;
      case msrClef::kBassPlus8Clef:
        fLilypondCodeIOstream << "\"bass^8\"";
        break;
      case msrClef::kBassPlus15Clef:
        fLilypondCodeIOstream << "\"bass^15\"";
        break;
      case msrClef::kVarbaritoneClef:
        fLilypondCodeIOstream << "\"varbaritone\"";
        break;
      case msrClef::kTablature4Clef:
      case msrClef::kTablature5Clef:
      case msrClef::kTablature6Clef:
      case msrClef::kTablature7Clef:
        if (gLilypondOptions->fModernTab)
          fLilypondCodeIOstream <<
            "\"moderntab\"" <<
            endl <<
            "\\tabFullNotation";
        else
          fLilypondCodeIOstream <<
            "\"tab\"" <<
            endl <<
            "\\tabFullNotation";
        break;
      case msrClef::kPercussionClef:
        fLilypondCodeIOstream << "\"percussion\"";
        break;
      case msrClef::kJianpuClef:
        fLilypondCodeIOstream << "%{jianpuClef???%}";
        break;
    } // switch
  
  fLilypondCodeIOstream <<
    endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrClef '" <<
      elt->asString () <<
      "'" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrKey '" <<
      elt->asString () <<
      "'" <<
      endl;
  }

  switch (elt->getKeyKind ()) {
    case msrKey::kTraditionalKind:
      fLilypondCodeIOstream <<
        "\\key " <<
        msrQuarterTonesPitchKindAsString (
          gLpsrOptions->fLpsrQuarterTonesPitchesLanguageKind,
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
          fLilypondCodeIOstream <<
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
    
                                      
                fLilypondCodeIOstream <<
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
                                      
                fLilypondCodeIOstream <<
                  "(" <<
                  item->getKeyItemDiatonicPitchKind () <<
                  " . ," <<
                  alterationKindAsLilypondString (
                    item->getKeyItemAlterationKind ()) <<                
                  ")";
            }
                
            if (++i == iEnd) break;
            
            fLilypondCodeIOstream << " ";
          } // for
  
          fLilypondCodeIOstream <<
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

void lpsr2LilypondTranslator::visitEnd (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrKey '" <<
      elt->asString () <<
      "'"  <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTime " <<
      elt->asString () <<
      endl;
  }

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
    fLilypondCodeIOstream <<
      "\\undo\\omit Staff.TimeSignature" <<
      endl;

    fVoiceIsCurrentlySenzaMisura = false;
  }

  // handle the time
  if (timeSymbolKind == msrTime::kTimeSymbolSenzaMisura) {
    // senza misura time
    
    fLilypondCodeIOstream <<
      "\\omit Staff.TimeSignature" <<
      endl;

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
          fLilypondCodeIOstream <<
            "\\once\\override Staff.TimeSignature.style = #'single-digit" <<
            endl;
          break;
        case msrTime::kTimeSymbolSenzaMisura:
          break;
        case msrTime::k_NoTimeSymbol:
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
          timeSymbolKind == msrTime::k_NoTimeSymbol
            ||
          gLilypondOptions->fNumericalTime) {
          fLilypondCodeIOstream <<
            "\\numericTimeSignature ";
        }
  
        fLilypondCodeIOstream <<
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
    
        fLilypondCodeIOstream <<
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
          fLilypondCodeIOstream <<
            "(";
  
          // then generate all beats numbers in the vector
          for (int j = 0; j < beatsNumbersNumber; j++) {
            fLilypondCodeIOstream <<
              beatsNumbersVector [j] <<
              " ";
          } // for
      
          // then generate the beat type
          fLilypondCodeIOstream <<
            timeItem->getTimeBeatValue ();
  
          // and finally generate the closing parenthesis
          fLilypondCodeIOstream <<
            ")";
  
          if (i != timesItemsNumber - 1)
            fLilypondCodeIOstream <<
              " ";
        } // for
              
      fLilypondCodeIOstream <<
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

void lpsr2LilypondTranslator::visitEnd (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTime " <<
      elt->asString () <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTranspose& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTranspose" <<
      endl;
  }

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
          gXml2lyOptions->fInputSourceName,
          elt->getInputLineNumber (),
          __FILE__, __LINE__,
          s.str ());
      }
  } // switch

  // 4 is the octave starting with middle C
  int transpositionOctave;
  
  if (transposeChromatic < 0)
    transpositionOctave = 3;
  else
    transpositionOctave = 4;

  // take the transpose octave change into account
  transpositionOctave += transposeOctaveChange;

  // take the transpose double if any into account
  if (transposeDouble)
    transpositionOctave--;

  string
    transpositionPitchKindAsString =
      msrQuarterTonesPitchKindAsString (
        gLpsrOptions->
          fLpsrQuarterTonesPitchesLanguageKind,
        transpositionPitchKind);

  string
    transpositionOctaveAsString =
      absoluteOctaveAsLilypondString (
        transpositionOctave);

/* JMI
  if (gTraceOptions->fTraceTranspositions) {
    fLilypondCodeIOstream << // JMI
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
*/

  // now we can generate the transpostion command
  fLilypondCodeIOstream <<
    "\\transposition " <<
    transpositionPitchKindAsString <<
    transpositionOctaveAsString <<
    " " <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTranspose& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTranspose" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPartNameDisplay& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrPartNameDisplay" <<
      endl;
  }

  string partNameDisplayValue =
    elt->getPartNameDisplayValue ();
    
  fLilypondCodeIOstream <<
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
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrPartAbbreviationDisplay" <<
      endl;
  }

  string partAbbreviationDisplayValue =
    elt->getPartAbbreviationDisplayValue ();

  fLilypondCodeIOstream <<
    "\\set Staff.shortInstrumentName = \"" <<
    partAbbreviationDisplayValue <<
    "\"" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTempo" <<
      endl;
  }

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  S_msrWords tempoWords = elt->getTempoWords ();
  
  msrDottedDuration tempoBeatUnit  = elt->getTempoBeatUnit ();
  string            tempoPerMinute = elt->getTempoPerMinute ();

  msrTempo::msrTempoParenthesizedKind
    tempoParenthesizedKind =
      elt->getTempoParenthesizedKind ();
      
  fLilypondCodeIOstream <<
    endl;

  switch (elt->getTempoPlacementKind ()) {
    case msrPlacementKind::kPlacementNone:
      break;
    case msrPlacementKind::kPlacementAbove:
      break;
    case msrPlacementKind::kPlacementBelow:
      fLilypondCodeIOstream <<
        "\\once\\override Score.MetronomeMark.direction = #DOWN";
      break;
    } // switch
  
  switch (elt->getTempoKind ()) {
    case msrTempo::k_NoTempoKind:
      break;

    case msrTempo::kTempoBeatUnitsPerMinute:
      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeIOstream <<
            "\\tempo";
        
          if (tempoWords) {
            fLilypondCodeIOstream <<
              " \"" << tempoWords->getWordsContents () << "\"";
          }
    
          fLilypondCodeIOstream <<
            " " <<
            "\\markup {" <<
            endl;
    
          gIndenter++;
    
          fLilypondCodeIOstream <<
            "\\concat {" <<
            endl;
    
          gIndenter++;
    
          fLilypondCodeIOstream <<
            "(" <<
            endl;
            
          gIndenter++;
          
          fLilypondCodeIOstream <<
            "\\smaller \\general-align #Y #DOWN \\note #\"" <<
            dottedDurationAsLilypondStringWithoutBackSlash (
              inputLineNumber,
              tempoBeatUnit) <<
            "\" #UP" <<
            endl <<
            "=" <<
            endl <<
            tempoPerMinute <<
            endl;
            
          gIndenter--;
    
          fLilypondCodeIOstream <<
            ")" <<
            endl;
            
          gIndenter--;
    
          fLilypondCodeIOstream <<
            "}" <<
            endl;
            
          gIndenter--;
            
          fLilypondCodeIOstream <<
            "}" <<
            endl;
          break;

        case msrTempo::kTempoParenthesizedNo:
          fLilypondCodeIOstream <<
            "\\tempo";
        
          if (tempoWords) {
            fLilypondCodeIOstream <<
              " \"" << tempoWords->getWordsContents () << "\"";
          }
    
          fLilypondCodeIOstream <<
            " " <<
            dottedDurationAsLilypondString (
              inputLineNumber,
              tempoBeatUnit) <<
            " = " <<
            tempoPerMinute;

          fLilypondCodeIOstream <<
            endl;
          break;
      } // switch
      break;

    case msrTempo::kTempoBeatUnitsEquivalence:
      fLilypondCodeIOstream <<
        "\\tempo";
    
      if (tempoWords) {
        fLilypondCodeIOstream <<
          " \"" << tempoWords->getWordsContents () << "\"";
      }

      fLilypondCodeIOstream <<
        " " <<
        "\\markup {" <<
        endl;

      gIndenter++;

      fLilypondCodeIOstream <<
        "\\concat {" <<
        endl;

      gIndenter++;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeIOstream <<
            "(" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch
        
      gIndenter++;
      
      fLilypondCodeIOstream <<
        "\\smaller \\general-align #Y #DOWN \\note #\"" <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          tempoBeatUnit) <<
        "\" #UP" <<
        endl <<
        "=" <<
        endl <<
        "\\smaller \\general-align #Y #DOWN \\note #\"" <<
        dottedDurationAsLilypondStringWithoutBackSlash (
          inputLineNumber,
          elt->getTempoEquivalentBeatUnit ()) <<
        "\" #UP" <<
        endl;
        
      gIndenter--;

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeIOstream <<
            ")" <<
            endl;
          break;
        case msrTempo::kTempoParenthesizedNo:
          break;
      } // switch
        
      gIndenter--;

      fLilypondCodeIOstream <<
        "}" <<
        endl;
        
      gIndenter--;
        
      fLilypondCodeIOstream <<
        "}" <<
        endl;
        
      break;
    case msrTempo::kTempoNotesRelationShip:
      fLilypondCodeIOstream <<
        "\\tempoRelationship #\"";

      if (tempoWords) {
        fLilypondCodeIOstream <<
          tempoWords->getWordsContents ();
      }

      fLilypondCodeIOstream <<
        "\"";

      switch (tempoParenthesizedKind) {
        case msrTempo::kTempoParenthesizedYes:
          fLilypondCodeIOstream <<
            " ##t";
          break;
        case msrTempo::kTempoParenthesizedNo:
          fLilypondCodeIOstream <<
            " ##f";
          break;
      } // switch
        
      fLilypondCodeIOstream <<
        endl;
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitStart (S_msrTempoRelationshipElements& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTempoRelationshipElements" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\fixed b' {" <<
    endl;

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTempoRelationshipElements& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTempoRelationshipElements" <<
      endl;
  }

  gIndenter--;

  fLilypondCodeIOstream <<
    endl <<
    "}" <<
    endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrTempoNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTempoNote" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "b" <<
    wholeNotesAsLilypondString (
      elt->getInputLineNumber (),
      elt->getTempoNoteWholeNotes ()) <<
      " ";
}

void lpsr2LilypondTranslator::visitStart (S_msrTempoTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTempoTuplet" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\tuplet " <<
    elt->getTempoTupletActualNotes () <<
    "/" <<
    elt->getTempoTupletNormalNotes() << " { ";
}

void lpsr2LilypondTranslator::visitEnd (S_msrTempoTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTempoTuplet" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "}";
}

void lpsr2LilypondTranslator::visitEnd (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTempo" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrArticulation" <<
      endl;
  }

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrArticulation" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrFermata& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrFermata" <<
      endl;
  }

/*
Articulations can be attached to rests as well as notes but they cannot be attached to multi-measure rests. A special predefined command, \fermataMarkup, is available for at- taching a fermata to a multi-measure rest (and only a multi-measure rest). This creates a MultiMeasureRestText object.
*/

/* JMI
  switch (elt->getArticulationPlacement ()) {
    case kPlacementNone:
      // nothing needed
      break;
    case kPlacementAbove:
      fLilypondCodeIOstream << "^";
      break;
    case kPlacementBelow:
      fLilypondCodeIOstream << "_";
      break;
  } // switch
*/
      
  switch (elt->getFermataTypeKind ()) {
    case msrFermata::kFermataTypeNone:
      // no placement needed
      break;
    case msrFermata::kFermataTypeUpright:
      // no placement needed
      break;
    case msrFermata::kFermataTypeInverted:
      fLilypondCodeIOstream << "_";
      break;
  } // switch

  switch (elt->getFermataKind ()) {
    case msrFermata::kNormalFermataKind:
      fLilypondCodeIOstream << "\\fermata ";
      break;
    case msrFermata::kAngledFermataKind:
      fLilypondCodeIOstream << "\\shortfermata ";
      break;
    case msrFermata::kSquareFermataKind:
      fLilypondCodeIOstream << "\\longfermata ";
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrFermata& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrFermata" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrArpeggiato& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrArpeggiato" <<
      endl;
  }

/* VIRER JMI
  */
}

void lpsr2LilypondTranslator::visitEnd (S_msrArpeggiato& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrArpeggiato" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrNonArpeggiato& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrNonArpeggiato" <<
      endl;
  }

/* VIRER JMI
  */
}

void lpsr2LilypondTranslator::visitEnd (S_msrNonArpeggiato& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrNonArpeggiato" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTechnical" <<
      endl;
  }

  // don't generate the technical here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTechnical" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTechnicalWithInteger" <<
      ", fOnGoingChord = " <<
      booleanAsString (fOnGoingChord) <<
      endl;
  }

  // don't generate the technicalWithInteger here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTechnicalWithInteger" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTechnicalWithString" <<
      endl;
  }

  // don't generate the technicalWithString here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTechnicalWithString" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrOrnament" <<
      endl;
  }

  // don't generate the ornament here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrOrnament" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrGlissando& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrGlissando" <<
      endl;
  }

  // don't generate the glissando here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrGlissando& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrGlissando" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSlide& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrSlide" <<
      endl;
  }

  // don't generate the slide here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrSlide& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrSlide" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrSingleTremolo" <<
      endl;
  }

  // don't generate the singleTremolo here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrSingleTremolo" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrDoubleTremolo" <<
      endl;
  }

  // get double tremolo number of repeats
  int numberOfRepeats =
    elt->getDoubleTremoloNumberOfRepeats ();

  if (gTraceOptions->fTraceTremolos) {
    fLilypondCodeIOstream <<
      "% visitStart (S_msrDoubleTremolo&)" <<
      endl;

    gIndenter++;
    
    fLilypondCodeIOstream <<
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
  
  fLilypondCodeIOstream <<
    "\\repeat tremolo " << numberOfRepeats << " {";

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrDoubleTremolo" <<
      endl;
  }

  gIndenter--;
  
  fLilypondCodeIOstream <<
    "}" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrDynamics" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrDynamics" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrOtherDynamics" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrOtherDynamics" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrWords" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrWords" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrSlur" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrSlur" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrLigature" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrLigature" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrWedge" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrWedge" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrGraceNotes" <<
      endl;
  }

  /*
    1. no slash, no slur: \grace
    2. slash and slur: \acciaccatura
    3. slash but no slur: \slashedGrace
    4. no slash but slur: \appoggiatura
  */
  
  if (elt->getGraceNotesIsSlashed ()) {
    if (elt->getGraceNotesIsTied ()) {
      fLilypondCodeIOstream <<
        "\\acciaccatura";
    }
    else {
      fLilypondCodeIOstream <<
        "\\slashedGrace";
    }
  }
  
  else {
    if (elt->getGraceNotesIsTied ()) {
      fLilypondCodeIOstream <<
        "\\appoggiatura";
    }
    else {
      fLilypondCodeIOstream <<
        "\\grace";
    }
  }

  fLilypondCodeIOstream << " { ";

  // force durations to be displayed explicitly
  // at the beginning of the grace notes
  fLastMetWholeNotes = rational (0, 1);
}

void lpsr2LilypondTranslator::visitEnd (S_msrGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrGraceNotes" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "} ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrAfterGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrAfterGraceNotes" <<
      endl;
  }

 // JMI exists? if (elt->getGraceNotesIsSlashed ())
  fLilypondCodeIOstream <<
    "\\afterGrace { ";
}

void lpsr2LilypondTranslator::visitStart (S_msrAfterGraceNotesContents& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrAfterGraceNotesContents" <<
      endl;
  }

  // write a first closing right bracket right now
  fLilypondCodeIOstream <<
    "} { ";

  // force durations to be displayed explicitly
  // at the beginning of the after grace notes contents
  fLastMetWholeNotes = rational (0, 1);
}

void lpsr2LilypondTranslator::visitEnd (S_msrAfterGraceNotesContents& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrAfterGraceNotesContents" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "} ";
}

void lpsr2LilypondTranslator::visitEnd (S_msrAfterGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrAfterGraceNotes" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting " <<
      msrNote::noteKindAsString (elt->getNoteKind ()) <<
      " note" <<
      endl;
  }

  // is this a multiple rest to be ignored?
  switch (elt->getNoteKind ()) {
    case msrNote::kRestNote:
      // don't handle multiple rests, that's done in visitEnd (S_msrMultipleRest&)
      if (fOnGoingMultipleRestMeasures) {
        if (elt->getNoteOccupiesAFullMeasure ()) {
          bool inhibitMultipleRestMeasuresBrowsing =
            fVisitedLpsrScore->
              getMsrScore ()->
                getInhibitMultipleRestMeasuresBrowsing ();
        
          if (inhibitMultipleRestMeasuresBrowsing) {
            if (gMsrOptions->fTraceMsrVisitors || gTraceOptions->fTraceRepeats) {
              gLogIOstream <<
                "% ==> visiting multiple rest measure is ignored" <<
                endl;
            }

          gLogIOstream <<
            "% ==> returning from visitStart (S_msrNote&)" <<
            endl;
  
          return;
          }
        }
      }
      break;
    default:
      ;
  } // switch
    
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
        case msrWedge::k_NoWedgeKind:
          break;
          
        case msrWedge::kCrescendoWedge:
          switch (wedge->getWedgeNienteKind ()) {
            case msrWedge::kWedgeNienteYes:
              fLilypondCodeIOstream <<
                endl <<
                "\\once\\override Hairpin.circled-tip = ##t " <<
                endl;
              break;
            case msrWedge::kWedgeNienteNo:
              break;
            } // switch
          break;

        case msrWedge::kDecrescendoWedge:
          switch (wedge->getWedgeNienteKind ()) {
            case msrWedge::kWedgeNienteYes:
              fLilypondCodeIOstream <<
                endl <<
                "\\once\\override Hairpin.circled-tip = ##t " <<
                endl;
              break;
            case msrWedge::kWedgeNienteNo:
              break;
            } // switch
          break;
          
        case msrWedge::kStopWedge:
        /* JMI
          fLilypondCodeIOstream <<
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
        i++) {
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
                fLilypondCodeIOstream <<
                  "\\once\\slurSolid ";
                */
                break;
              case kLineTypeDashed:
                fLilypondCodeIOstream <<
                  "\\once\\slurDashed ";
                break;
              case kLineTypeDotted:
                fLilypondCodeIOstream <<
                  "\\once\\slurDotted ";
                break;
              case kLineTypeWavy:
                fLilypondCodeIOstream <<
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
        case msrGlissando::k_NoGlissandoType:
          break;
          
        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando style
          switch (glissando->getGlissandoLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeIOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fLilypondCodeIOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fLilypondCodeIOstream <<
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
        case msrSlide::k_NoSlideType:
          break;
          
        case msrSlide::kSlideTypeStart:
          // generate the glissando style
          switch (slide->getSlideLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeIOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dashed-line" <<
                endl;
              break;
            case kLineTypeDotted:
              fLilypondCodeIOstream <<
                endl <<
                "\\once\\override Glissando.style = #'dotted-line" <<
                endl;
              break;
            case kLineTypeWavy:
              fLilypondCodeIOstream <<
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
        case msrSpanner::kSpannerTrill:
          break;
        case msrSpanner::kSpannerDashes:
          break;
        case msrSpanner::kSpannerWavyLine:
          if (spanner->getSpannerNoteUplink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            doGenerateSpannerCode = false;
          }
          break;
      } // switch

  //    doGenerateSpannerCode = true;
      
      fLilypondCodeIOstream <<
        "% doGenerateSpannerCode = " <<
        booleanAsString (doGenerateSpannerCode) <<
        endl;

      if (doGenerateSpannerCode) {
        switch (spanner->getSpannerKind ()) {
          case msrSpanner::kSpannerTrill:
            break;
          case msrSpanner::kSpannerDashes:
            fLilypondCodeIOstream <<
              "\\override TextSpanner.bound-details.left.text = \\markup { " " }" <<
              endl;
            break;
          case msrSpanner::kSpannerWavyLine:
            break;
        } // switch
  
        fLilypondCodeIOstream <<
          spannerAsLilypondString (spanner) << " ";
      }
    } // for
  }
  
  // should the note be parenthesized?
  msrNote::msrNoteHeadParenthesesKind
    noteHeadParenthesesKind =
      elt->getNoteHeadParenthesesKind ();

  switch (noteHeadParenthesesKind) {
    case msrNote::kNoteHeadParenthesesYes:
      fLilypondCodeIOstream << "\\parenthesize ";
      break;
    case msrNote::kNoteHeadParenthesesNo:
      break;
  } // switch

  // handling note head
  msrNote::msrNoteHeadKind
    noteHeadKind =
      elt->getNoteHeadKind ();
 
  switch (noteHeadKind) {
    case msrNote::kNoteHeadSlash:
      fLilypondCodeIOstream << "\\tweak style #'slash ";
      break;
    case msrNote::kNoteHeadTriangle:
      fLilypondCodeIOstream << "\\tweak style #'triangle ";
      break;
    case msrNote::kNoteHeadDiamond:
      fLilypondCodeIOstream << "\\tweak style #'diamond ";
      break;
    case msrNote::kNoteHeadSquare:
      fLilypondCodeIOstream << "\\tweak style #'la ";
      break;
    case msrNote::kNoteHeadCross:
      fLilypondCodeIOstream << "\\tweak style #'cross ";
      break;
    case msrNote::kNoteHeadX:
      fLilypondCodeIOstream << "\\tweak style #'cross ";
      break;
    case msrNote::kNoteHeadCircleX:
      fLilypondCodeIOstream << "\\tweak style #'xcircle ";
      break;
    case msrNote::kNoteHeadInvertedTriangle:
      fLilypondCodeIOstream << "%{kNoteHeadInvertedTriangle%} ";
      break;
    case msrNote::kNoteHeadArrowDown:
      fLilypondCodeIOstream << "%{kNoteHeadArrowDown%} ";
      break;
    case msrNote::kNoteHeadArrowUp:
      fLilypondCodeIOstream << "%{kNoteHeadArrowUp%} ";
      break;
    case msrNote::kNoteHeadSlashed:
      fLilypondCodeIOstream << "%{kNoteHeadSlashed%} ";
      break;
    case msrNote::kNoteHeadBackSlashed:
      fLilypondCodeIOstream << "%{kNoteHeadBackSlashed%} ";
      break;
    case msrNote::kNoteHeadNormal:
 // JMI     fLilypondCodeIOstream << "%{kNoteHeadNormal%} ";
      break;
    case msrNote::kNoteHeadCluster:
      fLilypondCodeIOstream << "%{kNoteHeadCluster%} ";
      break;
    case msrNote::kNoteHeadCircleDot:
      fLilypondCodeIOstream << "%{kNoteHeadCircleDot%} ";
      break;
    case msrNote::kNoteHeadLeftTriangle:
      fLilypondCodeIOstream << "%{kNoteHeadLeftTriangle%} ";
      break;
    case msrNote::kNoteHeadRectangle:
      fLilypondCodeIOstream << "%{kNoteHeadRectangle%} ";
      break;
    case msrNote::kNoteHeadNone:
      fLilypondCodeIOstream << "\\once\\omit NoteHead ";
      break;
    case msrNote::kNoteHeadDo:
      fLilypondCodeIOstream << "\\tweak style #'do ";
      break;
    case msrNote::kNoteHeadRe:
      fLilypondCodeIOstream << "\\tweak style #'re ";
      break;
    case msrNote::kNoteHeadMi:
      fLilypondCodeIOstream << "\\tweak style #'mi ";
      break;
    case msrNote::kNoteHeadFa:
      fLilypondCodeIOstream << "\\tweak style #'fa ";
      break;
    case msrNote::kNoteHeadFaUp:
      fLilypondCodeIOstream << "\\tweak style #'triangle ";
      break;
    case msrNote::kNoteHeadSo:
      fLilypondCodeIOstream << "\\tweak style #'sol ";
      break;
    case msrNote::kNoteHeadLa:
      fLilypondCodeIOstream << "\\tweak style #'la ";
      break;
    case msrNote::kNoteHeadTi:
      fLilypondCodeIOstream << "\\tweak style #'ti ";
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
                  
                fLilypondCodeIOstream <<
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
                  
                fLilypondCodeIOstream <<
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
    fLilypondCodeIOstream <<
      "\\once \\override NoteHead.font-size = -3 ";
  }

  // print the note as a LilyPond string
  printNoteAsLilypondString (elt);

  if (gLilypondOptions->fNoteInputLineNumbers) {
    // print the note line number as a comment
    fLilypondCodeIOstream <<
      "%{ " << elt->getInputLineNumber () << " %} ";
  }
  
  fOnGoingNote = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting " <<
      msrNote::noteKindAsString (elt->getNoteKind ()) <<
      " note" <<
      endl;
  }

  // is this a multiple rest to be ignored?
  switch (elt->getNoteKind ()) {
    case msrNote::kRestNote:
      // don't handle multiple rests, that's done in visitEnd (S_msrMultipleRest&)
      if (fOnGoingMultipleRestMeasures) {
        if (elt->getNoteOccupiesAFullMeasure ()) {
          bool inhibitMultipleRestMeasuresBrowsing =
            fVisitedLpsrScore->
              getMsrScore ()->
                getInhibitMultipleRestMeasuresBrowsing ();
        
          if (inhibitMultipleRestMeasuresBrowsing) {
            if (gTraceOptions->fTraceNotes || gTraceOptions->fTraceRepeats) {
              gLogIOstream <<
                "% ==> visiting multiple rest measure is ignored" <<
                endl;
            }
  
          gLogIOstream <<
            "% ==> returning from visitEnd (S_msrNote&)" <<
            endl;
            
          return;
          }
        }
      }
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
      fLilypondCodeIOstream <<
        singleTremoloDurationAsLilypondString (
          noteSingleTremolo);
    }
  }

/* JMI
  // get note stem kind 
  msrStem::msrStemKind
    stemKind = // JMI
      fCurrentStem
        ? fCurrentStem->getStemKind ()
        : msrStem::k_NoStem;
*/

/* JMI
  // has the stem been omitted?
  if (elt->getNoteIsStemless ()) {
    fLilypondCodeIOstream <<
      endl;
  }
  */
  
  // print the note articulations if any
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
        case msrArticulation::kFermata:
          // this is handled in visitStart (S_msrFermata&)
          break;
          
        default:
          writeNoteArticulationAsLilyponString ((*i));
          fLilypondCodeIOstream <<
            " ";
      } // switch
    } // for
  }

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
        
      fLilypondCodeIOstream <<
        technicalAsLilypondString ((*i));

      switch ((*i)->getTechnicalPlacementKind ()) {
        case kPlacementNone:
          break;
        case kPlacementAbove:
          fLilypondCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fLilypondCodeIOstream << "_";
          break;
      } // switch

      fLilypondCodeIOstream << " ";
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
      
            fLilypondCodeIOstream <<
              technicalWithIntegerAsLilypondString (
                technicalWithInteger);
      
            switch (technicalWithInteger->getTechnicalWithIntegerPlacementKind ()) {
              case kPlacementNone:
                break;
              case kPlacementAbove:
                fLilypondCodeIOstream << "^";
                break;
              case kPlacementBelow:
                fLilypondCodeIOstream << "_";
                break;
            } // switch
      
            fLilypondCodeIOstream << " ";
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
        
      fLilypondCodeIOstream <<
        technicalWithStringAsLilypondString ((*i));

      switch ((*i)->getTechnicalWithStringPlacementKind ()) {
        case kPlacementNone:
          break;
        case kPlacementAbove:
          fLilypondCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fLilypondCodeIOstream << "_";
          break;
      } // switch

      fLilypondCodeIOstream << " ";
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
      fLilypondCodeIOstream <<
        ornamentAsLilypondString ((*i)) << // some ornaments are not yet supported
        " ";
    } // for
  }

  // print the note dynamics if any
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
   // JMI       fLilypondCodeIOstream << "-3";
          break;
        case kPlacementAbove:
          fLilypondCodeIOstream << "^";
          break;
        case kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fLilypondCodeIOstream <<
        dynamicsAsLilypondString (dynamics) << " ";
    } // for
  }
  
  // print the note other dynamics if any
  const list<S_msrOtherDynamics>&
    noteOtherDynamics =
      elt->getNoteOtherDynamics ();
      
  if (noteOtherDynamics.size ()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (
      i=noteOtherDynamics.begin ();
      i!=noteOtherDynamics.end ();
      i++) {
      fLilypondCodeIOstream <<
        "-\\markup { "
        "\\dynamic \"" << (*i)->getOtherDynamicsString () << "\" } ";
    } // for
  }

  // print the note words if any
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
            s <<
              "%{ " <<
              wordsFontSize->getFontNumericSize () <<
              " points %} ";
            break;
        } // switch

        s <<
 // JMI         quoteStringIfNonAlpha (wordsContents) <<
          "\"" << wordsContents << "\"" <<
          " } ";

        markup = s.str ();
        }

      fLilypondCodeIOstream <<
        markup;
    } // for
  }

  // print the note slurs if any,
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

        /*
        \slurDashed, \slurDotted, \slurHalfDashed,
        \slurHalfSolid, \slurDashPattern, \slurSolid
        */
        
        switch (slur->getSlurTypeKind ()) {
          case msrSlur::k_NoSlur:
            break;
          case msrSlur::kRegularSlurStart:
            fLilypondCodeIOstream << "( ";
            break;
          case msrSlur::kPhrasingSlurStart:
            fLilypondCodeIOstream << "\\( ";
            break;
          case msrSlur::kSlurContinue:
            break;
          case msrSlur::kRegularSlurStop:
            fLilypondCodeIOstream << ") ";
            break;
          case msrSlur::kPhrasingSlurStop:
            fLilypondCodeIOstream << "\\) ";
            break;
        } // switch
      } // for
    }
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
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kLigatureStart:
   // JMI       fLilypondCodeIOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
          fLilypondCodeIOstream << "\\] ";
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
        case msrWedge::k_NoWedgeKind:
          break;
          
        case msrWedge::kCrescendoWedge:
          switch (wedge->getWedgePlacementKind ()) {
            case kPlacementNone:
              break;
            case kPlacementAbove:
              fLilypondCodeIOstream <<
                "^";
              break;
            case kPlacementBelow:
              fLilypondCodeIOstream <<
                "_";
              break;
            } // switch
          fLilypondCodeIOstream <<
            "\\< ";
          break;

        case msrWedge::kDecrescendoWedge:
          switch (wedge->getWedgePlacementKind ()) {
            case kPlacementNone:
              break;
            case kPlacementAbove:
              fLilypondCodeIOstream <<
                "^";
              break;
            case kPlacementBelow:
              fLilypondCodeIOstream <<
                "_";
              break;
            } // switch
          fLilypondCodeIOstream <<
            "\\> ";
          break;
          
        case msrWedge::kStopWedge:
          fLilypondCodeIOstream <<
            "\\! ";
          break;
      } // switch
    } // for
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
        case msrGlissando::k_NoGlissandoType:
          break;
          
        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando itself
          fLilypondCodeIOstream <<
            "\\glissando ";
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
        case msrSlide::k_NoSlideType:
          break;
          
        case msrSlide::kSlideTypeStart:
          // generate the glissando itself
          fLilypondCodeIOstream <<
            "\\glissando ";
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
        case msrSpanner::kSpannerTrill:
          break;
        case msrSpanner::kSpannerDashes:
          break;
        case msrSpanner::kSpannerWavyLine:
          if (spanner->getSpannerNoteUplink ()->getNoteTrillOrnament ()) {
            // don't generate anything, the trill will display the wavy line
            doGenerateSpannerCode = false;
          }
          break;
      } // switch

   //   doGenerateSpannerCode = true;

      fLilypondCodeIOstream <<
        "% doGenerateSpannerCode = " <<
        booleanAsString (doGenerateSpannerCode) <<
        endl;

      if (doGenerateSpannerCode) {
        switch (spanner->getSpannerPlacementKind ()) {
          case kPlacementNone:
     // JMI       fLilypondCodeIOstream << "-3";
            break;
          case kPlacementAbove:
            fLilypondCodeIOstream << "^";
            break;
          case kPlacementBelow:
            // this is done by LilyPond by default
            break;
        } // switch
  
        fLilypondCodeIOstream <<
          spannerAsLilypondString (spanner) << " ";
      }
    } // for
  }
  
  if (false && elt->getNoteIsFollowedByGraceNotes ()) { // JMI
    if (! elt->getNoteIsARest ()) {
      fLilypondCodeIOstream <<
       " { % NoteIsFollowedByGraceNotes" <<
        endl; // JMI ???
    }
  }

  fOnGoingNote = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrOctaveShift" <<
      endl;
  }

  int octaveShiftSize =
    elt->getOctaveShiftSize (); // 8 or 15
    
  fLilypondCodeIOstream <<
    endl <<
    "\\ottava #";
    
  switch (elt->getOctaveShiftKind ()) {
    case msrOctaveShift::k_NoOctaveShift:
      break;
    case msrOctaveShift::kOctaveShiftUp:
      fLilypondCodeIOstream <<
        "-" << (octaveShiftSize - 1) / 7; // 1 or 2
      break;
    case msrOctaveShift::kOctaveShiftDown:
      fLilypondCodeIOstream <<
        (octaveShiftSize - 1) / 7; // 1 or 2
      break;
    case msrOctaveShift::kOctaveShiftStop:
      fLilypondCodeIOstream <<
        0;
      break;
    case msrOctaveShift::kOctaveShiftContinue:
     // JMI ??? fLilypondCodeIOstream << 0;
      break;
  } // switch
  
  fLilypondCodeIOstream <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrOctaveShift" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrAccordionRegistration& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrAccordionRegistration" <<
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
    if (nonZeroNumberHasBeenIssued)
      numbersToBeUsed +=
        to_string (middleDotsNumber);
  }
  
  numbersToBeUsed +=
    to_string (lowDotsNumber);
  
  fLilypondCodeIOstream <<
    "\\discant \"" <<
    numbersToBeUsed <<
    "\" ";
}

void lpsr2LilypondTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrHarpPedalsTuning" <<
      endl;
  }

  map<msrDiatonicPitchKind, msrAlterationKind>
    harpPedalsAlterationKindsMap =
      elt->getHarpPedalsAlterationKindsMap ();
      
  if (harpPedalsAlterationKindsMap.size ()) {
    gIndenter++;

    fLilypondCodeIOstream <<
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
      "\" } ";
  }
  else {
    fLilypondCodeIOstream <<
      "%{empty harp pedals tuning???%} "; // JMI
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrStem" <<
      endl;
  }

  fCurrentStem = elt;
}

void lpsr2LilypondTranslator::visitEnd (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrStem" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBeam& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrBeam" <<
      endl;
  }

  // LilyPond will take care of multiple beams automatically,
  // so we need only generate code for the first number (level)
  switch (elt->getBeamKind ()) {
    
    case msrBeam::kBeginBeam:
      if (elt->getBeamNumber () == 1)
        fLilypondCodeIOstream << "[ ";
      break;
      
    case msrBeam::kContinueBeam:
      break;
      
    case msrBeam::kEndBeam:
      if (elt->getBeamNumber () == 1)
        fLilypondCodeIOstream << "] ";
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
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrBeam" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrChord" <<
      endl;
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
        case msrGlissando::k_NoGlissandoType:
          break;
          
        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando style
          switch (glissando->getGlissandoLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeIOstream <<
                "\\once\\override Glissando.style = #'dashed-line ";
              break;
            case kLineTypeDotted:
              fLilypondCodeIOstream <<
                "\\once\\override Glissando.style = #'dotted-line ";
              break;
            case kLineTypeWavy:
              fLilypondCodeIOstream <<
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
        case msrSlide::k_NoSlideType:
          break;
          
        case msrSlide::kSlideTypeStart:
          // generate the glissando style
          switch (slide->getSlideLineTypeKind ()) {
            case kLineTypeSolid:
              break;
            case kLineTypeDashed:
              fLilypondCodeIOstream <<
                "\\once\\override Glissando.style = #'dashed-line ";
              break;
            case kLineTypeDotted:
              fLilypondCodeIOstream <<
                "\\once\\override Glissando.style = #'dotted-line ";
              break;
            case kLineTypeWavy:
              fLilypondCodeIOstream <<
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
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kLigatureStart:
          fLilypondCodeIOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
  // JMI        fLilypondCodeIOstream << "\\] ";
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
            fLilypondCodeIOstream << "\\arpeggioNormal";
            break;
          case kDirectionUp:
            fLilypondCodeIOstream << "\\arpeggioArrowUp";
            break;
          case kDirectionDown:
            fLilypondCodeIOstream << "\\arpeggioArrowDown";
            break;
        } // switch
          
        fLilypondCodeIOstream << " ";

        fCurrentArpeggioDirectionKind = directionKind;
      }
      
      else if (
        // non arpeggiato?
        S_msrNonArpeggiato
          nonArpeggiato =
            dynamic_cast<msrNonArpeggiato*>(&(*articulation))
        ) {
        fLilypondCodeIOstream << "\\arpeggioBracket";
        
        switch (nonArpeggiato->getNonArpeggiatoTypeKind ()) {
          case msrNonArpeggiato::kNonArpeggiatoTypeNone:
            fLilypondCodeIOstream << " %{\\kNonArpeggiatoTypeNone???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeTop:
            fLilypondCodeIOstream << " %{\\kNonArpeggiatoTypeTop???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeBottom:
            fLilypondCodeIOstream << " %{\\kNonArpeggiatoTypeBottom???%}";
            break;
        } // switch
          
        fLilypondCodeIOstream << " ";
      }
   } // for
  }

  fLilypondCodeIOstream <<
    "<";

  fOnGoingChord = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrChord" <<
      endl;
  }

  int chordInputLineNumber =
    elt->getInputLineNumber ();
    
  fLilypondCodeIOstream <<
    ">";

  if (
    elt->getChordIsFirstChordInADoubleTremolo ()
      ||
    elt->getChordIsSecondChordInADoubleTremolo ()) {
      // print chord note duration
      fLilypondCodeIOstream <<
        elt->getChordSoundingWholeNotes ();
  }
  
  else {
    // print the chord duration
    fLilypondCodeIOstream <<
      durationAsLilypondString (
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

      fLilypondCodeIOstream <<
        "\\" <<
        stringNumber;
        
      if (++i == iEnd) break;
      fLilypondCodeIOstream <<
        " ";
    } // for
    fLilypondCodeIOstream <<
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
    fLilypondCodeIOstream <<
      singleTremoloDurationAsLilypondString (
        chordSingleTremolo);
  }

  fLilypondCodeIOstream <<
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
      writeChordArticulationAsLilyponString ((*i));
             
      fLilypondCodeIOstream <<
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
      fLilypondCodeIOstream <<
        technicalAsLilypondString ((*i)) <<
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
      fLilypondCodeIOstream <<
        technicalWithIntegerAsLilypondString ((*i)) <<
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
      fLilypondCodeIOstream <<
        technicalWithStringAsLilypondString ((*i)) <<
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
      fLilypondCodeIOstream <<
        ornamentAsLilypondString ((*i)) << // some ornaments are not yet supported
        " ";
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
          fLilypondCodeIOstream << "-";
          break;
        case kPlacementAbove:
          fLilypondCodeIOstream << "^";
          break;
        case kPlacementBelow:
          // this is done by LilyPond by default
          break;
      } // switch

      fLilypondCodeIOstream <<
        dynamicsAsLilypondString (dynamics) << " ";
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
      fLilypondCodeIOstream <<
        "\\" << (*i)->asString () << " ";
    } // for
  }

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
      fLilypondCodeIOstream <<
        "] ";
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
          fLilypondCodeIOstream << "^";
          break;
        case kPlacementBelow:
          fLilypondCodeIOstream << "_";
          break;
      } // switch
    
      fLilypondCodeIOstream <<
        "\\markup" << " { " <<
        quoteStringIfNonAlpha (wordsContents) <<
        " } ";
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
          fLilypondCodeIOstream << "( ";
          break;
        case msrSlur::kPhrasingSlurStart:
          fLilypondCodeIOstream << "\\( ";
          break;
        case msrSlur::kSlurContinue:
          break;
        case msrSlur::kRegularSlurStop:
          fLilypondCodeIOstream << ") ";
          break;
        case msrSlur::kPhrasingSlurStop:
          fLilypondCodeIOstream << "\\) ";
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
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kLigatureStart:
  // JMI        fLilypondCodeIOstream << "\\[ ";
          break;
        case msrLigature::kLigatureContinue:
          break;
        case msrLigature::kLigatureStop:
          fLilypondCodeIOstream << "\\] ";
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
        case msrWedge::k_NoWedgeKind:
          break;
        case msrWedge::kCrescendoWedge:
          fLilypondCodeIOstream << "\\< ";
          break;
        case msrWedge::kDecrescendoWedge:
          fLilypondCodeIOstream << "\\> ";
          break;
        case msrWedge::kStopWedge:
          fLilypondCodeIOstream << "\\! ";
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
        case msrGlissando::k_NoGlissandoType:
          break;
          
        case msrGlissando::kGlissandoTypeStart:
          // generate the glissando itself
          fLilypondCodeIOstream <<
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
        case msrSlide::k_NoSlideType:
          break;
          
        case msrSlide::kSlideTypeStart:
          // generate the glissando itself
          fLilypondCodeIOstream <<
            "\\glissando ";
          break;
          
        case msrSlide::kSlideTypeStop:
          break;
      } // switch
    } // for
  }

/*
  // print the tie if any
  {
    S_msrTie chordTie = elt->getChordTie ();
  
    if (chordTie) {
      if (chordTie->getTieKind () == msrTie::kTieStart) {
        fLilypondCodeIOstream << "~ ";
      }
    }
  }
*/

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  fRelativeOctaveReference =
    elt->getChordNotes () [0];

  fOnGoingChord = false; 
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTuplet" <<
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
          getTupletActualNotes (),
        containingTuplet->
          getTupletNormalNotes ());
  }

  if (gLilypondOptions->fIndentTuplets) {
    fLilypondCodeIOstream <<
      endl;
  }

  // get bracket kind
  msrTuplet::msrTupletBracketKind
    tupletBracketKind =
      elt->getTupletBracketKind ();

  switch (tupletBracketKind) {
    case msrTuplet::kTupletBracketYes:
    /* JMI
      fLilypondCodeIOstream <<
        "%{kTupletBracketYes%}" <<
        endl;
        */
      break;
    case msrTuplet::kTupletBracketNo:
      fLilypondCodeIOstream <<
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
      fLilypondCodeIOstream <<
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
      fLilypondCodeIOstream <<
        "%{tupletShowNumberActual%}" <<
        endl;
        */
      break;
    case msrTuplet::kTupletShowNumberBoth:
      fLilypondCodeIOstream <<
        "\\once \\override TupletNumber.text = #tuplet-number::calc-fraction-text" <<
        endl;
      break;
    case msrTuplet::kTupletShowNumberNone:
      fLilypondCodeIOstream <<
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
      fLilypondCodeIOstream <<
     // JMI ???   "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-fraction-text \"" <<
        "\\once \\override TupletNumber.text = #(tuplet-number::append-note-wrapper tuplet-number::calc-denominator-text \"" <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        "\")" <<
        endl;
      break;
    case msrTuplet::kTupletShowTypeBoth:
      fLilypondCodeIOstream <<
        "\\once \\override TupletNumber.text = #(tuplet-number::fraction-with-notes \"" <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        "\" \"" <<
        wholeNotesAsLilypondString (
          inputLineNumber,
          memberNoteDisplayWholeNotes) <<
        "\")" <<
        endl;
      break;
    case msrTuplet::kTupletShowTypeNone:
    /* JMI
      fLilypondCodeIOstream <<
        "%{tupletShowTypeNone%}" <<
        endl;
        */
      break;
  } // switch

  fLilypondCodeIOstream <<
    "\\tuplet " <<
    elt->getTupletActualNotes () <<
    "/" <<
    elt->getTupletNormalNotes() << " { ";

  fTupletsStack.push (elt);

  gIndenter++;

  // force durations to be displayed explicitly
  // at the beginning of the tuplet
  fLastMetWholeNotes = rational (0, 1);
}

void lpsr2LilypondTranslator::visitEnd (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTuplet" <<
      endl;
  }

  gIndenter--;

  if (gLilypondOptions->fIndentTuplets) {
    fLilypondCodeIOstream <<
      endl;
  }
  
  fLilypondCodeIOstream <<
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
      fLilypondCodeIOstream <<
        "\\undo \\tupletsCurvedBrackets" <<
        endl;
      break;
  } // switch

  fTupletsStack.pop ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrTie" <<
      endl;
  }
  
  switch (elt->getTieKind ()) {
    case msrTie::k_NoTie:
      break;
    case msrTie::kTieStart:
 // JMI     fLilypondCodeIOstream << "~ "; // JMI
      break;
    case msrTie::kTieContinue:
      break;
    case msrTie::kTieStop:
      break;
   } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrTie" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSegno& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrSegno" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.segno\" }" <<
    endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrCoda& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrCoda" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.coda\" }" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrEyeGlasses& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting eyeGlasses" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "^\\markup {\\eyeglasses} ";
}

void lpsr2LilypondTranslator::visitStart (S_msrPedal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting pedal" <<
      endl;
  }
      
  fLilypondCodeIOstream <<
    endl;
    
  switch (elt->getPedalTypeKind ()) {
    case msrPedal::kPedalStart:
      fLilypondCodeIOstream <<
        "<>\\sustainOn";
      break;
    case msrPedal::kPedalContinue:
      fLilypondCodeIOstream <<
        "%{\\continue pedal???%}"; // JMI
      break;
    case msrPedal::kPedalChange:
      fLilypondCodeIOstream <<
        "%{\\change pedal???%}"; // JMI
      break;
    case msrPedal::kPedalStop:
      fLilypondCodeIOstream <<
        "<>\\sustainOff";
      break;
  } // switch

  fLilypondCodeIOstream <<
    endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrDamp& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting damp" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "^\\markup {\\damp} ";
}

void lpsr2LilypondTranslator::visitStart (S_msrDampAll& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting dampAll" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "^\\markup {\\dampAll} ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      endl <<
      "% --> Start visiting msrBarline" <<
      endl;
  }

  switch (elt->getBarlineCategory ()) {
    
    case msrBarline::kBarlineCategoryStandalone:
    /* JMI
      fLilypondCodeIOstream <<
        endl <<
        endl;
        */
      
      switch (elt->getBarlineStyleKind ()) {
        case msrBarline::k_NoBarlineStyle:
          break;
        case msrBarline::kBarlineStyleRegular:
          fLilypondCodeIOstream << "\\bar \"|\" ";
          break;
        case msrBarline::kBarlineStyleDotted:
          fLilypondCodeIOstream << "\\bar \";\" ";
          break;
        case msrBarline::kBarlineStyleDashed:
          fLilypondCodeIOstream << "\\bar \"!\" ";
          break;
        case msrBarline::kBarlineStyleHeavy:
          fLilypondCodeIOstream << "\\bar \".\" ";
          break;
        case msrBarline::kBarlineStyleLightLight:
          fLilypondCodeIOstream << "\\bar \"||\" ";
          break;
        case msrBarline::kBarlineStyleLightHeavy:
          fLilypondCodeIOstream <<
            endl <<
            "\\bar \"|.\" ";
          break;
        case msrBarline::kBarlineStyleHeavyLight:
          fLilypondCodeIOstream << "\\bar \".|\" ";
          break;
        case msrBarline::kBarlineStyleHeavyHeavy:
          fLilypondCodeIOstream << "\\bar \"..\" ";
          break;
        case msrBarline::kBarlineStyleTick:
          fLilypondCodeIOstream << "\\bar \"'\" ";
          break;
        case msrBarline::kBarlineStyleShort:
          // \bar "/" is the custom short barline
          fLilypondCodeIOstream << "\\bar \"/\" ";
          break;
        case msrBarline::kBarlineStyleNone:
          fLilypondCodeIOstream << "\\bar \"\" ";
          break;
      } // switch

      if (gLilypondOptions->fNoteInputLineNumbers) {
        // print the barline line number as a comment
        fLilypondCodeIOstream <<
          "%{ " << elt->getInputLineNumber () << " %} ";
      }

/* JMI
      fLilypondCodeIOstream <<
        endl <<
        endl;
*/

      switch (elt->getBarlineHasSegnoKind ()) {
        case msrBarline::kBarlineHasSegnoYes:
          fLilypondCodeIOstream <<
            "\\mark \\markup { \\musicglyph #\"scripts.segno\" } ";
          break;
        case msrBarline::kBarlineHasSegnoNo:
          break;
      } // switch

      switch (elt->getBarlineHasCodaKind ()) {
        case msrBarline::kBarlineHasCodaYes:
          fLilypondCodeIOstream <<
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
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      endl <<
      "% --> End visiting msrBarline" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrBarCheck" <<
      endl;
  }
      
  string nextBarNumber =
    elt->getNextBarNumber ();

  // don't generate a bar check before the end of measure 1 // JMI ???
  fLilypondCodeIOstream <<
    "| % " << nextBarNumber << " % bar check" <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrBarCheck" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarNumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrBarNumberCheck" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\barNumberCheck #" << elt->getNextBarNumber () <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrBarNumberCheck" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrLineBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrLineBreak" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\myBreak | % " << elt->getNextBarNumber () <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrLineBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrLineBreak" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPageBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrPageBreak" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "\\myPageBreak " <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrPageBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrPageBreak" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrRepeat" <<
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
  
  fLilypondCodeIOstream <<
    endl;

  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream << left <<
      setw (commentFieldWidth) <<
      s.str () << "% start of repeat" <<
      endl;
  }
  else {
    fLilypondCodeIOstream <<
      s.str () <<
      endl;
  }

  gIndenter++;

  int
    repeatTimes =
      elt->getRepeatTimes ();

  if (repeatTimes > 2) {
    fLilypondCodeIOstream <<
      "<>^\"" << repeatTimes << " times\"" << // JMI
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrRepeat" <<
      endl;
  }

  /*
    CAUTION:
      the end of the repeat has to be generated
      before the endings are handled
  */
  
  if (! fRepeatsDescrStack.back ()->getEndOfRepeatHasBeenGenerated ()) {
    // the end of the repeat has not been generated yet

    gIndenter--;

    if (gLilypondOptions->fComments) {      
      fLilypondCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fLilypondCodeIOstream <<
        "}" <<
        endl <<
        endl;
    }
  }

  fRepeatsDescrStack.pop_back ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrRepeatCommonPart" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrRepeatCommonPart" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeatEnding& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrRepeatEnding" <<
      endl;
  }

  fRepeatsDescrStack.back ()->incrementRepeatEndingsCounter ();
  
  if (elt->getRepeatEndingInternalNumber () == 1) {
    
    gIndenter--;
    
    // first repeat ending is in charge of
    // outputting the end of the repeat
    if (gLilypondOptions->fComments) {      
      fLilypondCodeIOstream <<
        setw (commentFieldWidth) << left <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fLilypondCodeIOstream <<
        "}" <<
        endl;
    }
    fRepeatsDescrStack.back ()->setEndOfRepeatHasBeenGenerated ();

    // first repeat ending is in charge of
    // outputting the start of the alternative
    if (gLilypondOptions->fComments) {      
      fLilypondCodeIOstream << left <<
        endl <<
        setw (commentFieldWidth) <<
        "\\alternative {" <<
        "% start of alternative" <<
        endl;
    }
    else {
      fLilypondCodeIOstream <<
        endl <<
        "\\alternative {" <<
        endl;
    }

    gIndenter++;
  }

  // output the start of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gLilypondOptions->fComments) {  
        fLilypondCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "{" << "% start of repeat hooked ending" <<
          endl;
      }
      else {
        fLilypondCodeIOstream <<
          "{" <<
          endl;
      }
      break;
      
    case msrRepeatEnding::kHooklessEnding:
      if (gLilypondOptions->fComments)    
        fLilypondCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "{" << "% start of repeat hookless ending" <<
          endl;
      else
        fLilypondCodeIOstream <<
          "{" <<
          endl;
      break;
  } // switch

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrRepeatEnding" <<
      endl;
  }

  gIndenter--;

  // output the end of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gLilypondOptions->fComments) {
        fLilypondCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "}" << "% end of repeat hooked ending" <<
          endl;
      }
      else {
        fLilypondCodeIOstream <<
          "}" <<
          endl;
      }
      break;
      
    case msrRepeatEnding::kHooklessEnding:
      if (gLilypondOptions->fComments)   { 
        fLilypondCodeIOstream << left <<
          setw (commentFieldWidth) <<
          "}" << "% end of repeat hookless ending" <<
          endl;
      }
      else {
        fLilypondCodeIOstream <<
          "}" <<
          endl;
      }
      break;
  } // switch

  if (gTraceOptions->fTraceRepeats) {
    fLilypondCodeIOstream <<
      "% ===**** fRepeatsDescrStack.back () = '" <<
      fRepeatsDescrStack.back ()->repeatDescrAsString () <<
      "'" <<
      endl;
  }
  
  if (
    fRepeatsDescrStack.back ()->getRepeatEndingsCounter ()
      ==
    fRepeatsDescrStack.back ()->getRepeatEndingsNumber ()) {
      
    gIndenter--;
    
    // last repeat ending is in charge of
    // outputting the end of the alternative
    if (gLilypondOptions->fComments)   { 
      fLilypondCodeIOstream << left <<
        setw (commentFieldWidth) <<
        "}" << "% end of alternative" <<
        endl;
    }
    else {
      fLilypondCodeIOstream <<
        "}" <<
        endl <<
        endl;
    }
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrComment" <<
      endl;
  }

  fLilypondCodeIOstream <<
    "% " << elt->getContents () <<
    endl;
    
  if (elt->getCommentGapKind () == lpsrComment::kGapAfterwards)
    fLilypondCodeIOstream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrComment" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrSchemeFunction& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting lpsrSchemeFunction" <<
      endl;
  }

  fLilypondCodeIOstream <<
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
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting lpsrSchemeFunction" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrRehearsal" <<
      endl;
  }

  fLilypondCodeIOstream <<
    endl;

  switch (elt->getRehearsalPlacementKind ()) {
    case msrPlacementKind::kPlacementNone:
      break;
    case msrPlacementKind::kPlacementAbove:
      break;
    case msrPlacementKind::kPlacementBelow:
      fLilypondCodeIOstream <<
        "\\once\\override Score.RehearsalMark.direction = #DOWN";
      break;
    } // switch
  
  fLilypondCodeIOstream <<
    endl <<
    "\\mark\\markup { ";

  switch (elt->getRehearsalKind ()) {
    case msrRehearsal::kNone:
      fLilypondCodeIOstream <<
        "\\box"; // default value
      break;
    case msrRehearsal::kRectangle:
      fLilypondCodeIOstream <<
        "\\box";
      break;
    case msrRehearsal::kOval:
      fLilypondCodeIOstream <<
        "\\oval";
      break;
    case msrRehearsal::kCircle:
      fLilypondCodeIOstream <<
        "\\circle";
      break;
    case msrRehearsal::kBracket:
      fLilypondCodeIOstream <<
        "\\bracket";
      break;
    case msrRehearsal::kTriangle:
      fLilypondCodeIOstream <<
        "%{\\triangle???%}";
      break;
    case msrRehearsal::kDiamond:
      fLilypondCodeIOstream <<
        "%{\\diamond???";
      break;
  } // switch
      
  fLilypondCodeIOstream <<
    " { \""<<
    elt->getRehearsalText () <<
    "\"" "}}" <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrRehearsal" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMeasuresRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrMeasuresRepeat" <<
      endl;
  }

  int repeatMeasuresNumber =
    elt->measuresRepeatPatternMeasuresNumber ();

  int replicasMeasuresNumber =
    elt->measuresRepeatReplicasMeasuresNumber ();
    
  int replicasNumber =
    elt->measuresRepeatReplicasNumber ();

  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceRepeats) {
    fLilypondCodeIOstream <<
      "% measure repeat, line " << elt->getInputLineNumber () << ":" <<
      endl;

    const int fieldWidth = 24;

    fLilypondCodeIOstream << left <<
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
  
  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "% start of measure repeat" <<
      singularOrPlural (
        elt->measuresRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fLilypondCodeIOstream <<
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
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrMeasuresRepeat" <<
      endl;
  }

  gIndenter--;
  
  fLilypondCodeIOstream <<
    endl <<
    endl <<
    " }" <<
    endl;

  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream <<
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

void lpsr2LilypondTranslator::visitStart (S_msrMeasuresRepeatPattern& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> Start visiting msrMeasuresRepeatPattern" <<
      endl;
  }

  gIndenter++;

  // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasuresRepeatPattern& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> End visiting msrMeasuresRepeatPattern" <<
      endl;
  }

  gIndenter--;
}
  
void lpsr2LilypondTranslator::visitStart (S_msrMeasuresRepeatReplicas& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> Start visiting msrMeasuresRepeatReplicas" <<
      endl;
  }

  // output the start of the ending
  if (gLilypondOptions->fComments) {  
    fLilypondCodeIOstream << left <<
      setw (commentFieldWidth) <<
      "{" << "% start of measures repeat replicas" <<
      endl;
  }
  else {
    fLilypondCodeIOstream <<
      endl <<
      "{" <<
      endl;
  }

  gIndenter++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasuresRepeatReplicas& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> End visiting msrMeasuresRepeatReplicas" <<
      endl;
  }

  gIndenter--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMultipleRest& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrMultipleRest" <<
      endl;
  }

  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream << left <<
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

  fOnGoingMultipleRestMeasures = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrMultipleRest& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrMultipleRest" <<
      endl;
  }
    
  if (gLilypondOptions->fComments) {
    gIndenter--;

    fLilypondCodeIOstream << left <<
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
    
  if (gTraceOptions->fTraceMeasures || gTraceOptions->fTraceRepeats) {
    fLilypondCodeIOstream <<
      "% multiple rest, line " << inputLineNumber << ":" <<
      endl;

    const int fieldWidth = 24;

    fLilypondCodeIOstream << left <<
      setw (fieldWidth) <<
      "% restMeasuresNumber" << " = " << restMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% restSegmentMeasuresNumber" << " = " << restSegmentMeasuresNumber <<
      endl <<
      endl;
  }
  
  if (gLilypondOptions->fComments) {
    fLilypondCodeIOstream << left <<
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
  fLilypondCodeIOstream <<
    "R" <<
    multipleRestWholeNoteAsLilypondString (
      inputLineNumber,
      multipleRestMeasureSoundingNotes);

  if (restMeasuresNumber > 1) {
    fLilypondCodeIOstream <<
      "*" <<
      restMeasuresNumber;
  }

  if (gLilypondOptions->fNoteInputLineNumbers) {
    // print the multiple rest line number as a comment
    fLilypondCodeIOstream <<
      " %{ " << inputLineNumber << " %} ";
  }

  fLilypondCodeIOstream <<    
    " | % " <<
    elt->getMultipleRestNextMeasureNumber () <<
    endl;

  fOnGoingMultipleRestMeasures = false;
}

void lpsr2LilypondTranslator::visitStart (S_msrMultipleRestContents& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> Start visiting msrMultipleRestContents" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrMultipleRestContents& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "%--> End visiting msrMultipleRestContents" <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> Start visiting msrMidi" <<
      endl;
  }

  if (gLilypondOptions->fNoMidi) {
    fLilypondCodeIOstream <<
      "%{" <<
      endl;

     gIndenter++;
  }

  fLilypondCodeIOstream <<
    "\\midi" << " {" <<
    endl;
  
  gIndenter++;
  
  if (gLilypondOptions->fNoMidi)
    fLilypondCodeIOstream <<
      "% ";
  
  fLilypondCodeIOstream <<
    "\\tempo " <<
    elt->getMidiTempoDuration () <<
    " = " <<
    elt->getMidiTempoPerSecond () <<
    endl;
  
  gIndenter--;
  
  fLilypondCodeIOstream <<
    "}" <<
    endl;

  if (gLilypondOptions->fNoMidi) {
    gIndenter--;

    fLilypondCodeIOstream <<
      "%}" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fLilypondCodeIOstream <<
      "% --> End visiting msrMidi" <<
      endl;
  }
}


} // namespace


/* JMI
  // is there an unmetered (stemless) section?
  if (stemKind != fCurrentStemKind) {
    switch (stemKind) {
      case msrStem::kStemNone:
        if (! fOnGoingStemNone) {
          fLilypondCodeIOstream <<
            endl <<
            endl << //JMI
            "\\temporary\\omit Stem" <<
            endl <<
            "\\once\\omit Staff.TimeSignature" <<
            endl <<
            "\\cadenzaOn" <<
            endl <<
            endl; //JMI

          fOnGoingStemNone = true;
        }
        else {
          fLilypondCodeIOstream <<
            endl <<
            endl << //JMI
            "\\cadenzaOff" <<
            endl <<
            "\\bar \"|\"" <<
            endl <<
            "\\undo\\omit Stem" <<
            endl <<
            endl; //JMI
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
