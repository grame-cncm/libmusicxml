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

#include "xml_tree_browser.h"

#include "lpsrUtilities.h"

#include "lpsr.h"
#include "xml2LpsrDictionaryVisitor.h"


using namespace std;

namespace MusicXML2
{
//________________________________________________________________________
xml2LpsrDictionaryVisitor::xml2LpsrDictionaryVisitor (
  S_translationSettings& ts)
{
  fTranslationSettings = ts;

  // create the dictionary
  fDictionary =
    lpsrDictionary::create (fTranslationSettings);

  // add the implicit part group to the dictionary
  fCurrentPartGroupNumber = 1;
  fCurrentPartGroup =
    fDictionary->
      addPartGroupToDictionary (fCurrentPartGroupNumber);

  // add the implicit part to the implicit part group
  string partXMLName = "implicit";
  fCurrentPart =
    fCurrentPartGroup->
      addPartToPartGroup (partXMLName); // JMI ???

  // add the implicit staff to the implicit part
  fCurrentStaffNumber = 1;
  fCurrentStaff =
    fCurrentPart->
      addStaffToPart (fCurrentStaffNumber); // JMI ???

  // add the implicit voice to the implicit staff
  fCurrentVoiceNumber = 1;
  fCurrentVoice =
    fCurrentStaff->
      addVoiceToStaff (fCurrentVoiceNumber); // JMI ???

  // add the implicit lyrics to the implicit voice
  fCurrentLyricNumber = 1;
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (fCurrentLyricNumber); // JMI ???
  
  fCurrentMeasureNumber = 0;
}

xml2LpsrDictionaryVisitor::~xml2LpsrDictionaryVisitor () {}

//________________________________________________________________________
S_lpsrDictionary
xml2LpsrDictionaryVisitor::buildDictionaryFromXMLElementTree (
  const Sxmlelement& xmlTree)
{
  S_lpsrDictionary result;
  
  if (xmlTree) {
    // create a tree browser on this visitor
    tree_browser<xmlelement> browser (this);
    
    // browse the xmlelement tree
    browser.browse (*xmlTree);
    
    // grab the resulting dictionary
    result = fDictionary;
  }

  return result;
}

void xml2LpsrDictionaryVisitor::internalError (
  string message)
{
  cerr <<
    "### Internal error: measure " << fCurrentMeasureNumber << endl <<
    message << endl;
}                        

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_part_list& elt)
{
}
/*
  <part-list>
    <part-group number="1" type="start">
      <group-symbol default-x="-7">bracket</group-symbol>
      <group-barline>yes</group-barline>
    </part-group>
    <score-part id="P1">
      <part-name>Piccolo</part-name>
      <part-abbreviation>Picc.</part-abbreviation>
      <score-instrument id="P1-I18">
        <instrument-name>Picc.</instrument-name>
      </score-instrument>
      <midi-instrument id="P1-I18">
        <midi-channel>1</midi-channel>
        <midi-program>73</midi-program>
      </midi-instrument>
    </score-part>
    <part-group number="2" type="start">
      <group-name>1
2</group-name>
      <group-barline>yes</group-barline>
    </part-group>
    <score-part id="P2">
*/

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_divisions& elt ) 
{
  fCurrentMusicXMLDivisions = (int)(*elt);
  
  if (fTranslationSettings->fTrace) {
    if (fCurrentMusicXMLDivisions == 1)
      cerr << "There is 1 division per quater note" << endl;
    else
      cerr <<
        "There are " << fCurrentMusicXMLDivisions <<
        " divisions per quater note" << endl;
  }
}

//________________________________________________________________________

/*
    <part-group type="start" number="1">
    </part-group>
    <part-group type="stop" number="1"/>
 */
void xml2LpsrDictionaryVisitor::visitStart (S_part_group& elt)
{
  // the part group number indicates nested/overlapping groups
  fCurrentPartGroupNumber =
    elt->getAttributeIntValue ("number", 0);
  fCurrentPartGroupType =
    elt->getAttributeValue ("type");

  if (fCurrentPartGroupType == "start") {

    // is this part group number already present?
    fCurrentPartGroup =
      fDictionary->dictionaryContainsPartGroup (
        fCurrentPartGroupNumber);

    // no, add it to the dictionary
    if (! fCurrentPartGroup) 
    fCurrentPartGroup =
      fDictionary->addPartGroupToDictionary (
        fCurrentPartGroupNumber);
        
  } else if (fCurrentPartGroupType == "stop") {

      fCurrentPartGroup->popPartGroupPartsStackTop ();
    
  } else {
    
    lpsrMusicXMLError (
      "unkown part group type \"" + fCurrentPartGroupType + "\"");
  }
}

void xml2LpsrDictionaryVisitor::visitStart (S_group_symbol& elt)
{
  fCurrentGroupSymbol = elt->getValue ();

  fCurrentPartGroup->setPartGroupSymbol (fCurrentGroupSymbol);
}

void xml2LpsrDictionaryVisitor::visitStart ( S_group_barline& elt)
{
  fCurrentGroupBarline = elt->getValue ();

  fCurrentPartGroup->
    setPartGroupBarline (fCurrentGroupBarline);
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_score_part& elt)
{
  fCurrentPartMusicXMLName = elt->getAttributeValue ("id");
/*
  // is this part already present?
  fCurrentPart =
    fCurrentPart->
      partContainsStaff (
        fCurrentPartMusicXMLName);

  // no, add it to the current JMI ???
  if (! fCurrentPart)
  */
    fCurrentPart =
      fCurrentPartGroup->
        addPartToPartGroup (
          fCurrentPartMusicXMLName);
}

void xml2LpsrDictionaryVisitor::visitStart (S_part_name& elt)
{
  string fCurrentPartName = elt->getValue ();

  fCurrentPart->
    setPartName (fCurrentPartName);
}

void xml2LpsrDictionaryVisitor::visitStart (S_part_abbreviation& elt)
{
  string fCurrentPartAbbreviation = elt->getValue ();

  fCurrentPart->
    setPartAbbreviation (fCurrentPartAbbreviation);
}

void xml2LpsrDictionaryVisitor::visitStart (S_instrument_name& elt)
{
  string fCurrentPartInstrumentName = elt->getValue(); // jMI

  fCurrentPart->
    setPartInstrumentName (fCurrentPartInstrumentName);
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_part& elt)
{
  fCurrentStaffNumber = 1; // default if there are no <staff> element

  fCurrentVoiceNumber = -1 ;

  fCurrentMeasureNumber = 0; // in case of an anacrusis
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_staves& elt)
{
  /* by S_measure in fact!
  fStavesNumber = int(*elt);

  if (fTranslationSettings->fTrace) {
    switch (fStavesNumber) {
      case 0:
        cerr << "There isn't any explicit staff (hence 1 by default)";
        break;
      case 1:
        cerr << "There is 1 staff";
        break;
      default:
        cerr << "There are " << fStavesNumber << " staves";
    } // switch
    cerr <<
      " in part " << fCurrentPart->getPartMusicXMLName() <<
      " (" << fCurrentPart->getPartLPSRName() << ")" << endl;
  }
  */
}

void xml2LpsrDictionaryVisitor::visitStart (S_staff& elt)
{
  fCurrentStaffNumber = int(*elt);

  // is this staff already present?
  fCurrentStaff =
    fCurrentPart->
      partContainsStaff (
        fCurrentStaffNumber);

  // no, add it to the current part
  if (! fCurrentStaff) 
    fCurrentStaff =
      fCurrentPart->
        addStaffToPart (fCurrentStaffNumber);
}
    
//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_voice& elt )
{
  fCurrentVoiceNumber = int(*elt);
  /*
  cout <<
    "--> S_voice, fCurrentVoiceNumber = " << fCurrentVoiceNumber << endl;
  */

  // is this voice already present?
  fCurrentVoice =
    fCurrentStaff->
      staffContainsVoice (
        fCurrentPartGroupNumber);

  // no, add it to the current staff
  if (! fCurrentVoice) 
    fCurrentVoice =
      fCurrentStaff->
        addVoiceToStaff (
          fCurrentVoiceNumber);

  fMusicXMLNoteData.fVoiceNumber = fCurrentVoiceNumber;
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_lyric& elt ) { 
  fCurrentLyricNumber =
    elt->getAttributeIntValue ("number", 0);

  // is this lyrics already present?
  fCurrentLyrics =
    fCurrentVoice->voiceContainsLyrics (
      fCurrentLyricNumber);

  // no, add it to the current staff
  if (! fCurrentLyrics) 
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (
        fCurrentLyricNumber);
}

void xml2LpsrDictionaryVisitor::visitStart ( S_syllabic& elt ) {
  fCurrentSyllabic = elt->getValue();
  /*

  if (fCurrentSyllabic == "begin") {
    fOnGoingLyrics = true;
  }
  else if (fCurrentSyllabic == "end") {
    fOnGoingLyrics = true;
  }
  * */
}

void xml2LpsrDictionaryVisitor::visitEnd ( S_text& elt ) 
{
  fCurrentText = elt->getValue();
/*
  cout <<
    "--> fCurrentLyricNumber = " << fCurrentLyricNumber <<
    ", fCurrentSyllabic = " << fCurrentSyllabic <<
    ", fCurrentText = |" << fCurrentText << "|" << endl;
*/
}

void xml2LpsrDictionaryVisitor::visitEnd ( S_lyric& elt ) {
  /*
 // fOnGoingLyrics = false;

  if (! fCurrentVoiceLyricsMap [fCurrentLyricNumber]) {
    // create lyrics on first visit
    initiateLyrics ();
  }

  // create lyrics chunk
  / *
  cout <<
      "--> creating lyrics word chunk  containing " <<
      fCurrentText << endl;

      enum LyricsChunkType {
      kSingleChunk, kBeginChunk, kMiddleChunk, kEndChunk,
      kSkipChunk, kBreakChunk };
  * /
  
  lpsrLyricsChunk::LyricsChunkType chunkType;
  
  if      (fCurrentSyllabic == "single") chunkType = lpsrLyricsChunk::kSingleChunk;
  else if (fCurrentSyllabic == "begin")  chunkType = lpsrLyricsChunk::kBeginChunk;
  else if (fCurrentSyllabic == "middle") chunkType = lpsrLyricsChunk::kMiddleChunk;
  else if (fCurrentSyllabic == "end")    chunkType = lpsrLyricsChunk::kEndChunk;
  else {
    stringstream s;
    string  result;
  
    s << "--> fCurrentSyllabic = " << fCurrentSyllabic << " is unknown";
    s >> result;
    lpsrMusicXMLError(result);
  }

  switch (chunkType) {
    case lpsrLyricsChunk::kSingleChunk:
    case lpsrLyricsChunk::kBeginChunk:
      {
      // create new lyrics on first visit
      /*
      S_lpsrLyrics // ???
        newLyrics =
          lpsrLyrics::create (fCurrentLyricsName, fCurrentVoiceName);
  * /
      // add the new lyrics to the current lyrics
   //   fCurrentLyrics->
    //    addLyricsToLyrics (fCurrentLyricNumber, newLyrics);
    //  fCurrentVoiceLyricsMap [fCurrentLyricNumber] = newLyrics;
  
      fCurrentChunk =
        lpsrLyricsChunk::create (
          chunkType, fCurrentText);

      cout <<
        "==> fCurrentSyllabic = " << fCurrentSyllabic <<
        ", fCurrentLyricNumber = " << fCurrentLyricNumber << endl;
  
      // add lyrics chunk to current lyrics
      fCurrentVoiceLyricsMap [fCurrentLyricNumber]->
        addChunkToLyrics (fCurrentChunk);
     }
     break;

    case lpsrLyricsChunk::kMiddleChunk:
    case lpsrLyricsChunk::kEndChunk:
      // add chunk to current lyrics
      fCurrentVoiceLyricsMap [fCurrentLyricNumber]->
        addChunkToLyrics (fCurrentChunk);
      break;
    case lpsrLyricsChunk::kSkipChunk:
    case lpsrLyricsChunk::kBreakChunk:
      {}
      break;
  } // switch
  */
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_measure& elt)
{
  fCurrentMeasureNumber =
    elt->getAttributeIntValue ("number", 0);
}

/*
  <barline location="left">
    <bar-style>heavy-light</bar-style>
    <repeat direction="forward"/>
  </barline>
  *
  *   <barline location="right">
    <bar-style>light-heavy</bar-style>
    <repeat direction="backward"/>
  </barline>
  * 
  */


//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_barline& elt ) 
{
  fCurrentBarlineLocation =
    elt->getAttributeValue ("location");
  
  if (fCurrentBarlineLocation == "left") {

    
  } else  if (fCurrentBarlineLocation == "middle") {
    
    S_lpsrBarLine
      barline =
        lpsrBarLine::create (fCurrentMeasureNumber+1);
    S_lpsrElement b = barline;
    fCurrentVoice->appendElementToVoiceSequence (b);
    
  } else if (fCurrentBarlineLocation == "right") {

    
  } else {

  }
}

void xml2LpsrDictionaryVisitor::visitEnd ( S_barline& elt ) 
{
 /*
  *       <barline location="right">
        <bar-style>light-heavy</bar-style>
      </barline>
*/
  if (
      fCurrentBarlineLocation == "right"
        &&
      fCurrentBarStyle == "light-heavy") {
    S_lpsrBarCommand
      barCommand =
        lpsrBarCommand::create ();
    S_lpsrElement b = barCommand;
    fCurrentVoice->appendElementToVoiceSequence (b);
      
  }
}

void xml2LpsrDictionaryVisitor::visitStart ( S_bar_style& elt ) 
{
  fCurrentBarStyle = elt->getValue();
}

void xml2LpsrDictionaryVisitor::visitStart ( S_repeat& elt ) 
{
  fCurrentRepeatDirection =
    elt->getAttributeValue ("direction");  
}

/*
 *   <barline location="left">
    <ending type="start" number="1"/>
  </barline>

*/

void xml2LpsrDictionaryVisitor::visitStart ( S_ending& elt ) 
{
  // start, stop, discontinue
  
  fCurrentEndingType =
    elt->getAttributeValue ("type");  

  fCurrentEndingNumber =
    elt->getAttributeIntValue ("number", 0);
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_step& elt )
{
  string step = elt->getValue();
  
   if (step.length() != 1) {
    stringstream s;
    string  message;
    s << "step value " << step << " should be a single letter from A to G";
    s >> message;
    lpsrMusicXMLError (message);
  }

  fMusicXMLNoteData.fMusicxmlStep = step[0];
}

void xml2LpsrDictionaryVisitor::visitStart ( S_alter& elt)
{
  fMusicXMLNoteData.fMusicxmlAlteration = (int)(*elt);
}

void xml2LpsrDictionaryVisitor::visitStart ( S_octave& elt)
{
  fMusicXMLNoteData.fMusicxmlOctave = (int)(*elt);
}

void xml2LpsrDictionaryVisitor::visitStart ( S_duration& elt )
{
  fMusicXMLNoteData.fMusicxmlDuration = (int)(*elt);
//  cout << "=== xml2LpsrDictionaryVisitor::visitStart ( S_duration& elt ), fCurrentMusicXMLDuration = " << fCurrentMusicXMLDuration << endl; JMI
}

void xml2LpsrDictionaryVisitor::visitStart ( S_dot& elt )
{
  fMusicXMLNoteData.fDotsNumber++;
}
       
void xml2LpsrDictionaryVisitor::visitStart ( S_type& elt )
{
  fCurrentType=elt->getValue();
}

void xml2LpsrDictionaryVisitor::visitStart ( S_stem& elt )
{
//  fCurrentStem = elt->getValue();
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_chord& elt)
{
  fMusicXMLNoteData.fNoteBelongsToAChord = true;
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_actual_notes& elt )
{
  fCurrentActualNotes = (int)(*elt);
}

void xml2LpsrDictionaryVisitor::visitStart ( S_normal_notes& elt )
{
  fCurrentNormalNotes = (int)(*elt);
}

void xml2LpsrDictionaryVisitor::visitStart ( S_normal_type& elt )
{
  fCurrentNormalType = elt->getValue();
}

void xml2LpsrDictionaryVisitor::visitStart ( S_tuplet& elt )
{
  fMusicXMLNoteData.fNoteBelongsToATuplet = true;

  fCurrentTupletNumber = atoi(elt->getAttributeValue("number").c_str());
  string type     = elt->getAttributeValue("type");
  
  /* JMI
  cout <<
    "xml2LpsrDictionaryVisitor::visitStart ( S_tuplet, fCurrentTupletNumber = " <<
    fCurrentTupletNumber << ", type = " << type <<endl;
  */
  
  fCurrentTupletKind = lpsrTuplet::k_NoTuplet;
  
  if (type == "start")
    fCurrentTupletKind = lpsrTuplet::kStartTuplet;
  else if (type == "continue")
    fCurrentTupletKind = lpsrTuplet::kContinueTuplet;
  else if (type == "stop")
    fCurrentTupletKind = lpsrTuplet::kStopTuplet;
  else {
    stringstream s;
    string  message;
    s << "stuplet type " << type << " is unknown";
    s >> message;
    lpsrMusicXMLError (message);
  }
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_note& elt ) 
{
  //  cout << "--> xml2LpsrDictionaryVisitor::visitStart ( S_note& elt ) " << endl;
  fMusicXMLNoteData.fMusicxmlStep = '_';
  fMusicXMLNoteData.fMusicxmlStepIsARest = false;
  fMusicXMLNoteData.fMusicxmlAlteration = 0; // natural notes
  fMusicXMLNoteData.fMusicxmlOctave = -13;
  fMusicXMLNoteData.fDotsNumber = 0;
   
  // assume this note doesn't belong to a chord until S_chord is met
  fMusicXMLNoteData.fNoteBelongsToAChord = false;

  // assume this note doesn't belong to a tuplet until S_chord is met
  fMusicXMLNoteData.fNoteBelongsToATuplet = fATupletIsBeingBuilt;
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_rest& elt)
{
  //  cout << "--> xmlPart2LpsrVisitor::visitStart ( S_rest& elt ) " << endl;
  fMusicXMLNoteData.fMusicxmlStepIsARest = true;
}

void xml2LpsrDictionaryVisitor::createChord (S_lpsrDuration noteDuration) {
  // cout << "--> creating a chord on its 2nd note" << endl;
  
  // fCurrentNote has been registered standalone in the part element sequence,
  // but it is actually the first note of a chord
  
   // create a chord
  fCurrentChord = lpsrChord::create(noteDuration);
  fCurrentElement = fCurrentChord; // another name for it
   
  //cout << "--> adding first note to fCurrentChord" << endl;
  // register fCurrentNote as first member of fCurrentChord
  fCurrentChord->addNoteToChord(fCurrentNote);
  fCurrentNote->setNoteBelongsToAChord();
  
  // move the pending dynamics if any from the first note to the chord
  list<S_lpsrDynamics> noteDynamics = fCurrentNote->getNoteDynamics();
  while (! noteDynamics.empty()) {
    //cout << "--> moving dynamics from fCurrentNote to fCurrentChord" << endl;
    S_lpsrDynamics dyn = noteDynamics.front();
    fCurrentChord->addDynamics(dyn);
    noteDynamics.pop_front();
  } // while
 
  // move the pending wedges if any from the first note to the chord
  list<S_lpsrWedge> noteWedges = fCurrentNote->getNoteWedges();
  while (! noteWedges.empty()) {
    //cout << "--> moving wedge from fCurrentNote to fCurrentChord" << endl;
    S_lpsrWedge wdg = noteWedges.front();
    fCurrentChord->addWedge(wdg);
    noteWedges.pop_front();
  } // while
}

void xml2LpsrDictionaryVisitor::createTuplet (S_lpsrNote note) {
  // create a tuplet element
  S_lpsrTuplet fCurrentTuplet = lpsrTuplet::create();
  fCurrentElement = fCurrentTuplet; // another name for it

  // populate it
  fCurrentTuplet->updateTuplet(
    fCurrentTupletNumber,
    fCurrentActualNotes,
    fCurrentNormalNotes);

  // register it in this visitor
  cout << "--> pushing tuplet to tuplets stack" << endl;
  fCurrentTupletsStack.push(fCurrentTuplet);
  
  // add note to the tuplet
  cout << "--> adding note " << note << " to tuplets stack top" << endl;
  fCurrentTuplet->addElementToTuplet(note);
}

void xml2LpsrDictionaryVisitor::finalizeTuplet (S_lpsrNote note) {
  // get tuplet from top of tuplet stack
  S_lpsrTuplet tup = fCurrentTupletsStack.top();

  // add note to the tuplet
  cout << "--> adding note " << note << " to tuplets stack top" << endl;
  tup->addElementToTuplet(note);

  // pop from the tuplets stack
  cout << "--> popping from tuplets stack" << endl;
  fCurrentTupletsStack.pop();        

  // add tuplet to the part
  cout << "=== adding tuplet to the part sequence" << endl;
  S_lpsrElement elem = tup;
  fCurrentVoice->appendElementToVoiceSequence (elem);
}          

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitEnd ( S_note& elt ) 
{
  //  cout << "<-- xml2LpsrDictionaryVisitor::visitEnd ( S_note& elt ) " << endl;
/*
  if (fTranslationSettings->fDebug)
    cerr <<
      "fMusicXMLNoteData.fMusicxmlDuration = " << 
      fMusicXMLNoteData.fMusicxmlDuration << ", " << 
      "fCurrentMusicXMLDivisions*4 = " <<
      fCurrentMusicXMLDivisions*4 << endl;
      
  if (fCurrentMusicXMLDivisions <= 0)
    lpsrMusicXMLError ("divisions cannot be 0 nor negative");
  */
  
  fMusicXMLNoteData.fMusicxmlDivisions = fCurrentMusicXMLDivisions;
  fMusicXMLNoteData.fTupletMemberType = fCurrentType;
  
  //cout << "::: creating a note" << endl;
  S_lpsrNote note =
    lpsrNote::createFromMusicXMLData (
      fTranslationSettings, fMusicXMLNoteData);
/*
  // attach the pending dynamics if any to the note
  if (! fPendingDynamics.empty()) {
/* JMI
    if (fMusicXMLNoteData.fMusicxmlStepIsARest)
      lpsrMusicXMLError (
        "dynamics cannot be attached to a rest, delayed until next note");
    else
* /
      while (! fPendingDynamics.empty()) {
        S_lpsrDynamics dyn = fPendingDynamics.front();
        note->addDynamics(dyn);
        fPendingDynamics.pop_front();
      } // while
  }
  
  // attach the pending wedges if any to the note
  if (! fPendingWedges.empty()) {
/* JMI
    if (fMusicXMLNoteData.fMusicxmlStepIsARest)
      lpsrMusicXMLError (
        "wedges cannot be attached to a rest, delayed until next note");
    else
* /
      while (! fPendingWedges.empty()) {
        S_lpsrWedge wdg = fPendingWedges.front();
        note->addWedge(wdg);
        fPendingWedges.pop_front();
      } // while
  }

  // a note can be standalone
  // or a member of a chord,
  // and the latter can belong to a tuplet
  // a rest can be standalone or belong to a tuplet
  
  if (fMusicXMLNoteData.fNoteBelongsToAChord) {
    
    if (fMusicXMLNoteData.fMusicxmlStepIsARest)
      lpsrMusicXMLError (
        "a rest cannot belong to a chord");
        
    if (! fAChordIsBeingBuilt) {
      // create a chord with fCurrentNote as its first note
      createChord (note->getNoteLpsrDuration());

      // account for chord being built
      fAChordIsBeingBuilt = true;
    }
    
    //cout << "--> adding note to fCurrentChord" << endl;
    // register note as a member of fCurrentChord
    fCurrentChord->addNoteToChord(note);
      
    // remove (previous) fCurrentNote that is the last element of the part sequence
    fCurrentVoice->removeLastElementOfVoiceSequence ();

    // add fCurrentChord to the part sequence instead
    S_lpsrElement elem = fCurrentChord;
    fCurrentVoice->appendElementToVoiceSequence (elem);

  } else if (fMusicXMLNoteData.fNoteBelongsToATuplet) {

    fMusicXMLNoteData.fTupletMemberType = fCurrentType;
    
    switch (fCurrentTupletKind) {
      case lpsrTuplet::kStartTuplet:
        {
          createTuplet(note);
          fATupletIsBeingBuilt = true;
        
          // swith to continuation mode
          // this is handy in case the forthcoming tuplet members
          // are not explictly of the "continue" type
          fCurrentTupletKind = lpsrTuplet::kContinueTuplet;
        }
        break;
  
      case lpsrTuplet::kContinueTuplet:
        {
          // populate the tuplet at the top of the stack
          cout << "--> adding note " << note << " to tuplets stack top" << endl;
          fCurrentTupletsStack.top()->addElementToTuplet(note);
        }
        break;
  
      case lpsrTuplet::kStopTuplet:
        {
          finalizeTuplet(note);

          // indicate the end of the tuplet
          fATupletIsBeingBuilt = false;
        }
        break;
      default:
        {}
    } // switch

  } else {

    // cout << "--> adding standalone note/rest to part sequence" << endl;
    // register note as standalone
    S_lpsrElement n = note;
    fCurrentVoice->appendElementToVoiceSequence (n);
  
    // account for chord not being built
    fAChordIsBeingBuilt = false;
  }
  */
  
   // keep track of note/rest in this visitor
  fCurrentNote    = note;
  fCurrentElement = fCurrentNote; // another name for it
}


} // namespace
