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

#include "msr2Summary.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2SummaryVisitor::msr2SummaryVisitor (
  S_msrOptions& msrOpts,
  ostream&      fOstream)
    : fOstream (fOstream)
{
  fMsrOptions = msrOpts;

//  JMI gCurrentLocation.fMeasureNumber     = 0; // in case of an anacrusis
//  gCurrentLocation.fPositionInMeasure = 1;

  fOnGoingStaff = false;
  
  fScoreStandaloneNotesCounter = 0;
  fScoreRestNotesCounter       = 0;
  fScoreSkipNotesCounter       = 0;
  fScoreGraceNotesCounter      = 0;
  fScoreChordNotesCounter      = 0;
  fScoreTupletNotesCounter     = 0;
  fScoreChordsCounter          = 0;
};
  
msr2SummaryVisitor::~msr2SummaryVisitor ()
{}

//________________________________________________________________________
void msr2SummaryVisitor::printSummaryFromMsrScore (
  const S_msrScore& score)
{
  if (score) {    
    // create a msrScore browser
    msrBrowser<msrScore> browser (this);

    // browse the score with the browser
    browser.browse (*score);
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrScore& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrScore" << endl;

  int partgroupsListSize = elt->getPartgroupsList ().size();
  
  fOstream << idtr <<
    "Score contains " << partgroupsListSize;
  if (partgroupsListSize == 1)
    fOstream << " part group";
  else
    fOstream << " part groups";
  fOstream << endl << endl;

  idtr++;
}

void msr2SummaryVisitor::visitEnd (S_msrScore& elt)
{
  idtr--;

  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrScore" << endl;
  
  fOstream <<
    idtr << "The score contains:" << endl;

  idtr++;
  
  fOstream <<
    idtr <<
      fScoreStandaloneNotesCounter <<" standalone notes" <<
      endl <<
    idtr <<
      fScoreRestNotesCounter << " rest notes" <<
      endl <<
    idtr <<
      fScoreSkipNotesCounter << " skip notes" <<
      endl <<
    idtr <<
      fScoreGraceNotesCounter << " grace notes" <<
      endl <<
    idtr <<
      fScoreChordNotesCounter << " chord notes" <<
      endl <<
    idtr <<
      fScoreTupletNotesCounter << " tuplet notes" <<
      endl <<
    idtr <<
      fScoreChordsCounter << " chords" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPartgroup& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPartgroup" << endl;

  int partgroupElementsSize = elt->getPartgroupElements ().size();
  
  fOstream << idtr <<
    "Partgroup" << " " << elt->getPartgroupCombinedName () <<
    " contains " << partgroupElementsSize;
  if (partgroupElementsSize == 1)
    fOstream << " part or sub part group";
  else
    fOstream << " parts or sub part groups";
  fOstream << endl;

  idtr++;

  fOstream << left <<
    idtr <<
      setw(24) << "PartgroupName" << " : \"" <<
      elt->getPartgroupName  () << "\"" <<
      endl <<
    idtr <<
      setw(24) << "PartgroupAbbrevation" << " : \"" <<
      elt->getPartgroupAbbreviation  () << "\"" <<
      endl <<
    idtr <<
      setw(24) << "fPartgroupSymbolDefaultX" << " : " <<
      elt->getPartgroupSymbolDefaultX  () <<
      endl <<
    idtr <<
    setw(24) << "fPartgroupSymbolKind" << " : \"";
    
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
  
  fOstream << "\"" <<
    endl <<
    idtr <<
      setw(24) << "PartgroupBarline" << " : ";
      
  if (elt->getPartgroupBarline ())
    fOstream << "true";
  else
    fOstream << "false";
  fOstream <<
    endl << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPartgroup& elt)
{
  idtr--;

  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPartgroup" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPart& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPart" << endl;

  int partStavesMapSize = elt->getPartStavesMap ().size();
  
  fOstream << idtr <<
    "Part " << elt->getPartCombinedName () <<
    " contains " << partStavesMapSize;
  if (partStavesMapSize == 1)
    fOstream << " staff";
  else
    fOstream << " staves";
  fOstream << endl;
    
  idtr++;
  
  fOstream << left <<
    idtr <<
      setw(25) << "PartID" << " : \"" <<
      elt->getPartID () << "\"" <<
      endl <<
    idtr <<
      setw(25) << "PartMSRName" << " : \"" <<
      elt->getPartMSRName () << "\"" <<
      endl <<
    idtr <<
      setw(25) << "PartName" << " : \"" <<
      elt->getPartName  () << "\"" <<
      endl <<
    idtr <<
      setw(25) << "PartAbbrevation" << " : \"" <<
      elt->getPartAbbreviation () << "\"" <<
      endl <<
    idtr <<
      setw(25) << "PartDivisionsPerWholeNote" << " : " <<
      elt->getPartDivisionsPerWholeNote () <<
      endl <<
    idtr <<
      setw(25) << "PartInstrumentName" << " : \"" <<
      elt->getPartInstrumentName () << "\"" <<
      endl <<
    endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPart& elt)
{
  idtr--;

  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPart" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStaff& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrStaff" << endl;

  int staffVoicesMapSize = elt->getStaffVoicesMap ().size();
    
  fOstream << idtr <<
    "Staff" << " " << elt->getStaffName () <<
    " contains " << staffVoicesMapSize;
  if (staffVoicesMapSize == 1)
    fOstream << " voice";
  else
    fOstream << " voices";
  fOstream << endl;

  idtr++;
  
  if (elt->getStaffClef ())
    fOstream << idtr << elt->getStaffClef ();
  else
    fOstream << idtr << "NO_CLEF";
  fOstream << endl;

  if (elt->getStaffKey ())
    fOstream << idtr << elt->getStaffKey ();
  else
    fOstream << idtr << "NO_KEY";
  fOstream << endl;

  if (elt->getStaffTime ())
    fOstream << idtr << elt->getStaffTime ();
  else
    fOstream << idtr << "NO_TIME";
  fOstream << endl;

/* JMI
  fOstream <<
    idtr << "StaffInstrumentName: \"" <<
    elt->getStaffInstrumentName () << "\"" << endl;
*/

  fOstream << endl;

  fOnGoingStaff = true;
}

void msr2SummaryVisitor::visitEnd (S_msrStaff& elt)
{
  idtr--;

  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrStaff" << endl;

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVoice& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrVoice" << endl;

  int voiceStanzasMapSize = elt->getVoiceStanzasMap ().size();

  fOstream << idtr <<
    "Voice" << " " << elt->getVoiceName () <<
    " has " << voiceStanzasMapSize;
  if (voiceStanzasMapSize == 1)
    fOstream << " lyric";
  else
    fOstream << " lyrics";
  fOstream << endl << endl;

  idtr++;

  // don't show fVoiceMasterStanza in the summary
}

void msr2SummaryVisitor::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrVoice" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStanza& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrStanza" << endl;

  int syllablesSize = elt->getSyllables ().size();

  fOstream << idtr <<
    "Stanza" << " " << elt->getStanzaName () <<
    " contains " << syllablesSize;
  if (syllablesSize == 1)
    fOstream << " syllable";
  else
    fOstream << " syllables";

  if (! elt->getStanzaTextPresent ())
    fOstream << " (No actual text)";
    
  fOstream << endl << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrStanza& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrStanza" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSyllable& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrSyllable" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSyllable& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrSyllable" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrClef& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrClef" << endl;

  if (! fOnGoingStaff) {
    fOstream << idtr <<
      elt->clefAsString ();
  }
}

void msr2SummaryVisitor::visitEnd (S_msrClef& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrClef" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrKey& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrKey" << endl;

  if (! fOnGoingStaff) {
    fOstream << idtr <<
      "Key " << elt->getKeyTonic () << " ";
    if (elt->getKeyModeKind () == msrKey::kMajor)
      fOstream << "\\major";
    else
      fOstream << "\\minor";
  }
}

void msr2SummaryVisitor::visitEnd (S_msrKey& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrKey" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTime& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTime" << endl;

  if (! fOnGoingStaff) {    
    fOstream << idtr <<
      "Time " << 
      elt->getBeatsNumber () << "/" << elt->getBeatsValue ();
  }
}

void msr2SummaryVisitor::visitEnd (S_msrTime& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTime" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTempo& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTempo" << endl;

  fOstream << idtr <<
    "Tempo" << " " <<
    elt->getTempoUnit () << " " << elt->getPerMinute () <<
    endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTempo& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTempo" << endl;
}

//________________________________________________________________________
/*
void msr2SummaryVisitor::visitStart (S_msrSequentialMusic& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrSequentialMusic" << endl;

  int sequenceElementsSize =
    elt->getSequentialMusicElements ().size();

  fOstream << idtr <<
    "SequentialMusic" <<
    " contains " << sequenceElementsSize;
  if (sequenceElementsSize == 1)
    fOstream << " element";
  else
    fOstream << " elements";
  
  if (! sequenceElementsSize)
    fOstream << " (No actual notes)";
  fOstream << endl << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSequentialMusic& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrSequentialMusic" << endl;
}
*/
//________________________________________________________________________

void msr2SummaryVisitor::visitStart (S_msrSegment& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrSegment" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSegment& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrSegment" << endl;
}

//________________________________________________________________________
/*
void msr2SummaryVisitor::visitStart (S_msrDuration& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrDuration" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrDuration& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrDuration" << endl;
}
*/

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrArticulation& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrArticulation" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrArticulation& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrArticulation" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrDynamics& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrDynamics" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrDynamics& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrDynamics" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrWedge& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrWedge" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrWedge& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrWedge" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrGracenotes& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrGracenotes" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrGracenotes& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrGracenotes" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrNote& elt)
{
  if (gGeneralOptions->fDebug) {
    fOstream << idtr <<
      "--> Start visiting ";
    switch (elt->getNoteKind ()) {
      case msrNote::k_NoNoteKind:
        break;
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
        break;
      case msrNote::kRestNote:
        fOstream << "rest";
        break;
      case msrNote::kSkipNote:
        fOstream << "skip";
        break;
      case msrNote::kGraceNote:
        fOstream << "grace";
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
      fScoreStandaloneNotesCounter++;
      break;
    case msrNote::kRestNote:
      fScoreRestNotesCounter++;
      break;
    case msrNote::kSkipNote:
      fScoreSkipNotesCounter++;
      break;
    case msrNote::kGraceNote:
      fScoreGraceNotesCounter++;
      break;
    case msrNote::kChordMemberNote:
      fScoreChordNotesCounter++;
      break;
    case msrNote::kTupletMemberNote:
      fScoreTupletNotesCounter++;
      break;
  } // switch
}

void msr2SummaryVisitor::visitEnd (S_msrNote& elt)
{
  if (gGeneralOptions->fDebug) {
    fOstream << idtr <<
      "--> Start visiting ";
    switch (elt->getNoteKind ()) {
      case msrNote::k_NoNoteKind:
        break;
      case msrNote::kStandaloneNote:
        fOstream << "standalone";
        break;
      case msrNote::kRestNote:
        fOstream << "rest";
        break;
      case msrNote::kSkipNote:
        fOstream << "skip";
        break;
      case msrNote::kGraceNote:
        fOstream << "grace";
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

  switch (elt->getNoteKind ()) { // JMI
    case msrNote::k_NoNoteKind:
      break;
    case msrNote::kStandaloneNote:
      break;
    case msrNote::kRestNote:
      break;
    case msrNote::kSkipNote:
      break;
    case msrNote::kGraceNote:
      break;
    case msrNote::kChordMemberNote:
      break;
    case msrNote::kTupletMemberNote:
      break;
  } // switch
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStem& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrStem" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrStem& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrStem" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBeam& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBeam" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBeam& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBeam" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrChord& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrChord" << endl;

  fScoreChordsCounter++;
}

void msr2SummaryVisitor::visitEnd (S_msrChord& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrChord" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTuplet& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrTuplet" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTuplet& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrTuplet" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSlur& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrSlur" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSlur& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrSlur" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarline& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarline" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBarline& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarline" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarnumberCheck& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBarnumberCheck" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBarnumberCheck& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBarnumberCheck" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBreak& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrBreak" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBreak& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrBreak" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrRepeat& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrRepeat" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrRepeat& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrRepeat" << endl;
}

//________________________________________________________________________
/* JMI
void msr2SummaryVisitor::visitStart (S_msrComment& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrComment" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrComment& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrComment" << endl;
}
*/

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVarValAssoc& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrVarValAssoc" << endl;

  fOstream << idtr <<
    "VarValAssoc" << endl;
  
  idtr++;
  
  fOstream <<
    idtr << "variable name : " <<
    "\"" << elt->getVariableName () << "\"" << endl <<
    idtr << "variable value: " <<
    "\"" << elt->getVariableValue () << "\"" << endl;
  
  idtr--;
}

void msr2SummaryVisitor::visitEnd (S_msrVarValAssoc& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrVarValAssoc" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPageGeometry& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrPageGeometry" << endl;

  fOstream <<
    idtr << elt;
}

void msr2SummaryVisitor::visitEnd (S_msrPageGeometry& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrPageGeometry" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLayout& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrLayout" << endl;

  fOstream << "Layout" << endl;

  idtr++;
}

void msr2SummaryVisitor::visitEnd (S_msrLayout& elt)
{
  idtr--;

  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrLayout" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrMidi& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> Start visiting msrMidi" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrMidi& elt)
{
  if (gGeneralOptions->fDebug)
    fOstream << idtr <<
      "--> End visiting msrMidi" << endl;
}


} // namespace
