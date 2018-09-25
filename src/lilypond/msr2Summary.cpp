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

#include "msrOptions.h"

#include "msr2Summary.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2SummaryVisitor::msr2SummaryVisitor (
  S_msrOptions&    msrOpts,
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
  fScoreUnpitchedNotesCounter     = 0;
  fScoreStandaloneNotesCounter    = 0;
  fScoreRestNotesCounter          = 0;
  fScoreSkipNotesCounter          = 0;
  fScoreDoubleTremoloNotesCounter = 0;
  fScoreGraceNotesGroupCounter    = 0;
  
  // chords
  fScoreChordNotesCounter         = 0;
  fScoreChordsCounter             = 0;

  // tuplets
  fScoreTupletNotesCounter        = 0;
  fScoreTupletsCounter            = 0;
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
  
  fMsrSummaryOutputStream <<
    "Score contains " <<
    singularOrPlural (
      elt->getPartGroupsList ().size (),
      "part group", "part groups") <<
    ", " <<
    singularOrPlural (
      elt->getScoreNumberOfMeasures (),
      "measure", "measures") <<
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
  
  const int fieldWidth = 3;

  fMsrSummaryOutputStream <<
    setw (fieldWidth) <<
    singularOrPlural (
      fPartGroupsCounter,
      "part group",
      "part groups") <<
    endl <<
    
    setw (fieldWidth) <<
    singularOrPlural (
      fPartsCounter,
      "part",
      "parts") <<
    endl <<
    
    setw (fieldWidth) <<
    singularOrPlural (
      fStavesCounter,
      "stave",
      "staves") <<
    endl <<
    
    setw (fieldWidth) <<
    singularOrPlural (
      fVoicesCounter,
      "voice",
      "voices") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fStanzasCounter,
      "stanza",
      "stanzas") <<
    endl <<

    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      elt->getScoreNumberOfMeasures (),
      "measure",
      "measures") <<
    endl <<

    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreStandaloneNotesCounter,
      "standalone note",
      "standalone notes") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreRestNotesCounter,
      "rest note",
      "rest notes") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreSkipNotesCounter,
      "skip note",
      "skip notes") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreUnpitchedNotesCounter,
      "unpitched note",
      "unpitched notes") <<
    endl <<

    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreChordsCounter,
      "chord",
      "chords") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreChordNotesCounter,
      "chord note",
      "chord notes") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreTupletsCounter,
      "tuplet",
      "tuplets") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreTupletNotesCounter,
      "tuplet note",
      "tuplet notes") <<
    endl <<

    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreGraceNotesGroupCounter,
      "grace note gropu",
      "grace notes groups") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreDoubleTremoloNotesCounter,
      "double tremolo note",
      "double tremolo notes") <<
    endl <<
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

  int partGroupElementsSize = elt->getPartGroupElements ().size ();
  
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
    "partGroupAbsoluteNumber" << " : \"" <<
    elt->getPartGroupAbsoluteNumber  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupName" << " : \"" <<
    elt->getPartGroupName  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupNameDisplayText" << " : \"" <<
    elt->getPartGroupNameDisplayText  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupAccidentalText" << " : \"" <<
    elt->getPartGroupAccidentalText  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupAbbrevation" << " : \"" <<
    elt->getPartGroupAbbreviation  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupSymbolKind" << " : " <<
    msrPartGroup::partGroupSymbolKindAsString (
      elt->getPartGroupSymbolKind  ()) <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupSymbolDefaultX" << " : " <<
    elt->getPartGroupSymbolDefaultX  () <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupBarline" << " : \"" <<
    msrPartGroup::partGroupBarlineKindAsString (
      elt->getPartGroupBarlineKind ()) <<
    "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partGroupInstrumentName" << " : \"" <<
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
  
  int partStavesMapSize = elt->getPartStavesMap ().size ();
  
  fMsrSummaryOutputStream <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    singularOrPlural (
      partStavesMapSize,
      "staff", "staves") <<
    endl;
    
  gIndenter++;
  
  const int fieldWidth = 28;
  
  fMsrSummaryOutputStream << left <<
    setw (fieldWidth) <<
    "partID" << " : \"" <<
    elt->getPartID () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partMsrName" << " : \"" <<
    elt->getPartMsrName () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partName" << " : \"" <<
    elt->getPartName  () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partAbbrevation" << " : \"" <<
    elt->getPartAbbreviation () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partInstrumentName" << " : \"" <<
    elt->getPartInstrumentName () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partInstrumentAbbreviation" << " : \"" <<
    elt->getPartInstrumentAbbreviation () << "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "partNumberOfMeasures" << " : " <<
    elt->getPartNumberOfMeasures () <<
    endl <<
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
  
  int staffAllVoicesListSize = elt->getStaffAllVoicesList ().size ();
    
  fMsrSummaryOutputStream <<
    "Staff" << " " << elt->getStaffName () <<
    " contains " <<
    singularOrPlural (
      staffAllVoicesListSize,
      "voice", "voices") <<
    endl;

  gIndenter++;

  
  const int fieldWidth = 27;

  // print the staff number
  fMsrSummaryOutputStream << left <<
    setw (fieldWidth) <<
    "staffNumber" << " : " <<
    elt->getStaffNumber () <<
    endl <<

    setw (fieldWidth) <<
   "staffInstrumentName" << ": \"" <<
    elt->getStaffInstrumentName () << "\"" <<
    endl <<

    setw (fieldWidth) <<
   "staffInstrumentAbbreviation" << ": \"" <<
    elt->getStaffInstrumentAbbreviation () << "\"" <<
    endl;

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

  int voiceStanzasMapSize = elt->getVoiceStanzasMap ().size ();

  fMsrSummaryOutputStream <<
    "Voice" << " " << elt->getVoiceName () <<
    " has " <<
    singularOrPlural (
      voiceStanzasMapSize,
      "stanza", "stanzas") <<
    endl;

  gIndenter++;

  const int fieldWidth = 34;
  
  fMsrSummaryOutputStream << left <<
    setw (fieldWidth) <<
    "voiceNumber" << " : " <<
    elt->getVoiceNumber () <<
    endl <<
    setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    elt->getRegularVoiceStaffSequentialNumber () <<
    endl <<
    setw (fieldWidth) <<
    "noiceActualNotesCounter" << " : " <<
    elt->getVoiceActualNotesCounter () <<
    endl <<
    setw (fieldWidth) <<
    "VoiceActualHarmoniesCounter" << " : " <<
    elt->getVoiceActualHarmoniesCounter () <<
    endl <<
    setw (fieldWidth) <<
    "musicHasBeenInsertedInVoice" << " : " <<
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
  
  int syllablesSize = elt->getSyllables ().size ();

  fMsrSummaryOutputStream <<
    "Stanza" << " " << elt->getStanzaName () <<
    " contains " <<
    singularOrPlural (
      syllablesSize,
      "syllable", "syllables");

  if (! elt->getStanzaTextPresent ()) {
    fMsrSummaryOutputStream <<
      " (no actual text)";
  }
    
  fMsrSummaryOutputStream <<
    endl <<
    endl;
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
      elt->asString () <<
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
      elt->asString () <<
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
      elt->asString () <<
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
    elt->getTempoBeatUnit () << " " << elt->getTempoPerMinute () <<
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
void msr2SummaryVisitor::visitStart (S_msrGraceNotesGroup& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrGraceNotesGroup" << endl;
  }
}

void msr2SummaryVisitor::visitEnd (S_msrGraceNotesGroup& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrGraceNotesGroup" << endl;
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
    case msrNote::kUnpitchedNote:
      fScoreUnpitchedNotesCounter++;
      break;
    case msrNote::kStandaloneNote:
      fScoreStandaloneNotesCounter++;
      break;
    case msrNote::kDoubleTremoloMemberNote:
      fScoreStandaloneNotesCounter++;
      break;
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      fScoreGraceNotesGroupCounter++;
      break;
    case msrNote::kChordMemberNote:
      fScoreChordNotesCounter++;
      break;
    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
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
    case msrNote::kUnpitchedNote:
      break;
    case msrNote::kStandaloneNote:
      break;
    case msrNote::kDoubleTremoloMemberNote:
      break;
    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      break;
    case msrNote::kChordMemberNote:
      break;
    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
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

  fScoreTupletsCounter++;
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
  
  const int fieldWidth = 16;

  fMsrSummaryOutputStream <<
    setw (fieldWidth) <<
    "varValAssocKind" << " : " <<
    "\"" <<
    elt->varValAssocKindAsString () <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "variableValue" << " : " <<
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
void msr2SummaryVisitor::visitStart (S_msrVarValsListAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> Start visiting msrVarValsListAssoc" <<
      endl;
  }

  fMsrSummaryOutputStream <<
    "VarValsListAssoc" <<
    endl;
  
  gIndenter++;

  const int fieldWidth = 21;
  
  fMsrSummaryOutputStream << left <<
    setw (fieldWidth) <<
    "varValsListAssocKind" << " : " <<
    "\"" <<
    elt->varValsListAssocKindAsString () <<
    "\"" <<
    endl <<
    
    setw (fieldWidth) <<
    "varValsListAssocValues" << " : " <<
    "'" << elt->varValsListAssocValuesAsString () << "'" <<
    endl <<
    endl;
  
  gIndenter--;
}

void msr2SummaryVisitor::visitEnd (S_msrVarValsListAssoc& elt)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    fMsrSummaryOutputStream <<
      "--> End visiting msrVarValsListAssoc" <<
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
