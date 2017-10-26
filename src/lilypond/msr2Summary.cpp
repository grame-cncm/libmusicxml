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

#include "msrOptions.h"

#include "msr2Summary.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2SummaryVisitor::msr2SummaryVisitor (
  S_msrOptions&         msrOpts,
  indentedOstream& ios)
    : fMsrSummaryOutputStream (ios)
{
  fMsrOptions = msrOpts;

  // part groups
  fPartGroupsCounter = 0;

  // parts
  fPartsCounter = 0;

  // staves
  fStavesCounter = 0;
  fOnGoingStaff = false;

  // voices
  fVoicesCounter = 0;

  // stanzas
  fStanzasCounter = 0;
  
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
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrScore" <<
      endl;
  }

  int partGroupsListSize = elt->getPartGroupsList ().size();
  
  fMsrSummaryOutputStream <<
    "Score contains " <<
    singularOrPlural (
      partGroupsListSize, "part group", "part groups") <<
    endl <<
    endl;

  gIndenter++;
}

void msr2SummaryVisitor::visitEnd (S_msrScore& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrScore" <<
      endl;
  }
  
  fMsrSummaryOutputStream <<
    "The score contains:" <<
    endl;

  gIndenter++;
  
  fMsrSummaryOutputStream <<
    fPartGroupsCounter <<" part groups" <<
    endl <<
    fPartsCounter <<" parts" <<
    endl <<
    fStavesCounter <<" staves" <<
    endl <<
    fVoicesCounter <<" voices" <<
    endl <<
    fStanzasCounter <<" stanzas" <<
    endl <<

    fScoreStandaloneNotesCounter <<" standalone notes" <<
    endl <<
    fScoreRestNotesCounter << " rest notes" <<
    endl <<
    fScoreSkipNotesCounter << " skip notes" <<
    endl <<
    fScoreDoubleTremoloNotesCounter << " double tremolo notes" <<
    endl <<
    fScoreGraceNotesCounter << " grace notes" <<
    endl <<
    fScoreChordNotesCounter << " chord notes" <<
    endl <<
    fScoreTupletNotesCounter << " tuplet notes" <<
    endl <<
    fScoreChordsCounter << " chords" <<
    endl;

  gIndenter--;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPartGroup& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrPartGroup" <<
      endl;
  }

  fPartGroupsCounter++;

  int partGroupElementsSize = elt->getPartGroupElements ().size();
  
  fMsrSummaryOutputStream <<
    "PartGroup" << " " << elt->getPartGroupCombinedName () <<
    " contains " <<
    singularOrPlural (
      partGroupElementsSize,
      " part or sub part group",
      " parts or sub part groups") <<
    endl;

  gIndenter++;

  const int fieldWidth = 24;
  
  fMsrSummaryOutputStream << left <<
    setw (fieldWidth) <<
    "PartGroupAbsoluteNumber" << " : \"" <<
    elt->getPartGroupAbsoluteNumber  () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupName" << " : \"" <<
    elt->getPartGroupName  () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupDisplayText" << " : \"" <<
    elt->getPartGroupDisplayText  () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupAccidentalText" << " : \"" <<
    elt->getPartGroupAccidentalText  () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupAbbrevation" << " : \"" <<
    elt->getPartGroupAbbreviation  () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupSymbolKind" << " : " <<
    msrPartGroup::partGroupSymbolKindAsString (
      elt->getPartGroupSymbolKind  ()) <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupSymbolDefaultX" << " : " <<
    elt->getPartGroupSymbolDefaultX  () <<
    endl <<
    setw (fieldWidth) <<
    "fPartGroupBarline" << " : \"" <<
    msrPartGroup::partGroupBarlineKindAsString (
      elt->getPartGroupBarlineKind ()) <<
    "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartGroupInstrumentName" << " : \"" <<
      elt->getPartGroupInstrumentName () << "\"" <<
    endl <<
    endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPartGroup& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrPartGroup" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPart& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrPart" <<
      endl;
  }

  fPartsCounter++;
  
  int partStavesMapSize = elt->getPartStavesMap ().size();
  
  fMsrSummaryOutputStream <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    singularOrPlural (
      partStavesMapSize, "staff", "staves") <<
    endl;
    
  gIndenter++;
  
  const int fieldWidth = 28;
  
  fMsrSummaryOutputStream << left <<
    setw (fieldWidth) <<
    "PartID" << " : \"" <<
    elt->getPartID () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartMsrName" << " : \"" <<
    elt->getPartMsrName () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartName" << " : \"" <<
    elt->getPartName  () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartAbbrevation" << " : \"" <<
    elt->getPartAbbreviation () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartInstrumentName" << " : \"" <<
    elt->getPartInstrumentName () << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "PartInstrumentAbbreviation" << " : \"" <<
    elt->getPartInstrumentAbbreviation () << "\"" <<
    endl;

  fMsrSummaryOutputStream <<
    endl;
}

void msr2SummaryVisitor::visitEnd (S_msrPart& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrPart" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStaff& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrStaff" <<
      endl;
  }

  fStavesCounter++;
  
  int staffAllVoicesMapSize = elt->getStaffAllVoicesMap ().size();
    
  fMsrSummaryOutputStream <<
    "Staff" << " " << elt->getStaffName () <<
    " contains " <<
    singularOrPlural (
      staffAllVoicesMapSize, "voice", "voices") <<
    endl;

  gIndenter++;

  
  const int fieldWidth = 17;

  // print the staff number
  fMsrSummaryOutputStream << left <<
    setw (fieldWidth) <<
    "StaffNumber" << " : " <<
    elt->getStaffNumber () <<
    endl;

/* JMI
  fMsrSummaryOutputStream <<
    "StaffInstrumentName: \"" <<
    elt->getStaffInstrumentName () << "\"" << endl;
*/

  fMsrSummaryOutputStream <<
    endl;

  fOnGoingStaff = true;
}

void msr2SummaryVisitor::visitEnd (S_msrStaff& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrStaff" <<
      endl;
  }

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVoice& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrVoice" <<
      endl;
  }

  fVoicesCounter++;

  int voiceStanzasMapSize = elt->getVoiceStanzasMap ().size();

  fMsrSummaryOutputStream <<
    "Voice" << " " << elt->getVoiceName () <<
    " has " <<
    singularOrPlural (
      voiceStanzasMapSize, "stanza", "stanzas") <<
    endl;

  gIndenter++;

  const int fieldWidth = 30;
  
  fMsrSummaryOutputStream << left <<
    setw (fieldWidth) <<
    "VoiceAbsoluteNumber" << " : " <<
    elt->getVoiceAbsoluteNumber () <<
    endl <<
    setw (fieldWidth) <<
    "VoicePartRelativeID" << " : " <<
    elt->getVoicePartRelativeID () <<
    endl <<
    setw (fieldWidth) <<
    "VoiceStaffRelativeNumber" << " : " <<
    elt->getVoiceStaffRelativeNumber () <<
    endl <<
    setw (fieldWidth) <<
    "VoiceActualNotesCounter" << " : " <<
    elt->getVoiceActualNotesCounter () <<
    endl <<
    setw (fieldWidth) <<
    "VoiceActualHarmoniesCounter" << " : " <<
    elt->getVoiceActualHarmoniesCounter () <<
    endl <<
    setw (fieldWidth) <<
    "MusicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (
      elt->getMusicHasBeenInsertedInVoice ()) <<
    endl;

  fMsrSummaryOutputStream <<
    endl;

  // don't show fVoiceMuteStanza in the summary
}

void msr2SummaryVisitor::visitEnd (S_msrVoice& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrVoice" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrStanza& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrStanza" <<
      endl;
  }

  fStanzasCounter++;
  
  int syllablesSize = elt->getSyllables ().size();

  fMsrSummaryOutputStream <<
    "Stanza" << " " << elt->getStanzaName () <<
    " contains " <<
    singularOrPlural (
      syllablesSize, "syllable", "syllables");

  if (! elt->getStanzaTextPresent ())
    fMsrSummaryOutputStream << " (No actual text)";
    
  fMsrSummaryOutputStream << endl << endl;
}

void msr2SummaryVisitor::visitEnd (S_msrStanza& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrStanza" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSyllable& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrSyllable" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrSyllable& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrSyllable" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrClef& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrClef" <<
      endl;
  }

  if (! fOnGoingStaff) {
    fMsrSummaryOutputStream <<
      elt->clefAsString () <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrClef& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrClef" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrKey& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrKey" <<
      endl;
  }

  if (! fOnGoingStaff) {
    fMsrSummaryOutputStream <<
      elt->keyAsString () <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrKey& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrKey" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTime& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrTime" <<
      endl;
  }

  if (! fOnGoingStaff) {    
    fMsrSummaryOutputStream <<
      elt->timeAsString () <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrTime& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrTime" <<
    endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTempo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrTempo" <<
       endl;
  }

  fMsrSummaryOutputStream <<
    "Tempo" << " " <<
    elt->getTempoUnit () << " " << elt->getPerMinute () <<
    endl;
}

void msr2SummaryVisitor::visitEnd (S_msrTempo& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrTempo" <<
    endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSegment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrSegment" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrSegment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrSegment" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrArticulation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrArticulation" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrArticulation& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrArticulation" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrDynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrDynamics" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrDynamics& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrDynamics" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrWedge& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrWedge" << endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrWedge& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrWedge" << endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrGraceNotes& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrGraceNotes" << endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrGraceNotes& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrGraceNotes" << endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrNote& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
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
    fMsrSummaryOutputStream <<
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
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrStem" << endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrStem& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrStem" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBeam& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrBeam" << endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrBeam& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrBeam" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrChord& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrChord" <<
      endl;
  }

  fScoreChordsCounter++;
}

void msr2SummaryVisitor::visitEnd (S_msrChord& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrChord" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrTuplet& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrTuplet" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrTuplet& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrTuplet" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrSlur& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrSlur" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrSlur& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrSlur" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarline& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrBarline" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrBarline& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrBarline" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrBarNumberCheck& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrBarNumberCheck" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrBarNumberCheck" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLineBreak& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrLineBreak" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrLineBreak& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrLineBreak" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrRepeat& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrRepeat" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrRepeat& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrRepeat" <<
      endl;
  }
}

//________________________________________________________________________
/* JMI
void msr2SummaryVisitor::visitStart (S_msrComment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrComment" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrComment& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrComment" <<
      endl;
  }
}
*/

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrVarValAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrVarValAssoc" <<
      endl;
  }

  fMsrSummaryOutputStream <<
    "VarValAssoc" <<
    endl;
  
  gIndenter++;
  
  fMsrSummaryOutputStream <<
    "variable name : " <<
    "\"" << elt->getVariableName () << "\"" <<
    endl <<
    "variable value: " <<
    "\"" << elt->getVariableValue () << "\"" <<
    endl <<
    endl;
  
  gIndenter--;
}

void msr2SummaryVisitor::visitEnd (S_msrVarValAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrVarValAssoc" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrPageGeometry& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrPageGeometry" <<
      endl;
  }

  fMsrSummaryOutputStream <<
    elt;
}

void msr2SummaryVisitor::visitEnd (S_msrPageGeometry& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrPageGeometry" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrLayout& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrLayout" <<
      endl;
  }

  fMsrSummaryOutputStream << "Layout" << endl;

  gIndenter++;
}

void msr2SummaryVisitor::visitEnd (S_msrLayout& elt)
{
  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrLayout" <<
      endl;
  }
}

//________________________________________________________________________
void msr2SummaryVisitor::visitStart (S_msrMidi& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrMidi" <<
      endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrMidi& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrMidi" <<
      endl;
  }
}


} // namespace
