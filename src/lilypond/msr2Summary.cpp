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

#include <iomanip>      // setw, set::precision, ...

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

  // staves
  fOnGoingStaff = false;
  
  // notes
  fScoreStandaloneNotesCounter    = 0;
  fScoreRestNotesCounter          = 0;
  fScoreSkipNotesCounter          = 0;
  fScoreDoubleTremoloNotesCounter = 0;
  fScoreGraceNotesCounter         = 0;
  
  // chords
  fScoreChordNotesCounter         = 0;
  fScoreChordsCounter             = 0;

  // tuplets
  fScoreTupletNotesCounter        = 0;
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
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrScore" <<
      endl;

  int partGroupsListSize = elt->getPartGroupsList ().size();
  
  fOstream << idtr <<
    "Score contains " <<
    singularOrPlural (
      partGroupsListSize, "part group", "part groups") <<
    endl <<
    endl;

  idtr++;
}

void msr2SummaryVisitor::visitEnd (S_msrScore& elt)
{
  idtr--;

  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrScore" <<
      endl;
  
  fOstream <<
    idtr << "The score contains:" <<
    endl;

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
      fScoreDoubleTremoloNotesCounter << " double tremolo notes" <<
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
void msr2SummaryVisitor::visitStart (S_msrPartGroup& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrPartGroup" <<
      endl;

  int partGroupElementsSize = elt->getPartGroupElements ().size();
  
  fOstream << idtr <<
    "PartGroup" << " " << elt->getPartGroupCombinedName () <<
    " contains " <<
    singularOrPlural (
      partGroupElementsSize,
      " part or sub part group",
      " parts or sub part groups") <<
    endl;

  idtr++;

  const int fieldWidth = 24;
  
  fOstream << left <<
    idtr <<
      setw(fieldWidth) <<
      "PartGroupAbsoluteNumber" << " : \"" <<
      elt->getPartGroupAbsoluteNumber  () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartGroupName" << " : \"" <<
      elt->getPartGroupName  () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartGroupDisplayText" << " : \"" <<
      elt->getPartGroupDisplayText  () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartGroupAccidentalText" << " : \"" <<
      elt->getPartGroupAccidentalText  () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartGroupAbbrevation" << " : \"" <<
      elt->getPartGroupAbbreviation  () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartGroupSymbolKind" << " : " <<
      msrPartGroup::partGroupSymbolKindAsString (
        elt->getPartGroupSymbolKind  ()) <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartGroupSymbolDefaultX" << " : " <<
      elt->getPartGroupSymbolDefaultX  () <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fPartGroupBarline" << " : \"" <<
      booleanAsString (
        elt->getPartGroupBarline ()) <<
      "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartGroupInstrumentName" << " : \"" <<
        elt->getPartGroupInstrumentName () << "\"" <<
      endl <<
    endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPartGroup& elt)
{
  idtr--;

  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrPartGroup" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPart& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrPart" <<
      endl;

  int partStavesMapSize = elt->getPartStavesMap ().size();
  
  fOstream << idtr <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    singularOrPlural (
      partStavesMapSize, "staff", "staves") <<
    endl;
    
  idtr++;
  
  const int fieldWidth = 28;
  
  fOstream << left <<
    idtr <<
      setw(fieldWidth) <<
      "PartID" << " : \"" <<
      elt->getPartID () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartMsrName" << " : \"" <<
      elt->getPartMsrName () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartName" << " : \"" <<
      elt->getPartName  () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartAbbrevation" << " : \"" <<
      elt->getPartAbbreviation () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartDivisionsPerQuarterNote" << " : " <<
      elt->getPartDivisionsPerQuarterNote () <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartInstrumentName" << " : \"" <<
      elt->getPartInstrumentName () << "\"" <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "PartInstrumentAbbreviation" << " : \"" <<
      elt->getPartInstrumentAbbreviation () << "\"" <<
      endl;

  fOstream <<
    endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPart& elt)
{
  idtr--;

  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrPart" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStaff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrStaff" <<
      endl;

  int staffAllVoicesMapSize = elt->getStaffAllVoicesMap ().size();
    
  fOstream << idtr <<
    "Staff" << " " << elt->getStaffName () <<
    " contains " <<
    singularOrPlural (
      staffAllVoicesMapSize, "voice", "voices") <<
    endl;

  idtr++;

  
  const int fieldWidth = 17;

  // print the staff number
  fOstream << left <<
    idtr <<
      setw(fieldWidth) <<
      "StaffNumber" << " : " <<
      elt->getStaffNumber () <<
      endl;

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

  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrStaff" <<
      endl;

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVoice& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrVoice" <<
      endl;

  int voiceStanzasMapSize = elt->getVoiceStanzasMap ().size();

  fOstream << idtr <<
    "Voice" << " " << elt->getVoiceName () <<
    " has " <<
    singularOrPlural (
      voiceStanzasMapSize, "stanza", "stanzas") <<
    endl;

  idtr++;

  const int fieldWidth = 30;
  
  fOstream << left <<
    idtr <<
      setw(fieldWidth) <<
      "VoiceAbsoluteNumber" << " : " <<
      elt->getVoiceAbsoluteNumber () <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "ExternalVoiceNumber" << " : " <<
      elt->getExternalVoiceNumber () <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "StaffRelativeVoiceNumber" << " : " <<
      elt->getStaffRelativeVoiceNumber () <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "VoiceActualNotesCounter" << " : " <<
      elt->getVoiceActualNotesCounter () <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "VoiceActualHarmoniesCounter" << " : " <<
      elt->getVoiceActualHarmoniesCounter () <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "MusicHasBeenInsertedInVoice" << " : " <<
      booleanAsString (
        elt->getMusicHasBeenInsertedInVoice ()) <<
    endl;

  fOstream <<
    endl;

  // don't show fVoiceMasterStanza in the summary
}

void msr2SummaryVisitor::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrVoice" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStanza& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrStanza" <<
      endl;

  int syllablesSize = elt->getSyllables ().size();

  fOstream << idtr <<
    "Stanza" << " " << elt->getStanzaName () <<
    " contains " <<
    singularOrPlural (
      syllablesSize, "syllable", "syllables");

  if (! elt->getStanzaTextPresent ())
    fOstream << " (No actual text)";
    
  fOstream << endl << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrStanza& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrStanza" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSyllable& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrSyllable" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSyllable& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrSyllable" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrClef& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrClef" <<
      endl;

  if (! fOnGoingStaff) {
    fOstream <<
      idtr <<
      elt->clefAsString () <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrClef& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrClef" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrKey& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrKey" <<
      endl;

  if (! fOnGoingStaff) {
    fOstream <<
      idtr <<
      elt->keyAsString () <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrKey& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrKey" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTime& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTime" <<
      endl;

  if (! fOnGoingStaff) {    
    fOstream <<
      idtr <<
      elt->timeAsString () <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrTime& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTime" <<
    endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTempo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTempo" <<
       endl;

  fOstream << idtr <<
    "Tempo" << " " <<
    elt->getTempoUnit () << " " << elt->getPerMinute () <<
    endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTempo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTempo" <<
    endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSegment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrSegment" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSegment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrSegment" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrArticulation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrArticulation" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrArticulation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrArticulation" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrDynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrDynamics" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrDynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrDynamics" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrWedge& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrWedge" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrWedge& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrWedge" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrGraceNotes& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrGraceNotes" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrGraceNotes& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrGraceNotes" << endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrNote& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fOstream << idtr <<
      "--> Start visiting " <<
      msrNote::noteKindAsString (
        elt->getNoteKind ()) <<
      " msrNote" <<
      endl;
  }

  switch (elt->getNoteKind ()) {
    case msrNote::k_NoNoteKind:
      break;
    case msrNote::kRestNote:
      fScoreRestNotesCounter++;
      break;
    case msrNote::kSkipNote:
      fScoreSkipNotesCounter++;
      break;
    case msrNote::kStandaloneNote:
      fScoreStandaloneNotesCounter++;
      break;
    case msrNote::kDoubleTremoloMemberNote:
      fScoreStandaloneNotesCounter++;
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
  if (gMsrOptions->fTraceMsrVisitors) {
    fOstream << idtr <<
      "--> End visiting " <<
      msrNote::noteKindAsString (
        elt->getNoteKind ()) <<
      " msrNote" <<
      endl;
  }

  switch (elt->getNoteKind ()) { // JMI
    case msrNote::k_NoNoteKind:
      break;
    case msrNote::kRestNote:
      break;
    case msrNote::kSkipNote:
      break;
    case msrNote::kStandaloneNote:
      break;
    case msrNote::kDoubleTremoloMemberNote:
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
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrStem" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrStem& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrStem" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBeam& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBeam" << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBeam& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBeam" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrChord& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrChord" <<
      endl;

  fScoreChordsCounter++;
}

void msr2SummaryVisitor::visitEnd (S_msrChord& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrChord" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTuplet& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrTuplet" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTuplet& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrTuplet" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSlur& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrSlur" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrSlur& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrSlur" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarline& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBarline" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBarline& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBarline" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarNumberCheck& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBarNumberCheck" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBarNumberCheck" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBreak& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrBreak" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrBreak& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrBreak" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrRepeat& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrRepeat" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrRepeat& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrRepeat" <<
      endl;
}

//________________________________________________________________________
/* JMI
void msr2SummaryVisitor::visitStart (S_msrComment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrComment" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrComment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrComment" <<
      endl;
}
*/

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVarValAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrVarValAssoc" <<
      endl;

  fOstream << idtr <<
    "VarValAssoc" <<
    endl;
  
  idtr++;
  
  fOstream <<
    idtr <<
      "variable name : " <<
      "\"" << elt->getVariableName () << "\"" <<
      endl <<
    idtr <<
      "variable value: " <<
      "\"" << elt->getVariableValue () << "\"" <<
      endl <<
    endl;
  
  idtr--;
}

void msr2SummaryVisitor::visitEnd (S_msrVarValAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrVarValAssoc" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPageGeometry& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrPageGeometry" <<
      endl;

  fOstream <<
    idtr << elt;
}

void msr2SummaryVisitor::visitEnd (S_msrPageGeometry& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrPageGeometry" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLayout& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrLayout" <<
      endl;

  fOstream << "Layout" << endl;

  idtr++;
}

void msr2SummaryVisitor::visitEnd (S_msrLayout& elt)
{
  idtr--;

  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrLayout" <<
      endl;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrMidi& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> Start visiting msrMidi" <<
      endl;
}

void msr2SummaryVisitor::visitEnd (S_msrMidi& elt)
{
  if (gMsrOptions->fTraceMsrVisitors)
    fOstream << idtr <<
      "--> End visiting msrMidi" <<
      endl;
}


} // namespace
