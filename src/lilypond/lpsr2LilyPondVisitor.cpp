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

#include "lpsr2LilyPondVisitor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
lpsr2LilyPondVisitor::lpsr2LilyPondVisitor (
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
  
  fOnGoingStaff  = false;

  fOngoingNonEmptyLyrics = false;

  fOnGoingScoreBlock = false;

  fCurrentStemKind = msrStem::k_NoStem;
  
  fMusicOlec.setMaxElementsPerLine (
    fLpsrOptions->fGenerateNoteInputLineNumbers
      ?  5
      : 10);
      
  fLyricsOlec.setMaxElementsPerLine (10);
};
  
lpsr2LilyPondVisitor::~lpsr2LilyPondVisitor () {}

//________________________________________________________________________
void lpsr2LilyPondVisitor::generateLilyPondCodeFromLpsrScore ()
{
  if (fVisitedLpsrScore) {    
    // browse a msrScore browser
    msrBrowser<lpsrScore> browser (this);
    browser.browse (*fVisitedLpsrScore);
  }
}

//________________________________________________________________________
string lpsr2LilyPondVisitor::noteMsrPitchAsLilyPondString (
  S_msrNote& note)
{
  stringstream s;
  
  //JMI assertMsr(fMsrPitch != k_NoMsrPitch, "fMsrPitch != k_NoMsrPitch");

  if (note->getNoteIsUnpitched ())
    s << "unpitched ";
  else
    s << note->notePitchAsString ();
  
  // in MusicXML, octave number is 4 for the octave starting with middle C

  msrNoteData::msrDiatonicPitch
    noteDiatonicPitch =
      note->getDiatonicPitch ();
      
  string
    noteDiatonicPitchAsString =
      note->noteDiatonicPitchAsString ();
        
  int noteAbsoluteOctave =
    note->getNoteOctave ();

/*
  bool noteIsChordFirstNote =
    note->getNoteIsChordFirstNote ();
  */  
  // should an absolute octave be generated?
  bool genAbsoluteOctave =
    fLpsrOptions->fGenerateAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;
/* JMI
      ||
    noteIsChordFirstNote;
*/

 // if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
  if (fMsrOptions->fDebugDebug) {
    cerr << left <<
      endl <<
      idtr <<
          setw(33) << "% line" <<
          " = " <<
          note->getInputLineNumber () <<
          endl <<
      idtr <<
        setw(33) << "% noteDiatonicPitchAsString" <<
        " = - " <<
        noteDiatonicPitchAsString <<
        " -" <<
        endl <<
      idtr <<
        setw(33) << "% noteAbsoluteOctave" <<
        " = - " <<
        noteAbsoluteOctave <<
        " -" <<
        endl <<
        /*
      idtr <<
        setw(33) << "% noteIsChordFirstNote" <<
        " = " <<
        string (noteIsChordFirstNote
          ? "true"
          : "false") <<
        endl <<
        */
/*
      idtr <<
        setw(33) << "% noteDiatonicPitch" <<
        " = " <<
        noteDiatonicPitch <<
        endl <<
*/
    endl;
  }

  if (genAbsoluteOctave) {
    
    // generate LilyPond absolute octave
    switch (noteAbsoluteOctave) {
      case 0:
        s << ",,,";
        break;
      case 1:
        s << ",,";
        break;
      case 2:
        s << ",";
        break;
      case 3:
        s << "";
        break;
      case 4:
        s << "'";
        break;
      case 5:
        s << "''";
        break;
      case 6:
        s << "'''";
        break;
      case 7:
        s << "''''";
        break;
      case 8:
        s << "'''''";
        break;
      default:
        s << "###";
    } // switch
  }

  else {
    
    // generate LilyPond octave relative to fRelativeOctaveReference

    msrNoteData::msrDiatonicPitch
      referenceDiatonicPitch =
        fRelativeOctaveReference->getDiatonicPitch ();

    string
      referenceDiatonicPitchAsString =
        fRelativeOctaveReference->noteDiatonicPitchAsString ();
        
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
        noteDiatonicPitch - msrNoteData::kC,
        
      referenceAboluteDiatonicOrdinal =
        referenceAbsoluteOctave * 7
          +
        referenceDiatonicPitch - msrNoteData::kC;

//    if (fMsrOptions->fForceDebug || fMsrOptions->fDebugDebug) {
    if (fMsrOptions->fDebugDebug) {
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

/*
  if (noteIsChordFirstNote)
    fRelativeOctaveReference = note;
*/

  return s.str();
}
        
//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrScore" << endl;

  // initial empty line in LilyPond code
  // to help copy/paste it
  fOstream << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrScore" << endl;

  // final empty line in LilyPond code
  // to help copy/paste it
  fOstream << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrLilypondVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrLilypondVarValAssoc" << endl;

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

void lpsr2LilyPondVisitor::visitEnd (S_lpsrLilypondVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrLilypondVarValAssoc" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrSchemeVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrSchemeVarValAssoc" << endl;

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

void lpsr2LilyPondVisitor::visitEnd (S_lpsrSchemeVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrSchemeVarValAssoc" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrHeader& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrHeader" << endl;

  fOstream << idtr <<
    "\\header" << " {" <<
    endl;

  idtr++;

  fOnGoingHeader = true;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrHeader& elt)
{
  fOnGoingHeader = false;

  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrHeader" << endl;

  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrPaper& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrPaper" << endl;

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
  setprecision(4) << 0.0 << "\\cm" <<
  endl;

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
    os << idtr << "between-system-space = " << setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (elt->getPageTopSpace > 0) {
    os << idtr << "page-top-space = " << setprecision(4) << elt->getPageTopSpace << "\\cm" << endl;
  }
*/
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrPaper& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrPaper" << endl;
      
  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrLayout& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrLayout" << endl;

  fOstream <<
    idtr << "\\layout" << " {" <<
    endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrLayout& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrLayout" << endl;

  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrScoreBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrScoreBlock" << endl;

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

void lpsr2LilyPondVisitor::visitEnd (S_lpsrScoreBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrScoreBlock" << endl;
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
void lpsr2LilyPondVisitor::visitStart (S_lpsrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrParallelMusic" << endl;

  if (elt->getParallelMusicElements ().size()) { // JMI
    fOstream <<
      idtr << "<<" <<
      endl;
  
    idtr++;
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrParallelMusic" << endl;

  if (elt->getParallelMusicElements ().size()) { // JMI
    idtr--;
    
    fOstream <<
      idtr << ">>" <<
      endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrPartgroupBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrPartgroupBlock" << endl;

  // the top level part group is the score block's S_lpsrParallelMusic
  
  if (elt->getPartgroup ()->getPartgroupPartgroupUplink ()) {
    // the part group is not the top level one

    S_msrPartgroup
      partgroup =
        elt->getPartgroup ();
        
    msrPartgroup::msrPartgroupSymbolKind
      partgroupSymbolKind =
        partgroup->getPartgroupSymbolKind ();
  
    string
      partgroupInstrumentName =
        partgroup->getPartgroupInstrumentName ();
        
    string partGroupContextName;
  
    // LPNR, page 567
    
    switch (partgroupSymbolKind) {
      case msrPartgroup::k_NoPartgroupSymbol:
        partGroupContextName = "";
        break;
        
      case msrPartgroup::kBracePartgroupSymbol: // JMI
        if (partgroupInstrumentName.size ())
          partGroupContextName = "PianoStaff";
        else
          partGroupContextName = "GrandStaff";
        break;
        
      case msrPartgroup::kBracketPartgroupSymbol:
        partGroupContextName = "GrandStaff";
        break;
        
      case msrPartgroup::kLinePartgroupSymbol:
        partGroupContextName = "StaffGroup";
        break;
        
      case msrPartgroup::kSquarePartgroupSymbol:
        partGroupContextName = "StaffGroup";
        break;
    } // switch
  
  
    fOstream << idtr <<
      "\\new " << partGroupContextName << " " "<<";
      
    if (fLpsrOptions->fGenerateComments)
      fOstream <<
        setw(30) << " " << "% part group " <<
        elt->getPartgroup ()->getPartgroupCombinedName ();
        
    fOstream <<
      endl;

  /* JMI
    if (partgroupInstrumentName.size ())
      fOstream << idtr <<
        "\\set PianoStaff.instrumentName = #\"" <<
        partgroupInstrumentName <<
        "\"" <<
        endl;
  */
  
    fOstream <<
      endl;
  
    idtr++;
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrPartgroupBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrPartgroupBlock" << endl;

  // the top level part group is the score block's S_lpsrParallelMusic
  
  if (elt->getPartgroup ()->getPartgroupPartgroupUplink ()) {
    // the part group is not the top level one
    idtr--;

    fOstream <<
      idtr <<
      ">>";
    if (fLpsrOptions->fGenerateComments)
      fOstream <<
        setw(30) << " " << "% part group " <<
        elt->getPartgroup ()->getPartgroupCombinedName ();
    fOstream <<
      endl << endl;
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrPartBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrPartBlock" << endl;

  S_msrPart
    part =
      elt->getPart ();
      
  string
    partInstrumentName =
      part->getPartInstrumentName ();

  if (fLpsrOptions->fGenerateComments) {
    fOstream << left <<
      idtr <<
      setw(30) << "\\new StaffGroup <<" <<
      " % part " << elt->getPart ()->getPartCombinedName ();
  }
  else {
    fOstream << idtr <<
      "\\new StaffGroup" <<  " " "<<";      
  }
      /* JMI
  if (partInstrumentName.size ())
    fOstream << idtr <<
      "\\set Staff.instrumentName = #\"" <<
      partInstrumentName <<
      "\"" <<
      endl;
      */

  fOstream <<
    endl << endl;
  
  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrPartBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrPartBlock" << endl;

  idtr--;

  if (fLpsrOptions->fGenerateComments) {
    fOstream <<
      idtr <<
      setw(30) << ">>" <<    
      " % part " <<
      elt->getPart ()->getPartCombinedName ();
  }
  else {
    fOstream <<
      idtr <<
      ">>";
  }
  
  fOstream <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrStaffBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrStaffBlock" << endl;

  S_msrStaff
    staff =
      elt->getStaff ();
      
  S_msrPart
    part =
      staff->getStaffPartUplink ();
      
  string
    staffInstrumentName = // JMI
      staff->getStaffInstrumentName (),
    partName =
      part->getPartName (),
    partAbbreviation =
      part->getPartAbbreviation ();

  if (fLpsrOptions->fGenerateComments) {
    fOstream << left <<
      idtr <<
      setw(30) << "\\new Staff <<" <<
      " % staff " << elt->getStaff ()->getStaffName ();
  }
  else {
    fOstream << idtr <<
      "\\new Staff" <<  " " "<<";      
  }
      
  fOstream <<
    endl;

  idtr++;

  fOstream << idtr <<
    "\\set Staff.instrumentName = \"" <<
    partName <<
    "\"" <<
    endl;

  fOstream << idtr <<
    "\\set Staff.shortInstrumentName = \"" <<
    partAbbreviation <<
    "\"" <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrStaffBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrStaffBlock" << endl;

  idtr--;

  if (fLpsrOptions->fGenerateComments) {
    fOstream <<
      idtr <<
      setw(30) << ">>" <<    
      " % staff " <<
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
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewStaffgroupBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffgroupBlock" << endl;

   fOstream << idtr <<
     "\\new StaffGroup" << " " << "{" <<
      endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewStaffgroupBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffgroupBlock" << endl;

  idtr--;

  fOstream <<
    " }" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewStaffBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffBlock" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewStaffBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffBlock" << endl;

  idtr--;
}
*/

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrUseVoiceCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrUseVoiceCommand" << endl;

  S_msrVoice voice = elt->getVoice ();

  S_msrStaff staff = voice-> getVoiceStaffUplink ();

  fOstream << idtr <<
    "\\context Voice" << " = " <<
    "\"" << voice->getVoiceName () << "\"" " " "{" <<
     endl;

  idtr++;

  if (fLpsrOptions->fNoAutoBeaming)
    fOstream << idtr <<
      "\\set Voice.autoBeaming = ##f" <<
      endl;

  if (staff->getStaffVoicesMap ().size () > 1) {
    fOstream << idtr;
    switch (voice->getVoiceNumber ()) {
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
    fOstream <<
      endl;
  }

  fOstream <<
    idtr << "\\" << voice->getVoiceName () << endl;

  idtr--;
  
  fOstream <<
    idtr << "}" <<
    endl;

 // JMI \context Voice = "PartPOneVoiceOne" { \PartPOneVoiceOne }
 
}
void lpsr2LilyPondVisitor::visitEnd (S_lpsrUseVoiceCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrUseVoiceCommand" << endl;
}
  
//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewLyricsBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewLyricsBlock" << endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
 //   if (fMsrOptions->fForceDebug || fOngoingNonEmptyLyrics) {
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
          "\\" << elt->getLyrics ()->getLyricsName () <<
        endl;

      idtr--;
 //   }
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewLyricsBlock& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewLyricsBlock" << endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
    if (fMsrOptions->fForceDebug || fOngoingNonEmptyLyrics) { // JMI
    }
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrVariableUseCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrVariableUseCommand" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrVariableUseCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrVariableUseCommand" << endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrContext& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrContext" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrContext& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrContext" << endl;

  idtr--;
}
  
//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrBarCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrBarCommand" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrBarCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrBarCommand" << endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrScore" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrScore" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrPartgroup& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrPartgroup" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrPartgroup& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrPartgroup" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrPart" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrPart" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrStaff& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrStaff" << endl;
  
  fOnGoingStaff = true;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrStaff& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrStaff" << endl;
  
  fOnGoingStaff = false;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrVoice& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrVoice" << endl;

  fOstream << idtr <<
    elt->getVoiceName () << " = ";

  if (! fLpsrOptions->fGenerateAbsoluteOctaves)
    fOstream << "\\relative ";
    
  fOstream <<
    "{" <<
    endl;

  idtr++;

  fRelativeOctaveReference = 0;

  fMusicOlec.reset ();
}

void lpsr2LilyPondVisitor::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrVoice" << endl;
  
  fOstream << idtr <<
    "}" <<
    endl <<
    endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrVoicechunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrVoicechunk" << endl;

  if (fLpsrOptions->fGenerateComments) {
    fOstream << idtr <<
      "{" <<
      setw(30) << " " "% start of msrVoicechunk" <<
      endl;

    idtr++;
  }

  fVoicechunkNotesAndChordsCountersStack.push (0);
}

void lpsr2LilyPondVisitor::visitEnd (S_msrVoicechunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrVoicechunk" << endl;

  if (fLpsrOptions->fGenerateComments) {
    idtr--;
    
    fOstream <<
      idtr <<
      "}" <<
      setw(30) << " " "% end of msrVoicechunk" <<
      endl;
  }
  /* JMI
  else
    fOstream <<
      endl;
*/

  fVoicechunkNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrMeasure& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrMeasure" << endl;

  if (fLpsrOptions->fGenerateComments) {
    fOstream << idtr <<
      "{" <<
      setw(30) << " " "% start of msrMeasure" <<
      endl;

    idtr++;
  }

  fVoicechunkNotesAndChordsCountersStack.push (0);

  switch (elt->getMeasureKind ()) {
    case msrMeasure::kRegularMeasure:
      break;
      
    case msrMeasure::kIncompleteLeftMeasure:
      {
        string errorMessage;

        string partialDuration =
          divisionsAsMSRDuration (
            elt->getMeasureLength (),
            elt->getMeasureDivisionsPerWholeNote (),
            errorMessage,
            false); // 'true' to debug it
            
        fOstream << idtr <<
          "\\partial" " " << partialDuration <<
          endl;
      }
      break;
      
    case msrMeasure::kIncompleteRightMeasure:
      // there's no need for '\partial' in this case
      break;
  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_msrMeasure& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrMeasure" << endl;

  if (fLpsrOptions->fGenerateComments) {
    idtr--;
    
    fOstream <<
      endl <<
      idtr <<
      "}" <<
      setw(30) << " " "% end of msrMeasure" <<
      endl;
  }
  /* JMI
  else
    fOstream <<
      endl <<
      idtr;
*/

  fVoicechunkNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrLyrics" << endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
    fOngoingNonEmptyLyrics =
      elt->getLyricsTextPresent ();

    if (fOngoingNonEmptyLyrics) {
      fOstream << idtr <<
        elt->getLyricsName () << " = " << "\\lyricmode" << " {" <<
        endl;
        
      idtr++;
      
      fOstream << idtr;
      
      fLyricsOlec.reset ();
    }
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrLyrics" << endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
    if (fOngoingNonEmptyLyrics) {
      idtr--;
    
      fOstream <<
        idtr << "}" <<
        endl <<
        endl;
    }

    fOngoingNonEmptyLyrics = false;
  }
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrLyricschunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrLyricschunk" << endl;

  if (! fLpsrOptions->fDontGenerateLilyPondLyrics) {
    if (fMsrOptions->fForceDebug || fOngoingNonEmptyLyrics) {
      
      switch (elt->getLyricschunkKind ()) {
        case msrLyricschunk::kSingleChunk:
          fOstream <<
            quoteStringIfNonAlpha (elt->getChunkText ()) << " ";
          break;
          
        case msrLyricschunk::kBeginChunk:
          fOstream <<
            quoteStringIfNonAlpha (elt->getChunkText ()) << " ";
          break;
          
        case msrLyricschunk::kMiddleChunk:
          fOstream <<
            "-- " <<
            quoteStringIfNonAlpha (elt->getChunkText ()) << " ";
          break;
          
        case msrLyricschunk::kEndChunk:
          fOstream <<
            "-- " <<
            quoteStringIfNonAlpha (elt->getChunkText ()) << " ";
          break;
          
        case msrLyricschunk::kSkipChunk:
          fOstream <<
            "\\skip" <<
              elt->getLyricschunkNote ()->noteDivisionsAsMSRString () <<
              " ";
          break;
          
        case msrLyricschunk::kSlurChunk:
          fOstream <<
            "%{ slur " << "\"" << elt->getChunkText () << "\"" << " %}" <<
            endl <<
            idtr;
          break;
  
        case msrLyricschunk::kTiedChunk:
          fOstream <<
            "%{ ~ " << "\"" << elt->getChunkText () << "\"" << " %}" <<
            endl <<
            idtr;
          break;
          
        case msrLyricschunk::kSlurBeyondEndChunk:
  // JMI       fOstream <<
   //         "__ " << " ";
          break;
  
        case msrLyricschunk::kBarcheckChunk:
          fOstream <<
     // JMI       "%{ | % " << elt->getChunkText () << " %}" <<
            "| %{ " << elt->getChunkText () << " %}" <<
            endl <<
            idtr;

          fLyricsOlec.reset ();
          break;
    
        case msrLyricschunk::kBreakChunk:
          fOstream <<
            "%{ break " << "\"" << elt->getChunkText () << "\"" << " %}" <<
            endl <<
            idtr;
          break;
    
        case msrLyricschunk::k_NoChunk: // JMI
          break;
      } // switch

      fLyricsOlec++;
    }
  }
}

void lpsr2LilyPondVisitor::visitEnd (S_msrLyricschunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrLyricschunk" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrClef" << endl;

  fOstream << idtr <<
    "\\clef" << " \"";

  if ( elt->getSign () == "G") {
    
    if ( elt->getLine () == 2 )
      fOstream << "treble"; 
    else { // unknown G clef line !!
      cerr << 
        "warning: unknown G clef line \"" << elt->getLine () << "\"" <<
        endl;
      return; 
    }
  }
    
  else
  if ( elt->getSign () == "F") {
    
    if ( elt->getLine () == 4 )
      fOstream << "bass"; 
    else { // unknown F clef line !!
      cerr << 
        "warning: unknown F clef line \"" << elt->getLine () << "\"" <<
        endl;
      return; 
    }
  }
    
  else
  if ( elt->getSign () == "C") {
    
    if ( elt->getLine () == 4 )
      fOstream << "tenor"; 
    else if ( elt->getLine () == 3 )
      fOstream << "alto"; 
    else { // unknown C clef line !!
      cerr << 
        "warning: unknown C clef line \"" <<
        elt->getLine () << 
        "\"" <<
        endl;
      return; 
    }
  }

  else
  if ( elt->getSign () == "percussion") {
    fOstream << "perc";
  }

  else
  if ( elt->getSign () == "TAB") {
    fOstream << "TAB";
  }

  else
  if ( elt->getSign () == "none") {
    fOstream << "none";
  }
    
  else { // unknown clef sign !!
    cerr << 
      "warning: unknown clef sign \"" <<
       elt->getSign () << 
       "\"" <<
      endl;
    return; 
  }
        
  if (elt->getOctaveChange () == 1)
    fOstream << "^8";
  else
  if (elt->getOctaveChange () == -1)
    fOstream << "_8";

  fOstream <<
    "\"" <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrClef" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrKey" << endl;

  fOstream << idtr <<
    "\\key " << elt->getKeyTonic () << " ";

  switch (elt->getKeyModeKind ()) {
    case msrKey::kMajor:
      fOstream << "\\major";
      break;
      
    case msrKey::kMinor:
      fOstream << "\\minor";
      break;
  } // switch
  
  fOstream << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrKey" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrTime" << endl;

  if (fLpsrOptions->fGenerateNumericalTime)
    fOstream << "\\numericTimeSignature ";
    
  fOstream << idtr <<
    "\\time" " " <<
    elt->getBeatsNumber () <<
    "/" <<
    elt->getBeatsValue () <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTime" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrTempo" << endl;

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

void lpsr2LilyPondVisitor::visitEnd (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTempo" << endl;
}

//________________________________________________________________________
/*
void lpsr2LilyPondVisitor::visitStart (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrDuration" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrDuration" << endl;
}
*/

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrArticulation" << endl;

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilyPondVisitor::visitEnd (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrArticulation" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrDynamics" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrDynamics" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrWords& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrWords" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrWords& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrWords" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrSlur" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrSlur" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrWedge" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrWedge" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrGracenotes& elt)
{
//  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrGracenotes" << endl;

  if (elt->getGracenotesIsSlashed ())
    fOstream << idtr <<
      "\\grace";
  else
    fOstream << idtr <<
      "\\acciaccatura";
  fOstream << " { ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrGracenotes& elt)
{
//  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrGracenotes" << endl;

  fOstream <<
    " } ";
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrNote& elt)
{
//  if (fMsrOptions->fForceDebug || fMsrOptions->fDebug) {
  if (fMsrOptions->fDebug) {
    fOstream << idtr <<
      "% --> Start visiting ";
      
    switch (elt->getNoteKind ()) {

      case msrNote::k_NoNoteKind:
        break;
        
      case msrNote::kStandaloneNote:
      /*
        if (fMusicElementsCounter > fMaxMusicElementsOnOneLine) {
          fOstream <<
            endl <<
            idtr;
          fMusicElementsCounter = 0;
        }
*/
        fOstream << "standalone";
        break;
        
      case msrNote::kGraceNote:
        fOstream << "grace";
        break;
        
      case msrNote::kRestNote:        
        fOstream << "rest";
        break;
        
      case msrNote::kChordMemberNote:
        fOstream << "chord member";
        break;
        
      case msrNote::kTupletMemberNote:
        fOstream << "tuplet member";
        break;
    } // switch
    
    fOstream << " msrNote" << endl;
    fMusicOlec++;
  }

  // indent before the fist note of the msrVoicechunk if needed
  if (++ fVoicechunkNotesAndChordsCountersStack.top () == 1)
    fOstream << idtr;

  // should stem direction be generated?
  if (fLpsrOptions->fGenerateStems) {

    if (fCurrentStem) {
      // get note stem kind 
      msrStem::msrStemKind
        stemKind =
          fCurrentStem->getStemKind ();
    
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
        } // switch
      }
      fMusicOlec++;

      // is there a stem kind change?
      if (stemKind != fCurrentStemKind)
        fCurrentStemKind = stemKind;
    }
  }
/*
  if (++fMusicElementsCounter > fMaxMusicElementsOnOneLine) {
    fOstream <<
      endl <<
      idtr;
    fMusicElementsCounter = 1;
  }
*/

  /*
  cout <<
    "#### fMusicElementsCounter = " << fMusicElementsCounter << endl <<
    "#### fMaxMusicElementsOnOneLine = " << fMaxMusicElementsOnOneLine << endl <<
    endl;
  */
  
  switch (elt->getNoteKind ()) {
    
    case msrNote::k_NoNoteKind:
      break;
        
    case msrNote::kStandaloneNote:
      // print the note name
      fOstream <<
        noteMsrPitchAsLilyPondString (elt);
      
      // print the note duration
      fOstream <<
        elt->noteDivisionsAsMSRString ();
      
      // print the tie if any
      {
        S_msrTie noteTie = elt->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fOstream << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = elt;
      break;

    case msrNote::kGraceNote:
      // print the note name
      fOstream <<
        noteMsrPitchAsLilyPondString (elt);
      
      // print the note duration
      fOstream <<
        elt->noteTypeAsMSRString ();

      // print the dots if any
      for (int i = 0; i < elt->getNoteDotsNumber (); i++) {
        fOstream << ".";
      } // for
      
      // print the tie if any
      {
        S_msrTie noteTie = elt->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fOstream << "~ ";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = elt;
      break;
      
    case msrNote::kRestNote:      
      // print the rest name
      fOstream <<
        string (
          elt->getNoteOccupiesAFullMeasure ()
            ? "R"
            : "r");
      
      // print the note duration
      fOstream <<
        elt->noteDivisionsAsMSRString ();

      // a rest is no relative octave reference,
      // the preceding one is kept
      break;
      
    case msrNote::kChordMemberNote:
      // print the note name
      fOstream <<
        noteMsrPitchAsLilyPondString (elt);
      
      // don't print the note duration,
      // it will be printed for the chord itself

      // inside chords, a note is relative to the preceding one
      fRelativeOctaveReference = elt;
      break;
      
    case msrNote::kTupletMemberNote:
      // print the note name
      if (elt->getNoteIsARest ()) {
        fOstream <<
          string (
            elt->getNoteOccupiesAFullMeasure ()
              ? "R"
              : "r");
      }
      else
        fOstream <<
          noteMsrPitchAsLilyPondString (elt);
      
      // print the note duration
      fOstream <<
        elt->noteDivisionsAsMSRString ();

      // print the tie if any
      {
        S_msrTie noteTie = elt->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fOstream << "~ ";
          }
        }
      }

      // a rest is no relative octave reference,
      if (! elt->getNoteIsARest ())
        // this note is the new relative octave reference
        fRelativeOctaveReference = elt;
      break;
  } // switch

  fOstream << " ";

  fMusicOlec++;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrNote& elt)
{
  if (fMsrOptions->fDebug) {
    fOstream << idtr <<
      "% --> Start visiting ";
    switch (elt->getNoteKind ()) {
      case msrNote::k_NoNoteKind:
        break;
        
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
        break;
        
      case msrNote::kGraceNote:
        fOstream << "grace";
        break;
        
      case msrNote::kRestNote:
        fOstream << "rest";
        break;
        
      case msrNote::kChordMemberNote:
        fOstream << "chord member";
        break;
        
      case msrNote::kTupletMemberNote:
        fOstream << "tuplet member";
        break;
    } // switch
    fOstream << " msrNote" << endl;
  }

  switch (elt->getNoteKind ()) {
    case msrNote::k_NoNoteKind:
      break;
    case msrNote::kStandaloneNote:
      break;
    case msrNote::kGraceNote:
      break;
    case msrNote::kRestNote:
      break;
    case msrNote::kChordMemberNote:
      break;
    case msrNote::kTupletMemberNote:
      break;
  } // switch

  if (fLpsrOptions->fGenerateNoteInputLineNumbers)
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
        case msrArticulation::kStaccato:
          fOstream << "-.";
          break;
        case msrArticulation::kStaccatissimo:
          fOstream << "-!";
          break;
        case msrArticulation::kDetachedLegato:
          fOstream << "-_";
          break;
        case msrArticulation::kFermata:
          fOstream << "\\fermata";
          break;
        case msrArticulation::kTrill:
          fOstream << "\\trill";
          break;
        case msrArticulation::kArpeggiato:
          fOstream << "\\arpeggio";
          /* JMI
           in one of the voices add:
  \once \set StaffGroup.connectArpeggios = ##t
  */
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

      switch (wordsPlacementKind) {
        case msrWords::kAbove:
          fOstream << "^";
          break;
        case msrWords::kBelow:
          fOstream << "_";
          break;
      } // switch
    
      fOstream <<
        "\\markup" << " { ";
      if (wordsFontStyle.size ())
        fOstream <<
          "\\" << wordsFontStyle << " ";
      if (wordsFontWeight.size ())
        fOstream <<
          "\\" << wordsFontWeight << " ";
      fOstream <<
        quoteStringIfNonAlpha (wordsContents) <<
        " } ";

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

/* JMI
  // print the tie if any
  {
    S_msrTie noteTie = elt->getNoteTie ();
  
    if (noteTie) {
      if (noteTie->getTieKind () == msrTie::kStartTie) {
        fOstream << "~ ";
      }
    }
  }

  // print the slurs if any
  switch (elt->getNoteslur ()->getSlurKind ()) {
    case msrSlur::kStartSlur:
      fOstream << "( ";
      break;
      
    case msrSlur::kContinueSlur:
      break;
      
    case msrSlur::kStopSlur:
      fOstream << ") ";
      break;
      
    case msrSlur::k_NoSlur:
      break;
  } // switch
  */
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrOctaveShift& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrOctaveShift" << endl;

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

void lpsr2LilyPondVisitor::visitEnd (S_msrOctaveShift& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrOctaveShift" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrStem& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrStem" << endl;

  fCurrentStem = elt;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrStem& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrStem" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBeam" << endl;

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

void lpsr2LilyPondVisitor::visitEnd (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBeam" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrChord" << endl;

  // indent before the fist chord of the msrVoicechunk if needed
  if (++ fVoicechunkNotesAndChordsCountersStack.top () == 1)
    fOstream << idtr;

  // don't take the chord into account for line breaking ??? JMI
  
  fOstream << "<";
  fMusicOlec++;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrChord" << endl;

  fOstream <<
    ">" <<
    elt->chordDivisionsAsMSRString () << " ";
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
        case msrArticulation::kStaccato:
          fOstream << "-.";
          break;
        case msrArticulation::kStaccatissimo:
          fOstream << "-!";
          break;
        case msrArticulation::kDetachedLegato:
          fOstream << "-_";
          break;
        case msrArticulation::kFermata:
          fOstream << "\\fermata";
          break;
        case msrArticulation::kTrill:
          fOstream << "\\trill";
          break;
        case msrArticulation::kArpeggiato:
          fOstream << "\\arpeggio";
          break;
      } // switch
      
      fOstream << " ";
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
  fRelativeOctaveReference = elt->getChordNotes () [0];
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrTuplet" << endl;

  fOstream <<
    "\\tuplet " <<
    elt->getTupletActualNotes () <<
    "/" <<
    elt->getTupletNormalNotes() << " { ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTuplet" << endl;

  fOstream << " } ";
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrTie& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrTie" << endl;
  
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

void lpsr2LilyPondVisitor::visitEnd (S_msrTie& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTie" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrSegno& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrSegno" << endl;

  fOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.segno\" }" <<
    endl;
}

void lpsr2LilyPondVisitor::visitStart (S_msrCoda& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrCoda" << endl;

  fOstream <<
    "\\mark \\markup { \\musicglyph #\"scripts.coda\" }" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrEyeglasses& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting eyeglasses" << endl;

  fOstream <<
    "\\eyeglasses ";
  fMusicOlec++;
}

void lpsr2LilyPondVisitor::visitStart (S_msrPedal& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting pedal" << endl;
      
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
void lpsr2LilyPondVisitor::visitStart (S_msrBarline& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBarline" << endl;

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
        endl << idtr;

      if (elt->getBarlineHasSegno ())
        fOstream <<
          "\\mark \\markup { \\musicglyph #\"scripts.segno\" } ";
      if (elt->getBarlineHasCoda ())
        fOstream <<
          "\\mark \\markup { \\musicglyph #\"scripts.coda\" } ";
      break;

    case msrBarline::kRepeatStart:
    case msrBarline::kRepeatEnd:
    case msrBarline::kEndingStart:
    case msrBarline::kHookedEndingEnd:
    case msrBarline::kHooklessEndingEnd:
      // should not occur, since
      // LilyPond will take care of displaying the repeat
      break;

  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBarline& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBarline" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBarCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBarCheck" << endl;
      
  int nextBarNumber =
    elt->getNextBarNumber ();

  // don't generate a bar check before the end of measure 1
 // JMI if (nextBarNumber > 1)
    fOstream <<
      "| % " << nextBarNumber <<
      endl;

    fMusicOlec.reset ();
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBarCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBarCheck" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBarnumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBarnumberCheck" << endl;

  fOstream << idtr <<
    "\\barNumberCheck #" << elt->getNextBarNumber () <<
    endl <<
    idtr;

  fMusicOlec.reset ();
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBarnumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBarnumberCheck" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBreak" << endl;

  fOstream <<
    "\\myBreak | % " << elt->getNextBarNumber () <<
    endl <<
    endl <<
    idtr;

  fMusicOlec.reset ();
  fLyricsOlec.reset ();
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBreak" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrRepeat" << endl;

  int voltaNumber =
    elt->getRepeatEndings ().size();
  
  if (voltaNumber == 0)
    voltaNumber = 2; // implicitly

  stringstream s;
  s <<
    "\\repeat volta " << voltaNumber << " {";
  
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

  fMusicOlec.reset ();
}

void lpsr2LilyPondVisitor::visitEnd (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrRepeat" << endl;

  if (elt->getRepeatEndings ().size() == 0) {
    // the end of the repeat has not been generated yet

    idtr --;
    
    if (fLpsrOptions->fGenerateComments) {
      idtr--;
      
      fOstream <<
        idtr <<
        "}" <<
        setw(30) << " " "% end of repeat" <<
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

  fMusicOlec.reset ();
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrRepeatending& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrRepeatending" << endl;

  if (elt->getRepeatendingInternalNumber () == 1) {
    
    idtr--;
    
    // first repeat ending is in charge of
    // outputting the end of the repeat
    fOstream << idtr <<
      setw(30) << "}" << "% end of repeat" <<
      endl << endl;

    // first repeat ending is in charge of
    // outputting the start of the alternative
    fOstream << idtr <<
      setw(30) << "\\alternative" " " "{" "% start of alternative" <<
      endl;
    
    idtr++;
  }

  fMusicOlec.reset ();
}

void lpsr2LilyPondVisitor::visitEnd (S_msrRepeatending& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrRepeatending" << endl;

  if (
  // JMI warning: comparison between signed and unsigned integer expressions [-Wsign-compare]
    elt->getRepeatendingInternalNumber ()
      ==
    (int) elt->getRepeatendingRepeatUplink ()->getRepeatEndings ().size()) {
      
    idtr--;
    
    // last repeat ending is in charge of
    // outputting the end of the alternative
    fOstream <<
      endl <<
      idtr <<
      setw(30) << "}" << "% end of alternative" <<
      endl << endl;
  }

  fMusicOlec.reset ();
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrComment" << endl;

  fOstream << idtr <<
    "% " << elt->getContents () <<
    endl;
    
  if (elt->getGapKind () == lpsrComment::kGapAfterwards)
    fOstream << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrComment" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrRehearsal& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrRehearsal" << endl;

  fOstream <<
    endl <<
    endl <<
    idtr <<
    "\\mark" " \""<< elt->getRehearsalText () << "\"" <<
    endl <<
    idtr;
  
}

void lpsr2LilyPondVisitor::visitEnd (S_msrRehearsal& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrRehearsal" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrMidi" << endl;

  fOstream << idtr <<
    "\\midi" << " {" <<
    endl;
  
  idtr++;
  
  fOstream << idtr <<
    "% to be completed" <<
    endl;
  
  idtr--;
  
  fOstream << idtr <<
    "}" <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrMidi" << endl;
}


} // namespace
