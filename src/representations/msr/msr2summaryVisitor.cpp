/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "oahOah.h"
#include "msrOah.h"

#include "msr2summaryVisitor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msr2summaryVisitor::msr2summaryVisitor (
  S_msrOahGroup& msrOpts)
{
  fMsrOahGroup = msrOpts;

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
  fScoreRegularNotesCounter    = 0;
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

msr2summaryVisitor::~msr2summaryVisitor ()
{}

//________________________________________________________________________
void msr2summaryVisitor::printSummaryFromMsrScore (
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
void msr2summaryVisitor::visitStart (S_msrScore& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScore" <<
      endl;
  }

  gLogStream <<
    "MSR contents summary of \"" <<
    gGlobalOahOahGroup->getInputSourceName () <<
    "\":" <<
    endl << endl;

  ++gIndenter;
}

void msr2summaryVisitor::visitEnd (S_msrScore& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScore" <<
      endl;
  }

  gLogStream <<
    "The score contains:" <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 3;

  gLogStream <<
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
      fScoreRegularNotesCounter,
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
      "grace note group",
      "grace notes groups") <<
    endl <<

    setw (fieldWidth) <<
    singularOrPlural (
      fScoreDoubleTremoloNotesCounter,
      "double tremolo note",
      "double tremolo notes") <<
    endl << endl;

  --gIndenter;
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrPartGroup& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPartGroup" <<
      endl;
  }

  ++fPartGroupsCounter;

  unsigned int partGroupElementsSize = elt->getPartGroupElements ().size ();

  gLogStream <<
    "PartGroup" << " " << elt->getPartGroupCombinedName () <<
    " contains " <<
    singularOrPlural (
      partGroupElementsSize,
      " part or sub part group",
      " parts or sub part groups") <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 24;

  gLogStream << left <<
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
    endl << endl;
}

void msr2summaryVisitor::visitEnd (S_msrPartGroup& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPartGroup" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrPart& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPart" <<
      endl;
  }

  ++fPartsCounter;

  unsigned int partStavesMapSize = elt->getPartStavesMap ().size ();

  gLogStream <<
    "Part " << elt->getPartCombinedName () <<
    " contains " <<
    singularOrPlural (
      partStavesMapSize,
      "staff", "staves") <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 28;

  gLogStream << left <<
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
    endl << endl;
}

void msr2summaryVisitor::visitEnd (S_msrPart& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPart" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrStaff& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStaff" <<
      endl;
  }

  ++fStavesCounter;

  unsigned int staffAllVoicesVectorSize =
    elt->getStaffAllVoicesVector ().size ();

  gLogStream <<
    "Staff" << " " << elt->getStaffName () <<
    " contains " <<
    singularOrPlural (
      staffAllVoicesVectorSize,
      "voice", "voices") <<
    endl;

  ++gIndenter;


  const unsigned int fieldWidth = 27;

  // print the staff number
  gLogStream << left <<
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

  gLogStream << endl;

  fOnGoingStaff = true;
}

void msr2summaryVisitor::visitEnd (S_msrStaff& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStaff" <<
      endl;
  }

  fOnGoingStaff = false;
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrVoice& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrVoice" <<
      endl;
  }

  ++fVoicesCounter;

  unsigned int voiceStanzasMapSize = elt->getVoiceStanzasMap ().size ();

  gLogStream <<
    "Voice" << " " << elt->getVoiceName () <<
    " has " <<
    singularOrPlural (
      voiceStanzasMapSize,
      "stanza", "stanzas") <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 34;

  gLogStream << left <<
    setw (fieldWidth) <<
    "voiceNumber" << " : " <<
    elt->getVoiceNumber () <<
    endl <<
    setw (fieldWidth) <<
    "regularVoiceStaffSequentialNumber" << " : " <<
    elt->getRegularVoiceStaffSequentialNumber () <<
    endl <<
    setw (fieldWidth) <<
    "voiceActualNotesCounter" << " : " <<
    elt->getVoiceActualNotesCounter () <<
    endl <<
    setw (fieldWidth) <<
    "voiceActualHarmoniesCounter" << " : " <<
    elt->getVoiceActualHarmoniesCounter () <<
    endl <<
    setw (fieldWidth) <<
    "musicHasBeenInsertedInVoice" << " : " <<
    booleanAsString (
      elt->getMusicHasBeenInsertedInVoice ()) <<
    endl;

  gLogStream << endl;

  // don't show fVoiceMuteStanza in the summary
}

void msr2summaryVisitor::visitEnd (S_msrVoice& elt)
{
  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrVoice" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrStanza& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStanza" <<
      endl;
  }

  ++fStanzasCounter;

  unsigned int syllablesSize = elt->getSyllables ().size ();

  gLogStream <<
    "Stanza" << " " << elt->getStanzaName () <<
    " contains " <<
    singularOrPlural (
      syllablesSize,
      "syllable", "syllables");

  if (! elt->getStanzaTextPresent ()) {
    gLogStream <<
      " (no actual text)";
  }

  gLogStream <<
    endl << endl;
}

void msr2summaryVisitor::visitEnd (S_msrStanza& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStanza" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrSyllable& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSyllable" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrSyllable& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSyllable" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrClef& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrClef" <<
      endl;
  }

  if (! fOnGoingStaff) {
    gLogStream <<
      elt->asString () <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrClef& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrClef" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrKey& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrKey" <<
      endl;
  }

  if (! fOnGoingStaff) {
    gLogStream <<
      elt->asString () <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrKey& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrKey" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrTime& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTime" <<
      endl;
  }

  if (! fOnGoingStaff) {
    gLogStream <<
      elt->asString () <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrTime& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTime" <<
    endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrTempo& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTempo" <<
       endl;
  }

  gLogStream <<
    "Tempo " <<
    elt->getTempoBeatUnit () << " " << elt->getTempoPerMinute () <<
    endl;
}

void msr2summaryVisitor::visitEnd (S_msrTempo& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTempo" <<
    endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrSegment& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSegment" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrSegment& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSegment" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrArticulation& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrArticulation" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrArticulation& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrArticulation" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrDynamics& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrDynamics" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrDynamics& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrDynamics" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrWedge& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrWedge" << endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrWedge& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrWedge" << endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrGraceNotesGroup& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrGraceNotesGroup" << endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrGraceNotesGroup& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrGraceNotesGroup" << endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrNote& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
      ++fScoreRestNotesCounter;
      break;
    case msrNote::kSkipNote:
      ++fScoreSkipNotesCounter;
      break;
    case msrNote::kUnpitchedNote:
      ++fScoreUnpitchedNotesCounter;
      break;
    case msrNote::kRegularNote:
      ++fScoreRegularNotesCounter;
      break;
    case msrNote::kDoubleTremoloMemberNote:
      ++fScoreRegularNotesCounter;
      break;
    case msrNote::kGraceNote:
    case msrNote::kGraceSkipNote:
    case msrNote::kGraceChordMemberNote:
      ++fScoreGraceNotesGroupCounter;
      break;
    case msrNote::kChordMemberNote:
      ++fScoreChordNotesCounter;
      break;
    case msrNote::kTupletMemberNote:
    case msrNote::kTupletRestMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletUnpitchedMemberNote:
      ++fScoreTupletNotesCounter;
      break;
  } // switch
}

void msr2summaryVisitor::visitEnd (S_msrNote& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
    case msrNote::kRegularNote:
      break;
    case msrNote::kDoubleTremoloMemberNote:
      break;
    case msrNote::kGraceNote:
    case msrNote::kGraceSkipNote:
    case msrNote::kGraceChordMemberNote:
      break;
    case msrNote::kChordMemberNote:
      break;
    case msrNote::kTupletMemberNote:
    case msrNote::kTupletRestMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletUnpitchedMemberNote:
      break;
  } // switch
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrStem& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrStem" << endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrStem& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrStem" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrBeam& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBeam" << endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrBeam& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBeam" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrChord& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrChord" <<
      endl;
  }

  ++fScoreChordsCounter;
}

void msr2summaryVisitor::visitEnd (S_msrChord& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrChord" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrTuplet& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrTuplet" <<
      endl;
  }

  ++fScoreTupletsCounter;
}

void msr2summaryVisitor::visitEnd (S_msrTuplet& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrTuplet" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrSlur& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrSlur" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrSlur& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrSlur" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrBarline& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarline" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrBarline& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarline" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrBarNumberCheck& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrBarNumberCheck" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrBarNumberCheck" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrLineBreak& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrLineBreak" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrLineBreak& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrLineBreak" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrRepeat& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrRepeat" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrRepeat& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrRepeat" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrScaling& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrScaling" <<
      endl;
  }

  gLogStream <<
    elt;
}

void msr2summaryVisitor::visitEnd (S_msrScaling& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrScaling" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrPageLayout& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrPageLayout" <<
      endl;
  }

  gLogStream << elt << endl;
}

void msr2summaryVisitor::visitEnd (S_msrPageLayout& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrPageLayout" <<
      endl;
  }
}

//________________________________________________________________________
void msr2summaryVisitor::visitStart (S_msrMidiTempo& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> Start visiting msrMidiTempo" <<
      endl;
  }
}

void msr2summaryVisitor::visitEnd (S_msrMidiTempo& elt)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "--> End visiting msrMidiTempo" <<
      endl;
  }
}


} // namespace
