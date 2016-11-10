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

#include "msr.h"
#include "msr2SummaryVisitor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2SummaryVisitor::msr2SummaryVisitor (
  S_msrOptions& msrOpts,
  ostream&      fOstream)
    : fOstream (fOstream)
  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
{
  fMsrOptions = msrOpts;

  /* JMI
    : gCurrentMusicXMLLocation (
      0, // before first line on MusicXML contents
      0, // in case of an anacrusis
      0) // at the beginning of a measure
      */
  gCurrentMusicXMLLocation.fInputLineNumber = 0;
  gCurrentMusicXMLLocation.fMeasureNumber = 0; // in case of an anacrusis
  gCurrentMusicXMLLocation.fPositionInMeasure = 1;

  fScoreStandaloneNotesCounter = 0;
  fScoreRestNotesCounter = 0;
  fScoreChordNotesCounter = 0;
  fScoreTupletNotesCounter = 0;
  fScoreChordsCounter = 0;
};
  
msr2SummaryVisitor::~msr2SummaryVisitor ()
{}

//________________________________________________________________________
void msr2SummaryVisitor::printSummaryFromMsrScore (
  const S_msrScore& score)
{
  if (score) {    
    // create a msrScore browser
 // JMI   msrBrowser<msrElement> browser (this);
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*score);
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrScore& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrScore" << endl;

  int partgroupsListSize = elt->getPartgroupsList ().size();
  
  fOstream <<
    "Score" <<
    " contains " << partgroupsListSize;
  if (partgroupsListSize == 1)
    fOstream << " part group";
  else
    fOstream << " part groups";
  fOstream << endl;

  idtr++;
}

void msr2SummaryVisitor::visitEnd (S_msrScore& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrScore" << endl;
  
  fOstream <<
    idtr << "The score contains" << endl;

  idtr++;
  
  fOstream <<
    idtr <<
      fScoreStandaloneNotesCounter <<" standalone notes" << endl <<
    idtr <<
      fScoreRestNotesCounter << " rest notes" << endl <<
    idtr <<
      fScoreChordNotesCounter << " chord notes" << endl <<
    idtr <<
      fScoreTupletNotesCounter << " tuplet notes" << endl <<
    idtr <<
      fScoreChordsCounter << " chords" << endl;

  idtr--;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPartgroup& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPartgroup" << endl;

  int partgroupElementsSize = elt->getPartgroupElements ().size();
  
  fOstream <<
    "Partgroup" << " " << elt->getPartgroupCombinedName () <<
    " contains " << partgroupElementsSize;
  if (partgroupElementsSize == 1)
    fOstream << " part or sub part group";
  else
    fOstream << " parts or sub part groups";
  fOstream << endl;

  idtr++;

  fOstream <<
    idtr << "PartgroupName            : \"" <<
     elt->getPartgroupName  () << "\"" << endl <<
    idtr << "PartgroupAbbrevation     : \"" <<
     elt->getPartgroupAbbreviation  () << "\"" << endl;
  fOstream <<
    idtr << "fPartgroupSymbolDefaultX : " <<
     elt->getPartgroupSymbolDefaultX  () << endl;
  fOstream <<
    idtr << "fPartgroupSymbolKind     : \"";
  switch (elt->getPartgroupSymbolKind ()) {
    case msrPartgroup::kBracePartgroupSymbol:
      fOstream << "brace";
      break;
    case msrPartgroup::kBracketPartgroupSymbol:
      fOstream << "bracket";
      break;
    case msrPartgroup::kLinePartgroupSymbol:
      fOstream << "line";
      break;
    case msrPartgroup::kSquarePartgroupSymbol:
      fOstream << "square";
      break;
    case msrPartgroup::k_NoPartgroupSymbol:
      break;
  } // switch
  fOstream << "\"" << endl;
  fOstream <<
    idtr << "PartgroupBarline         : ";
  if (elt->getPartgroupBarline ())
    fOstream << "true";
  else
    fOstream << "false";
  fOstream << endl;

}

void msr2SummaryVisitor::visitEnd (S_msrPartgroup& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPartgroup" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPart& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPart" << endl;

  int partStavesMapSize = elt->getPartStavesMap ().size();
  
  fOstream <<
    "Part " << elt->getPartCombinedName () <<
    " contains " << partStavesMapSize;
  if (partStavesMapSize == 1)
    fOstream << " staff";
  else
    fOstream << " staves";
  fOstream << endl;
    
  idtr++;
  
  fOstream <<
    idtr <<
      "PartMusicXMLName  : \"" <<
      elt->getPartMusicXMLName () << "\"" << endl <<
    idtr <<
      "PartAbbrevation   : \"" <<
      elt->getPartAbbreviation () << "\"" << endl;
  
  fOstream << idtr <<
    "fPartMusicXMLDivisions: " <<
    elt->getPartMusicXMLDivisions () << endl;

  fOstream << idtr <<
    "PartInstrumentName: \"" <<
    elt->getPartInstrumentName () << "\"" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPart& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPart" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStaff& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrStaff" << endl;

  int staffVoicesMapSize = elt->getStaffVoicesMap ().size();
    
  fOstream << "Staff" << " " << elt->getStaffName () <<
    " contains " << staffVoicesMapSize;
  if (staffVoicesMapSize == 1)
    fOstream << " voice";
  else
    fOstream << " voices";
  fOstream << endl;

  idtr++;
/*
  if (fStaffClef)
    fOstream << idtr << elt->getStaffClef ();
  else
    fOstream << idtr << "NO_CLEF" << endl;
*/
  if (fStaffKey)
    fOstream << idtr << elt->getStaffKey ();
  else
    fOstream << idtr << "NO_KEY";
  fOstream << endl;
/*
  if (fStaffTime)
    fOstream << idtr << elt->getStaffTime ();
  else
    fOstream << idtr << "NO_TIME" << endl;
*/
  fOstream <<
    idtr << "StaffInstrumentName: \"" <<
    elt->getStaffInstrumentName () << "\"" << endl;

  fOstream << endl;
  }

void msr2SummaryVisitor::visitEnd (S_msrStaff& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrStaff" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrClef" << endl;

  fOstream <<
    "Clef" << " \"" << fSign << "\"" <<
    " line " << fLine << ", " << fOctaveChange << "*8";
}

void msr2SummaryVisitor::visitEnd (S_msrClef& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrClef" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrKey" << endl;

  fOstream << "Key " << fTonic << " ";
  if (fKeyMode == kMajor) fOstream << "\\major";
  else fOstream << "\\minor";
}

void msr2SummaryVisitor::visitEnd (S_msrKey& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrKey" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTime" << endl;

  fOstream <<
    "Time " << 
    fRational.getNumerator() << "/" << fRational.getDenominator();
}

void msr2SummaryVisitor::visitEnd (S_msrTime& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTime" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTempo" << endl;

  fOstream <<
    "Tempo" << " " <<
    fTempoUnit << " " << fPerMinute << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTempo& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTempo" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVoice& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrVoice" << endl;

  int voiceLyricsMapSize = fVoiceLyricsMap.size();

  fOstream <<
    "Voice" << " " << getVoiceName () <<
    " has " << voiceLyricsMapSize;
  if (voiceLyricsMapSize == 1)
    fOstream << " lyric";
  else
    fOstream << " lyrics";
  fOstream << endl;

  idtr++;

  fOstream << idtr << fVoiceSequentialMusic;

  // don't show the voice master lyrics in summary
}

void msr2SummaryVisitor::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrVoice" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrLyrics" << endl;

  int lyricsChunksSize = fLyricsChunks.size();

  fOstream << "Lyrics" << " " << getLyricsName () <<
    " contains " << lyricsChunksSize;
  if (lyricsChunksSize == 1)
    fOstream << " chunk";
  else
    fOstream << " chunks";

  if (! fLyricsTextPresent)
    fOstream << " (No actual text)";
    
  fOstream << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrLyrics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrLyrics" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLyricsChunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrLyricsChunk" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrLyricsChunk& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrLyricsChunk" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrSequentialMusic" << endl;

  int sequenceElementsSize =
    elt->getSequentialMusicElements ().size();

  fOstream <<
    "SequentialMusic" <<
    " contains " << sequenceElementsSize;
  if (sequenceElementsSize == 1)
    fOstream << " element";
  else
    fOstream << " elements";
  
  if (! sequenceElementsSize)
    fOstream << " (No actual notes)";
  fOstream << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSequentialMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrSequentialMusic" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrParallelMusic" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrParallelMusic& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrParallelMusic" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrDuration" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrDuration& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrDuration" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrArticulation" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrArticulation& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrArticulation" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrDynamics" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrDynamics& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrDynamics" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrWedge" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrWedge& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrWedge" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrNote& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting ";

  switch (elt->getNoteKind ()) {
    case msrNote::kStandaloneNote:
      fOstream << "standalone";
      fScoreStandaloneNotesCounter++;
      break;
    case msrNote::kRestNote:
      fOstream << "rest";
      fScoreRestNotesCounter++;
      break;
    case msrNote::kChordMemberNote:
      fOstream << "chord member";
      fScoreChordNotesCounter++;
      break;
    case msrNote::kTupletMemberNote:
      fOstream << "tuplet member";
      fScoreTupletNotesCounter++;
      break;
  } // switch
  fOstream << " msrNote" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrNote& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting ";

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

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBeam" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBeam& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBeam" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrChord" << endl;

  fScoreChordsCounter++;
}

void msr2SummaryVisitor::visitEnd (S_msrChord& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrChord" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTuplet" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTuplet& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTuplet" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrSlur" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSlur& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrSlur" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarLine& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarLine" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBarLine& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarLine" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarNumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarNumberCheck" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarNumberCheck" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBreak" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBreak& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBreak" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrRepeat" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrRepeat& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrRepeat" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrComment" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrComment& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrComment" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrVarValAssoc" << endl;

  fOstream << "LilypondVarValAssoc" << endl;
  
  idtr++;
  
  fOstream << idtr << fVariableName << endl;
  fOstream << idtr << fVariableValue <<endl;
  
  idtr--;
}

void msr2SummaryVisitor::visitEnd (S_msrVarValAssoc& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrVarValAssoc" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrHeader& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrHeader" << endl;

  fOstream << "Header" << endl;

  idtr++;
  
  if (fWorkNumber) {
    fOstream << idtr << fWorkNumber;
  }
  
  if (fWorkTitle) {
    fOstream << idtr << fWorkTitle;
  }
    
  if (fMovementNumber) {
    fOstream << idtr << fMovementNumber;
  }
    
  if (fMovementTitle) {
    fOstream << idtr << fMovementTitle;
  }
    
  if (!fCreators.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i1;
    for (i1=fCreators.begin(); i1!=fCreators.end(); i1++) {
      fOstream << idtr << (*i1);
    } // for
  }
    
  if (fRights) {
    fOstream << idtr << fRights;
  }
    
  if (!fSoftwares.empty()) {
    vector<S_msrVarValAssoc>::const_iterator i2;
    for (i2=fSoftwares.begin(); i2!=fSoftwares.end(); i2++) {
      fOstream << idtr << (*i2);
    } // for
  }
    
  if (fEncodingDate) {
    fOstream << idtr << fEncodingDate;
  }
  
  idtr--;
}

void msr2SummaryVisitor::visitEnd (S_msrHeader& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrHeader" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPaper& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPaper" << endl;

  fOstream << "Paper" << endl;

  idtr++;
  
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

  idtr--;
}

void msr2SummaryVisitor::visitEnd (S_msrPaper& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPaper" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLayout& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrLayout" << endl;

  fOstream << "Layout" << endl;

  idtr++;
}

void msr2SummaryVisitor::visitEnd (S_msrLayout& elt)
{
  idtr--;

  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrLayout" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrMidi" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrMidi& elt)
{
  if (fMsrOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrMidi" << endl;
}


} // namespace
