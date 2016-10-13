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
#include <algorithm>    /* for_each */

#include "xml_tree_browser.h"

#include "conversions.h"

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

  /*
  A first part is created with an empty name and
  all the nneded contents.
  It will be reused when the first actual part is met,
  changing its name on the fly
  in lpsrPartGroup::tryAndReUseInitialAnonymousPart()
  */
  
  // add the anonymous part group to the dictionary
  fCurrentPartGroupNumber = 1;
  fCurrentPartGroup =
    fDictionary->
      addPartGroupToDictionary (fCurrentPartGroupNumber);

  // add the anonymous part to the anonymous part group
  string partXMLName = "";
  fCurrentPart =
    fCurrentPartGroup->
      addPartToPartGroup (partXMLName);

  // add the anonymous staff to the anonymous part
  fCurrentStaffNumber = 1;
  fCurrentStaff =
    fCurrentPart->
      addStaffToPart (fCurrentStaffNumber);

  // add the anonymous voice to the anonymous staff
  fCurrentVoiceNumber = 1;
  fCurrentVoice =
    fCurrentStaff->
      addVoiceToStaff (fCurrentVoiceNumber);

  // add the anonymous lyrics to the anonymous voice
  fCurrentLyricNumber = 1;
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (fCurrentLyricNumber);
  
  fCurrentMeasureNumber = 0;

  fAChordIsBeingBuilt = false;
  fATupletIsBeingBuilt = false;
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
void xml2LpsrDictionaryVisitor::resetCurrentTime ()
{
  fCurrentTimeStaffNumber = -1;
  
  fSenzaMisura = false;

  fCurrentBeats = 0;
  fCurrentBeatType = 0;
  
//  fTimeSignatures.clear();
  fSymbol = "";
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
      cerr << "There is 1 division";
    else
      cerr <<
        "There are " << fCurrentMusicXMLDivisions <<
        " divisions";
    cerr <<
      " per quater note in part \"" <<
      fCurrentPart->getPartCombinedName () << endl;
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
      "unknown part group type \"" + fCurrentPartGroupType + "\"");
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

  fCurrentStemDirection = kStemNeutral;
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_time& elt ) {
  resetCurrentTime();
  
  fCurrentTimeStaffNumber =
    elt->getAttributeIntValue ("number", -1);
    
  fSymbol =
    elt->getAttributeValue ("symbol");
  // time symbol="cut" JMI
}

void xml2LpsrDictionaryVisitor::visitStart ( S_beats& elt )
  { fCurrentBeats = (int)(*elt); }
  
void xml2LpsrDictionaryVisitor::visitStart ( S_beat_type& elt )
  { fCurrentBeatType = (int)(*elt); }
 
void xml2LpsrDictionaryVisitor::visitStart ( S_senza_misura& elt )
  { fSenzaMisura = true; }

/*
rational xml2LpsrDictionaryVisitor::timeSignatureFromIndex(int index)
{
  rational r(0,1);
  if (index < fTimeSignatures.size()) {
    const pair<string,string>& ts = fTimeSignatures[index];
    int   num = strtol (ts.first.c_str(), 0, 10);
    int   denum = strtol (ts.second.c_str(), 0, 10);
    if (num && denum) r.set(num, denum);
  }
  return r;
}
*/

void xml2LpsrDictionaryVisitor::visitEnd ( S_time& elt ) 
{
  S_lpsrTime
    time =
      lpsrTime::create (
        fCurrentBeats,
        fCurrentBeatType,
        fTranslationSettings->fGenerateNumericalTime);
  S_lpsrElement t = time;
  fCurrentVoice->appendElementToVoiceSequence (t);
}

//______________________________________________________________________________

void xml2LpsrDictionaryVisitor::visitStart ( S_key& elt ) {
  fCurrentFifths = 0;
  fCurrentCancel = 0;
  fCurrentMode   = "";
}
  
void xml2LpsrDictionaryVisitor::visitStart ( S_fifths& elt )
  { fCurrentFifths = (int)(*elt); }
  
void xml2LpsrDictionaryVisitor::visitStart ( S_mode& elt )
  { fCurrentMode = elt->getValue(); }

void xml2LpsrDictionaryVisitor::visitStart ( S_cancel& elt )
  { fCurrentCancel = (int)(*elt); }

void xml2LpsrDictionaryVisitor::visitEnd ( S_key& elt ) 
{    
  // create lpsrKey and add it to part
  S_lpsrElement
    key =
      lpsrKey::create (fCurrentFifths, fCurrentMode, fCurrentCancel);
  S_lpsrElement k = key;
  fCurrentVoice->appendElementToVoiceSequence (k);
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_clef& elt )
{ 
  fLine = 0;;
  fOctaveChange = 0;
  fNumber = -1;
  fSign = "";

  fNumber = elt->getAttributeIntValue("number", -1); 
}

void xml2LpsrDictionaryVisitor::visitStart ( S_clef_octave_change& elt )
  { fOctaveChange = (int)(*elt); }
  
void xml2LpsrDictionaryVisitor::visitStart ( S_line& elt )
  { fLine = (int)(*elt); }
  
void xml2LpsrDictionaryVisitor::visitStart ( S_sign& elt )
  { fSign = elt->getValue(); }

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitEnd ( S_clef& elt ) 
{
  int staffNum = elt->getAttributeIntValue("number", 0); // JMI
  
  S_lpsrClef
    clef =
      lpsrClef::create (fSign, fLine, staffNum);
  S_lpsrElement c = clef;
  fCurrentVoice->appendElementToVoiceSequence (c);
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_metronome& elt )
{
  string parentheses = elt->getAttributeValue("parentheses");
  
  fBeatsData.clear();
  fPerMinute = 0;
  fCurrentBeat.fBeatUnit = "";
  fCurrentBeat.fDots = 0;

  if (parentheses.size()) {
    cout << "S_metronome, parentheses = " << parentheses << endl;
    
    if (parentheses == "yes") 
      fParentheses = true;
    else if (parentheses == "no")
      fParentheses = true;
    else {
      stringstream s;
      string  message;
      s << "parentheses value " << parentheses << " should be 'yes' or 'no'";
      s >> message;
      lpsrMusicXMLError (message);
    }
  }
}
  
void xml2LpsrDictionaryVisitor::visitEnd ( S_metronome& elt )
{ 
 // if (fSkipDirection) return;

  if (fCurrentBeat.fBeatUnit.size()) {
    fBeatsData.push_back(fCurrentBeat);
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  
  if (fBeatsData.size() != 1) {
    lpsrMusicXMLWarning(
      "multiple beats found, but only per-minute tempos is supported");
    return;         // support per minute tempo only (for now)
  }
  
  if (! fPerMinute) {
    lpsrMusicXMLWarning(
      "per-minute not found, only per-minute tempos is supported");
    return;    // support per minute tempo only (for now)
  }

  musicXMLBeatData b = fBeatsData[0];
  rational         r = 
    NoteType::type2rational(
      NoteType::xml(b.fBeatUnit)), rdot(3,2);
  
  while (b.fDots-- > 0) {
    r *= rdot;
  }
  r.rationalise();

  S_lpsrTempoCommand tempo =
    lpsrTempoCommand::create (r.getDenominator(), fPerMinute);
    
  fCurrentVoice->appendElementToVoiceSequence (tempo);
  
 // JMI if (fCurrentOffset) addDelayed(cmd, fCurrentOffset);
}

void xml2LpsrDictionaryVisitor::visitStart ( S_beat_unit& elt )
{ 
  if (fCurrentBeat.fBeatUnit.size()) {
    fBeatsData.push_back (fCurrentBeat); 
    fCurrentBeat.fBeatUnit = "";
    fCurrentBeat.fDots = 0;
  }
  fCurrentBeat.fBeatUnit = elt->getValue();
}

void xml2LpsrDictionaryVisitor::visitStart ( S_beat_unit_dot& elt )
  { fCurrentBeat.fDots++; }
  
void xml2LpsrDictionaryVisitor::visitStart ( S_per_minute& elt )
  { fPerMinute = (int)(*elt); }


//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_tied& elt )
{
//           <tied orientation="over" type="start"/>

fCurrentTiedType =
  elt->getAttributeValue ("type");

fCurrentTiedOrientation =
  elt->getAttributeValue ("orientation");
}

void xml2LpsrDictionaryVisitor::visitStart (S_slur& elt )
{
//          <slur number="1" placement="above" type="start"/>
  fCurrentSlurNumber =
    elt->getAttributeIntValue ("number", 0);

  fCurrentSlurType =
    elt->getAttributeValue ("type");

  fCurrentSlurPlacement =
    elt->getAttributeValue ("placement");
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_lyric& elt )
{ 
  fCurrentLyricNumber =
    elt->getAttributeIntValue ("number", 0);

  // is this lyrics already present?
  fCurrentLyrics =
    fCurrentVoice->
      voiceContainsLyrics (
        fCurrentLyricNumber);

  // no, add it to the current staff
  if (! fCurrentLyrics) 
  fCurrentLyrics =
    fCurrentVoice->
      addLyricsToVoice (
        fCurrentLyricNumber);

  fCurrentLyricsHasText = false;
  fCurrentText = "";
  fCurrentElision = false;

  fCurrentNoteHasLyrics = true;
}

void xml2LpsrDictionaryVisitor::visitStart ( S_syllabic& elt )
{
  fCurrentSyllabic = elt->getValue();
/* JMI
  if (fCurrentSyllabic == "begin") {
    fOnGoingLyrics = true;
  }
  else if (fCurrentSyllabic == "end") {
    fOnGoingLyrics = true;
  }
  */
}

void xml2LpsrDictionaryVisitor::visitEnd ( S_text& elt ) 
{
  string text = elt->getValue();

  // text may be composed of only spaces, so:
  string dest;
  std::for_each (
    text.begin(), text.end(), stringSpaceRemover (dest));

  if (fCurrentElision)
    fCurrentText += " "+dest;
  else
    fCurrentText = dest;

  fCurrentLyricsHasText = true;

/*
  cout <<
    "--> fCurrentLyricNumber = " << fCurrentLyricNumber <<
    ", fCurrentSyllabic = " << fCurrentSyllabic <<
    ", fCurrentText = |" << fCurrentText << "|" << endl;
*/
}

/*
      <note default-x="143">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>4</octave>
        </pitch>
        <duration>6</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem default-y="-5">up</stem>
        <beam number="1">begin</beam>
        <lyric default-y="-80" justify="left" number="1">
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">1.</text>
          <elision> </elision>
          <syllabic>begin</syllabic>
          <text font-family="FreeSerif" font-size="11">A</text>
        </lyric>
        <lyric default-y="-97" justify="left" number="2">
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">2.</text>
          <elision> </elision>
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">'T</text>
          <elision> </elision>
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">was</text>
        </lyric>
        <lyric default-y="-113" justify="left" number="3">
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">3.</text>
          <elision> </elision>
          <syllabic>single</syllabic>
          <text font-family="FreeSerif" font-size="11">Throug</text>
          <extend type="start"/>
        </lyric>
*/

void xml2LpsrDictionaryVisitor::visitEnd ( S_elision& elt ) 
{
  fCurrentElision = true;
/*
  cout <<
    "--> fCurrentLyricNumber = " << fCurrentLyricNumber <<
    ", fCurrentSyllabic = " << fCurrentSyllabic <<
    ", fCurrentText = |" << fCurrentText << "|" << endl;
*/
}

void xml2LpsrDictionaryVisitor::visitEnd ( S_lyric& elt ) {
/*
      <note default-x="61">
        <pitch>
          <step>D</step>
          <octave>4</octave>
        </pitch>
        <duration>8</duration>
        <voice>1</voice>
        <type>quarter</type>
        <stem default-y="-10">up</stem>
        <lyric default-y="-80" number="1">
          <syllabic>single</syllabic>
          <text>オノ</text>
        </lyric>
      </note>
*/

/* JMI lyric without text
        <lyric name="verse" number="3">
          <extend type="stop"/>
        </lyric>
*/

  if (fCurrentLyricsHasText) {
    S_lpsrDuration
      lyricLpsrDuration =
        lpsrDuration::create (
          fMusicXMLNoteData.fMusicxmlDuration,
          fCurrentMusicXMLDivisions,
          fMusicXMLNoteData.fDotsNumber,
          fMusicXMLNoteData.fTupletMemberType);
  /*
  create (
          int    num,
          int    denom,
          int    dots,
          string tupletMemberType);
          *
          *   fNoteLpsrDuration =
      lpsrDuration::create (
        fMusicXMLNoteData.fMusicxmlDuration,
        divisionsPerWholeNote,
        fMusicXMLNoteData.fDotsNumber,
        fMusicXMLNoteData.fTupletMemberType);
  
  */
  
    fCurrentLyrics->
      addTextChunkToLyrics (
        fCurrentSyllabic,
        fCurrentText,
        fCurrentElision,
        lyricLpsrDuration);
  }
}

//________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart (S_measure& elt)
{
  fCurrentMeasureNumber =
    elt->getAttributeIntValue ("number", 0);
    
  if (fTranslationSettings->fTrace)
    cerr <<
      "=== MEASURE " << fCurrentMeasureNumber << " === " <<
      "PART " << fCurrentPart->getPartCombinedName () <<" ===" << endl;
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_print& elt ) 
{
  const string& newSystem = elt->getAttributeValue ("new-system");
  
  if (newSystem == "yes") {
    // create a barnumbercheck command
    S_lpsrBarNumberCheck barnumbercheck_ =
      lpsrBarNumberCheck::create (fCurrentMeasureNumber);
    S_lpsrElement b2 = barnumbercheck_;
    fCurrentVoice->appendElementToVoiceSequence (b2);

    // create a break command
    S_lpsrBreak break_ = lpsrBreak::create(fCurrentMeasureNumber);
    S_lpsrElement b1 = break_;
    fCurrentVoice->appendElementToVoiceSequence (b1);
  
    // add a break chunk to the lyrics
    fCurrentLyrics->
      addBreakChunkToLyrics (fCurrentMeasureNumber);
  }
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
  //         <stem default-y="28.5">up</stem>

  string        stem = elt->getValue();
  StemDirection stemDirection;
  
  if (stem == "up")
    stemDirection = kStemUp;
  else if (stem == "down")
    stemDirection = kStemDown;
  else
    stemDirection = kStemNeutral; // JMI

  if (stemDirection != fCurrentStemDirection) {
    if (fTranslationSettings->fGenerateStems) {
      switch (stemDirection) {
        case kStemNeutral:
          // \stemNeutral JMI
          break;
        case kStemUp:
          // \stemUp JMI
          break;
        case kStemDown:
          // \stemDown JMI
          break;
      } // switch
    }
    fCurrentStemDirection = stemDirection;
  }
  
  fCurrentStem = stem;
}

void xml2LpsrDictionaryVisitor::visitStart ( S_beam& elt )
{
  //        <beam number="1">begin</beam>

  fCurrentBeam = elt->getValue();

  fCurrentBeamNumber = 
    elt->getAttributeIntValue ("number", 0);
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

  fCurrentStem = "";

  // assume this note hasn't got lyrics until S_lyric is met
  fCurrentNoteHasLyrics = false;
  
  // assume this note doesn't belong to a chord until S_chord is met
  fMusicXMLNoteData.fNoteBelongsToAChord = false;

  // assume this note doesn't belong to a tuplet until S_chord is met
  fMusicXMLNoteData.fNoteBelongsToATuplet = fATupletIsBeingBuilt;

  fCurrentTiedType = "";
  fCurrentTiedOrientation = "";

  fCurrentSlurNumber = "";
  fCurrentSlurType = "";
  fCurrentSlurPlacement = "";
}

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitStart ( S_rest& elt)
{
  //  cout << "--> xml2LpsrDictionaryVisitor::visitStart ( S_rest& elt ) " << endl;
  fMusicXMLNoteData.fMusicxmlStepIsARest = true;
}

S_lpsrChord xml2LpsrDictionaryVisitor::createChord (
  S_lpsrDuration noteDuration)
{
  // cout << "--> creating a chord on its 2nd note" << endl;
  
  // fCurrentNote has been registered standalone in the part element sequence,
  // but it is actually the first note of a chord
  
  // create a chord
  S_lpsrChord chord;
  
  chord = lpsrChord::create (noteDuration);
  fCurrentElement = chord; // another name for it
   
  if (fTranslationSettings->fDebug)
    cerr << "--> adding first note to chord" << endl;
    
  // register fCurrentNote as first member of chord
  chord->addNoteToChord (fCurrentNote);
  fCurrentNote->setNoteBelongsToAChord ();
  
  // move the pending dynamics if any from the first note to the chord
  list<S_lpsrDynamics>
    noteDynamics = fCurrentNote->getNoteDynamics();
    
  while (! noteDynamics.empty()) {
    //cout << "--> moving dynamics from fCurrentNote to chord" << endl;
    S_lpsrDynamics
      dyn = noteDynamics.front();
    chord->addDynamics (dyn);
    noteDynamics.pop_front ();
  } // while
 
  // move the pending wedges if any from the first note to the chord
  list<S_lpsrWedge>
    noteWedges = fCurrentNote->getNoteWedges();
    
  while (! noteWedges.empty()) {
    //cout << "--> moving wedge from fCurrentNote to chord" << endl;
    S_lpsrWedge
      wdg = noteWedges.front();
    chord->addWedge (wdg);
    noteWedges.pop_front();
  } // while

  return chord;
}

void xml2LpsrDictionaryVisitor::createTuplet (S_lpsrNote note)
{
  // create a tuplet element
  S_lpsrTuplet fCurrentTuplet = lpsrTuplet::create();
  fCurrentElement = fCurrentTuplet; // another name for it

  // populate it
  fCurrentTuplet->updateTuplet(
    fCurrentTupletNumber,
    fCurrentActualNotes,
    fCurrentNormalNotes);

  // register it in this visitor
  if (fTranslationSettings->fDebug)
    cout << "--> pushing tuplet to tuplets stack" << endl;
  fCurrentTupletsStack.push(fCurrentTuplet);
  
  // add note to the tuplet
  if (fTranslationSettings->fDebug)
    cout << "--> adding note " << note << " to tuplets stack top" << endl;
  fCurrentTuplet->addElementToTuplet(note);
}

void xml2LpsrDictionaryVisitor::finalizeTuplet (S_lpsrNote note) {
  // get tuplet from top of tuplet stack
  S_lpsrTuplet tup = fCurrentTupletsStack.top();

  // add note to the tuplet
  if (fTranslationSettings->fDebug)
    cout << "--> adding note " << note << " to tuplets stack top" << endl;
  tup->addElementToTuplet(note);

  // pop from the tuplets stack
  if (fTranslationSettings->fDebug)
    cout << "--> popping from tuplets stack" << endl;
  fCurrentTupletsStack.pop();        

  // add tuplet to the part
  if (fTranslationSettings->fDebug)
    cout << "=== adding tuplet to the part sequence" << endl;
  S_lpsrElement elem = tup;
  fCurrentVoice->appendElementToVoiceSequence (elem);
}          

//______________________________________________________________________________
void xml2LpsrDictionaryVisitor::visitEnd ( S_note& elt ) 
{
  if (fTranslationSettings->fDebug)
    cout <<
      "<-- xml2LpsrDictionaryVisitor::visitEnd ( S_note& elt ) " << endl;
      
  /*
  This is a complex method, due to the fact that
  dynamics, wedges, chords and tuplets
  are not ordered in the same way in MusicXML and LilyPond
  */
  
  if (fTranslationSettings->fDebug)
    cerr <<
      "fMusicXMLNoteData.fMusicxmlDuration = " << 
      fMusicXMLNoteData.fMusicxmlDuration << ", " << 
      "fCurrentMusicXMLDivisions*4 = " <<
      fCurrentMusicXMLDivisions*4 << endl;
      
  if (fCurrentMusicXMLDivisions <= 0)
    lpsrMusicXMLError ("divisions cannot be 0 nor negative");
  
  fMusicXMLNoteData.fMusicxmlDivisions =
    fCurrentMusicXMLDivisions;
  fMusicXMLNoteData.fTupletMemberType =
    fCurrentType;
  
  //cout << "::: creating a note" << endl;
  S_lpsrNote note =
    lpsrNote::createFromMusicXMLData (
      fTranslationSettings, fMusicXMLNoteData);

  // attach the pending dynamics if any to the note
  if (! fPendingDynamics.empty()) {
    if (fMusicXMLNoteData.fMusicxmlStepIsARest) {
      if (fTranslationSettings->fDelayRestsDynamics) {
      cerr <<
        "--> Delaying dynamics attached to a rest until next note";
     } else {
       cerr <<
          "--> There is dynamics attached to a rest";
    }
    } else {
      while (! fPendingDynamics.empty()) {
        S_lpsrDynamics dyn = fPendingDynamics.front();
        note->addDynamics(dyn);
        fPendingDynamics.pop_front();
      } // while
    }
  }
  
  // attach the pending wedges if any to the note
  if (! fPendingWedges.empty()) {
    if (fMusicXMLNoteData.fMusicxmlStepIsARest) {
      if (fTranslationSettings->fDelayRestsDynamics) {
      cerr <<
        "--> Delaying wedge attached to a rest until next note";
     } else {
       cerr <<
          "--> There is a wedge attached to a rest";
    }
    } else {
      while (! fPendingWedges.empty()) {
        S_lpsrWedge wdg = fPendingWedges.front();
        note->addWedge(wdg);
        fPendingWedges.pop_front();
      } // while
    }
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
      fCurrentChord =
        createChord (note->getNoteLpsrDuration ());

      // account for chord being built
      fAChordIsBeingBuilt = true;
    }
    
    if (fTranslationSettings->fDebug)
      cout << "--> adding note to fCurrentChord" << endl;
    // register note as a member of fCurrentChord
    fCurrentChord->addNoteToChord (note);
      
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
          if (fTranslationSettings->fDebug)
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
  
  // keep track of note/rest in this visitor
  fCurrentNote    = note;
  fCurrentElement = fCurrentNote; // another name for it

  // add a skip chunk for notes/rests without lyrics
  // do a copy, not to share things! JMI ???
  if (! fCurrentNoteHasLyrics) {
    /*
    lpsrDuration
      lyricsLpsrDuration =
        lpsrDuration (*(note->getNoteLpsrDuration ()));
    */
    
    S_lpsrDuration
      lyricsLpsrDuration =
        note->getNoteLpsrDuration ();

    fCurrentLyrics->
      addSkipChunkToLyrics (
        lyricsLpsrDuration);
  }
}


} // namespace
