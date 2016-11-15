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
  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
{
  fMsrOptions       = msrOpts;
  fLpsrOptions      = lpsrOpts;
  fVisitedLpsrScore = lpScore;

  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
  gCurrentMusicXMLLocation.fInputLineNumber = 0;
  gCurrentMusicXMLLocation.fMeasureNumber = 0; // in case of an anacrusis
  gCurrentMusicXMLLocation.fPositionInMeasure = 1;

  fOnGoingHeader = false;
  fOnGoingStaff  = false;
};
  
lpsr2LilyPondVisitor::~lpsr2LilyPondVisitor ()
{}

//________________________________________________________________________
void lpsr2LilyPondVisitor::generateLilyPondCodeFromLpsrScore ()
{
  if (fVisitedLpsrScore) {    
    // create a msrScore browser
    msrBrowser<lpsrScore> browser (this);

    // browse the score with the browser
    browser.browse (*fVisitedLpsrScore);
  }
}

//________________________________________________________________________
string lpsr2LilyPondVisitor::noteMsrPitchAsLilyPondString (
  S_msrNote& note)
{
  stringstream s;
  
  //JMI assertMsr(fMsrPitch != k_NoMsrPitch, "fMsrPitch != k_NoMsrPitch");

 // JMI if (fMusicXMLNoteData.fMusicXMLStepIsUnpitched)
//    s << "unpitched ";
    
  switch (note->getNoteMsrPitch ()) {
    
    case msrNote::k_aeseh: s << "aeseh"; break;
    case msrNote::k_aes:   s << "aes"; break;
    case msrNote::k_aeh:   s << "aeh"; break;
    case msrNote::k_a:     s << "a"; break;
    case msrNote::k_aih:   s << "aih"; break;
    case msrNote::k_ais:   s << "ais"; break;
    case msrNote::k_aisih: s << "aisih"; break;
      
    case msrNote::k_beseh: s << "beseh"; break;
    case msrNote::k_bes:   s << "bes"; break;
    case msrNote::k_beh:   s << "beh"; break;
    case msrNote::k_b:     s << "b"; break;
    case msrNote::k_bih:   s << "bih"; break;
    case msrNote::k_bis:   s << "bis"; break;
    case msrNote::k_bisih: s << "bisih"; break;
      
    case msrNote::k_ceseh: s << "ceseh"; break;
    case msrNote::k_ces:   s << "ces"; break;
    case msrNote::k_ceh:   s << "ceh"; break;
    case msrNote::k_c:     s << "c"; break;
    case msrNote::k_cih:   s << "cih"; break;
    case msrNote::k_cis:   s << "cis"; break;
    case msrNote::k_cisih: s << "cisih"; break;
      
    case msrNote::k_deseh: s << "deseh"; break;
    case msrNote::k_des:   s << "des"; break;
    case msrNote::k_deh:   s << "deh"; break;
    case msrNote::k_d:     s << "d"; break;
    case msrNote::k_dih:   s << "dih"; break;
    case msrNote::k_dis:   s << "dis"; break;
    case msrNote::k_disih: s << "disih"; break;

    case msrNote::k_eeseh: s << "eeseh"; break;
    case msrNote::k_ees:   s << "ees"; break;
    case msrNote::k_eeh:   s << "eeh"; break;
    case msrNote::k_e:     s << "e"; break;
    case msrNote::k_eih:   s << "eih"; break;
    case msrNote::k_eis:   s << "eis"; break;
    case msrNote::k_eisih: s << "eisih"; break;
      
    case msrNote::k_feseh: s << "feseh"; break;
    case msrNote::k_fes:   s << "fes"; break;
    case msrNote::k_feh:   s << "feh"; break;
    case msrNote::k_f:     s << "f"; break;
    case msrNote::k_fih:   s << "fih"; break;
    case msrNote::k_fis:   s << "fis"; break;
    case msrNote::k_fisih: s << "fisih"; break;
      
    case msrNote::k_geseh: s << "geseh"; break;
    case msrNote::k_ges:   s << "ges"; break;
    case msrNote::k_geh:   s << "geh"; break;
    case msrNote::k_g:     s << "g"; break;
    case msrNote::k_gih:   s << "gih"; break;
    case msrNote::k_gis:   s << "gis"; break;
    case msrNote::k_gisih: s << "gisih"; break;
    
    default: s << "Note" << note->getNoteMsrPitch () << "???";
  } // switch

  // in MusicXML, octave number is 4 for the octave starting with middle C
  switch (note->getNoteMusicXMLOctave ()) {
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
  
  return s.str();
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrScore" << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrScore" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrVarValAssoc" << endl;

  fOstream << idtr;

  switch (elt->getBackslashKind ()) {
    case lpsrVarValAssoc::kWithBackslash:
      fOstream << "\\";
      break;
    case lpsrVarValAssoc::kWithoutBackslash:
      break;
  } // switch
  
  if (elt->getCommentedKind () == lpsrVarValAssoc::kCommented)
    fOstream << "\%";
  
  fOstream <<
    elt->getVariableName ();
  
  if (elt->getVarValSeparator () == lpsrVarValAssoc::kEqualSign)
    fOstream << " = ";
  else
    fOstream << " ";
  
  if (elt->getQuotesKind () == lpsrVarValAssoc::kQuotesAroundValue)
    fOstream << "\"";
    
  fOstream <<
    elt->getVariableValue ();

  if (elt->getUnit ().size())
    fOstream <<
      "\\" <<
      elt->getUnit ();
  
  if (elt->getQuotesKind () == lpsrVarValAssoc::kQuotesAroundValue)
    fOstream << "\"";
  
  fOstream << endl;

  switch (elt->getEndlKind ()) {
    case lpsrVarValAssoc::kWithEndl:
      fOstream << endl;
      break;
    case lpsrVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrVarValAssoc" << endl;
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
  /*
  if (fPaperWidth > 0) {
    fOstream << idtr <<
      "paper-width = " <<
      setprecision(4) << fPaperWidth << "\\cm" << endl;
  }
  if (fPaperHeight > 0) {
    fOstream << idtr <<
      "paper-height = " <<
      setprecision(4) << fPaperHeight << "\\cm" << endl;
  }
  if (fTopMargin > 0) {
    fOstream << idtr <<
      "top-margin = " <<
      setprecision(4) << fTopMargin << "\\cm" << endl;
  }
  if (fBottomMargin > 0) {
    fOstream << idtr <<
      "bottom-margin = " <<
      setprecision(4) << fBottomMargin << "\\cm" << endl;
  }
  if (fLeftMargin > 0) {
    fOstream << idtr <<
      "left-margin = " <<
      setprecision(4) << fLeftMargin << "\\cm" << endl;
  }

  if (fRightMargin > 0) {
    fOstream << idtr <<
      "right-margin = " <<
      setprecision(4) << fRightMargin << "\\cm" << endl;
  }
*/
/*
  if (fBetweenSystemSpace > 0) {
    fOstream << idtr << "between-system-space = " <<
      setprecision(4) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    fOstream << idtr << "page-top-space = " <<
      setprecision(4) << fPageTopSpace << "\\cm" << endl;
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
void lpsr2LilyPondVisitor::visitStart (S_msrLayout& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrLayout" << endl;

  fOstream << idtr <<
    "\\layout" << " {" <<
    endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrLayout& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrLayout" << endl;

  fOstream << idtr <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrScoreCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrScoreCommand" << endl;

  fOstream << idtr <<
    "\\score {" <<
    endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrScoreCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrScoreCommand" << endl;

  idtr--;
  
  fOstream << idtr <<
    "}" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrParallelMusic" << endl;

  fOstream << idtr <<
    "<<" <<
    endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrParallelMusic" << endl;

  idtr--;
  
  fOstream << idtr <<
    endl << idtr << // JMI
    ">>" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewStaffgroupCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffgroupCommand" << endl;

   fOstream << idtr <<
     "\\new StaffGroup" << " " << " }" <<
      endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewStaffgroupCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffgroupCommand" << endl;

  idtr--;

  fOstream <<
    " }" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewStaffCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewStaffCommand" << endl;

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewStaffCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewStaffCommand" << endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrUseVoiceCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrUseVoiceCommand" << endl;

  fOstream << idtr <<
    endl << idtr << // JMI
     "\\context Voice" << " = " <<
    "\"" << elt->getVoice ()->getVoiceName () << "\""<<
    " " <<
     "{ "<< "\\" << elt->getVoice ()->getVoiceName () << " }" <<
    endl;

 // JMI \context Voice = "PartPOneVoiceOne" { \PartPOneVoiceOne }
 
  idtr++;
}
void lpsr2LilyPondVisitor::visitEnd (S_lpsrUseVoiceCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrUseVoiceCommand" << endl;

  idtr--;
}
  
//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_lpsrNewlyricsCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting lpsrNewlyricsCommand" << endl;

   fOstream << idtr <<
     "\\new Lyrics" << " " <<
    "\\lyricsto" << " " <<
    "\""  << elt->getVoice ()->getVoiceName () << "\""  <<
    " " <<
    "\\" << elt->getLyrics ()->getLyricsName () <<
    endl;

 // JMI \new Lyrics \lyricsto "PartPOneVoiceOne" \PartPOneVoiceOneLyricsOne

  idtr++;
}

void lpsr2LilyPondVisitor::visitEnd (S_lpsrNewlyricsCommand& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting lpsrNewlyricsCommand" << endl;

  idtr--;
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
void lpsr2LilyPondVisitor::visitStart (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrLyrics" << endl;

  fOstream << idtr <<
    elt->getLyricsName () << " = " << "\\lyricmode" << " {" << endl;
  idtr++;
  fOstream << idtr;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrLyrics" << endl;

  idtr--;
  
  fOstream <<
    idtr << "}" <<
    endl <<
    endl;  
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrLyricschunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrLyricschunk" << endl;

  switch (elt->getLyricschunkType ()) {
    case msrLyricschunk::kSingleChunk:
      fOstream <<
        "\"" << elt->getChunkText () << "\"" << " ";
      break;
      
    case msrLyricschunk::kBeginChunk:
      fOstream <<
        "\"" << elt->getChunkText () << "\"" << " ";
      break;
      
    case msrLyricschunk::kMiddleChunk:
      fOstream <<
        "-- " << "\"" << elt->getChunkText () << "\"" << " ";
      break;
      
    case msrLyricschunk::kEndChunk:
      fOstream <<
        "-- " << "\"" << elt->getChunkText () << "\"" << " ";
      break;
      
    case msrLyricschunk::kSkipChunk:
      fOstream <<
        "\\skip" << elt->getChunkDuration () << " ";
      break;
      
    case msrLyricschunk::kSlurChunk:
      break;
    case msrLyricschunk::kTiedChunk:
      break;
      
    case msrLyricschunk::kBreakChunk:
      fOstream <<
        "%{ " << "\"" << elt->getChunkText () << "\"" << " %}" <<
        endl <<
        idtr;
      break;

    case msrLyricschunk::k_NoChunk:
      break;
  } // switch
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
    "\\key " << elt->getTonic () << " ";

  switch (elt->getKeyMode ()) {
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
    "\\time" << " " <<
    elt->getRational ().getNumerator () <<
    "/" <<
    elt->getRational ().getDenominator () <<
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
}

void lpsr2LilyPondVisitor::visitEnd (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTempo" << endl;

  fOstream <<
    "\\tempo" << " " <<
    elt->getTempoUnit () <<
    " = " <<
    elt->getPerMinute () << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrSequentialMusic" << endl;

  fOstream << idtr <<
    "{ " << " % msrSequentialMusic" << endl;
  idtr++;
  fOstream << idtr;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrSequentialMusic" << endl;

  idtr--;
  
  fOstream <<
    endl <<
    idtr << "}" << " % msrSequentialMusic" <<
    endl;
}

//________________________________________________________________________
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

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrArticulation" << endl;

  switch (elt->getArticulationKind ()) {
    case msrArticulation::kStaccato:
      fOstream << "-.";
      break;
    case msrArticulation::kStaccatissimo:
      fOstream << "-^";
      break;
    case msrArticulation::kFermata:
      fOstream << "\\fermata";
      break;
  } // switch
  fOstream << " ";
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
      
  fOstream << elt->dynamicsKindAsLilypondString () << " ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrDynamics" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrWedge" << endl;

  switch (elt->getWedgeKind ()) {
    case msrWedge::kCrescendoWedge:
      fOstream << "\\<";
      break;
    case msrWedge::kDecrescendoWedge:
      fOstream << "\\>";
      break;
    case msrWedge::kStopWedge:
      fOstream << "\\!";
      break;
  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrWedge" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrNote& elt)
{
  /*

string msrNote::octaveRepresentation (char octave)
{
  stringstream s;
  if (octave > 0) {
    int n = octave;
    while (n > 0) {
    s << "'";
    n--;
    }
  } else if (octave < 0) {
    int n = octave;
    while (n < 0) {
      s << ",";
      n++;
    }  
  }
  return s.str();
}
  */

  if (fMsrOptions->fDebug) {
    fOstream << idtr <<
      "% --> Start visiting ";
    switch (elt->getNoteKind ()) {
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
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
    
    case msrNote::kStandaloneNote:
      // print the note name
      fOstream <<
        noteMsrPitchAsLilyPondString (elt);
      
      // print the note duration
      fOstream <<
        elt->getNoteMsrDuration ();
      break;
      
    case msrNote::kRestNote:
      // print the note name
      fOstream <<
        "r";
      
      // print the note duration
      fOstream <<
        elt->getNoteMsrDuration ();
      break;
      
    case msrNote::kChordMemberNote:
      // print the note name
      fOstream <<
        noteMsrPitchAsLilyPondString (elt);
      
      // don't print the note duration,
      // it will be printed for the chord itself
      break;
      
    case msrNote::kTupletMemberNote:
      // print the note name
      fOstream <<
        noteMsrPitchAsLilyPondString (elt);
      
      // print the note duration
      fOstream <<
        elt->getNoteMsrDuration ();
      break;
  } // switch

  fOstream << " ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrNote& elt)
{
  if (fMsrOptions->fDebug) {
    fOstream << idtr <<
      "% --> Start visiting ";
    switch (elt->getNoteKind ()) {
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
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
    case msrNote::kStandaloneNote:
      break;
    case msrNote::kRestNote:
      break;
    case msrNote::kChordMemberNote:
      break;
    case msrNote::kTupletMemberNote:
      break;
  } // switch

  switch (elt->getNoteSlurKind ()) {
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
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBeam" << endl;

  switch (elt->getBeamKind ()) {
    case msrBeam::kBeginBeam:
      fOstream << "[ ";
      break;
    case msrBeam::kContinueBeam:
      break;
    case msrBeam::kEndBeam:
      fOstream << "] ";
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

  fOstream << "<";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrChord" << endl;

  fOstream <<
    ">";
    elt->getChordDuration ();
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrTuplet" << endl;

  fOstream <<
    "\\tuplet " <<
    elt->getActualNotes () <<
    "/" <<
    elt->getNormalNotes() << " { ";
}

void lpsr2LilyPondVisitor::visitEnd (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrTuplet" << endl;

  fOstream << " }" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrSlur" << endl;
  
  switch (elt->getSlurKind ()) {
    case msrSlur::kStartSlur:
      fOstream << "( ";
      break;
    case msrSlur::kContinueSlur:
      break;
    case msrSlur::kStopSlur:
      fOstream << ") ";
      break;
    default:
      fOstream << "Slur" << elt->getSlurKind () << "###";
  } // switch
}

void lpsr2LilyPondVisitor::visitEnd (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrSlur" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBarLine& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBarLine" << endl;

  fOstream << "| % " << elt->getNextBarNumber () << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBarLine& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBarLine" << endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrBarNumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrBarNumberCheck" << endl;

  fOstream <<
    "\\barNumberCheck #" << elt->getNextBarNumber () <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrBarNumberCheck" << endl;
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
    endl;
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

  fOstream << idtr <<
    "\\repeat" << " " << "volta" << " " << "2" << " {" << // JMI
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrRepeat" << endl;

  fOstream << idtr <<
    "}" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilyPondVisitor::visitStart (S_msrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> Start visiting msrComment" << endl;

  fOstream << idtr <<
    "% " << elt->getContents () <<
    endl;
  if (elt->getGapKind () == msrComment::kGapAfterwards)
    fOstream << endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrComment" << endl;
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
    endl <<
    endl;
}

void lpsr2LilyPondVisitor::visitEnd (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "% --> End visiting msrMidi" << endl;
}


} // namespace
