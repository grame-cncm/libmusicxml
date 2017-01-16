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
#include <iomanip>      // setw, set::precision, ...
#include <algorithm>    /* for_each */

#include "xml_tree_browser.h"

#include "conversions.h"

#include "xml2PartsInformationExtractor.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
xml2PartsInformationExtractor::xml2PartsInformationExtractor (
  S_msrOptions& msrOpts)
{
  fMsrOptions = msrOpts;

  fMillimeters       = -1;
  fTenths            = -1;

  fCurrentDivisionsPerQuarterNote = 0;

  // create the MSR score
  fMsrScore =
    msrScore::create (fMsrOptions, 0);

  fCurrentWordsContents = "";

  fCurrentVoiceNumber = 1; // JMI
  
  fCurrentLyricsNumber = -1; // JMI
  fCurrentSyllabic = "";
  fCurrentText = "";
  fCurrentElision = false;
  
  fCurrentNoteStaffNumber = 0;
  fCurrentNoteVoiceNumber = 0;
  fOnGoingNote            = false;
}

xml2PartsInformationExtractor::~xml2PartsInformationExtractor ()
{}

//________________________________________________________________________
S_xmlPartsInformation xml2PartsInformationExtractor::extractPartsInformationFromXMLElementTree (
  const Sxmlelement& xmlTree)
{
  S_xmlPartsInformation result;
  
  if (xmlTree) {
    // create a tree browser on this visitor
    tree_browser<xmlelement> browser (this);
    
    // browse the xmlelement tree
    browser.browse (*xmlTree);
    
    // grab the resulting Score
    result = fMsrScore;
  }

  return result;
}

//________________________________________________________________________
S_msrPartgroup xml2PartsInformationExtractor::createImplicitMSRPartgroup (
  int inputLineNumber)
{
  /*
  A first part group is created with all the needed contents
  if none is specified in the MusicXML data.
  */

  // create an implicit part group
  fCurrentPartgroupNumber = 1;
  
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating an implicit part group with number " <<
      fCurrentPartgroupNumber <<
      endl;

  S_msrPartgroup
    partgroup =
      msrPartgroup::create (
        fMsrOptions,
        inputLineNumber,
        fCurrentPartgroupNumber,
        "Implicit",
        "Impl.",
        msrPartgroup::kBracketPartgroupSymbol,
        0,
        true,
        0); // the top level part group has an empty uplink

  /*
    this implicit part group will be added to the MSR score
    in method 'visitEnd (S_part_list& elt)'
  */
  
  // add implicit part group to the map of this visitor
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding implicit part group " << fCurrentPartgroupNumber <<
      " to visitor's data" <<
      endl;
      
  fPartgroupsMap [fCurrentPartgroupNumber] = partgroup;
  fPartgroupsList.push_front (partgroup);

  fImplicitPartgroup = partgroup;
  
  return partgroup;
}

//______________________________________________________________________________
S_msrPartgroup xml2PartsInformationExtractor::fetchPartgroupInThisVisitor (
  int partgroupNumber)
{
  S_msrPartgroup result;
  
  if (fPartgroupsMap.count (partgroupNumber)) {
    result =
      fPartgroupsMap [partgroupNumber];
  }

  return result;
}

//______________________________________________________________________________
S_msrStaff xml2PartsInformationExtractor::createStaffInCurrentPartIfNeeded (
  int            inputLineNumber,
  int            staffNumber)
{    
  // is staffNumber already present in part?
  S_msrStaff
    staff =
      fCurrentPart->
        fetchStaffFromPart (staffNumber);

  if (! staff) 
    // no, add it to fCurrentPart
    staff =
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber, staffNumber);

  return staff;
}  

//______________________________________________________________________________
S_msrVoice xml2PartsInformationExtractor::registerVoiceInStaffInCurrentPartIfNeeded (
  int            inputLineNumber,
  int            staffNumber,
  int            voiceNumber)
{
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        inputLineNumber,
        staffNumber);

  // is voiceNumber already present in staff?
  S_msrVoice
    voice =
      staff->
        fetchVoiceFromStaff (
          inputLineNumber, voiceNumber);

  if (! voice) 
    // no, add it to the staff
    voice =
      staff->
        registerVoiceInStaffByItsExternalNumber (
          inputLineNumber, voiceNumber);

  return voice;
}  

//______________________________________________________________________________
void xml2PartsInformationExtractor::visitStart ( S_millimeters& elt )
{ 
  fMillimeters = (int)(*elt);
  
  fMsrScore->getPageGeometry ()->
    setMillimeters (fMillimeters);
}

void xml2PartsInformationExtractor::visitStart ( S_tenths& elt )
{
  fTenths = (int)(*elt);

  fMsrScore->getPageGeometry ()->
    setTenths (fTenths);
}

void xml2PartsInformationExtractor::visitEnd ( S_scaling& elt)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "There are " << fTenths <<
      " tenths for " <<  fMillimeters <<
      " millimeters, hence the global staff size is " <<
      fMsrScore->getPageGeometry ()->globalStaffSize () <<
      endl;
}

//________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_part_list& elt)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Analysing part list" <<
      endl;

  idtr++;
}

void xml2PartsInformationExtractor::visitEnd (S_part_list& elt)
{
  idtr--;

  if (fImplicitPartgroup) {
    // force an implicit part group "stop" on it
    // fCurrentPartgroupNumber holds the value 1
    handlePartgroupStop (elt->getInputLineNumber ());
    
    fImplicitPartgroup = 0;
  }
}

//________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_score_part& elt)
{
  fCurrentPartID = elt->getAttributeValue ("id");

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "Found part name \"" << fCurrentPartID << "\"" <<
      endl;

  fCurrentPartName         = "";
  fCurrentPartAbbreviation = "";
  
  fCurrentPartInstrumentName = "";
}

void xml2PartsInformationExtractor::visitStart (S_part_name& elt)
{
  fCurrentPartName = elt->getValue ();
}

void xml2PartsInformationExtractor::visitStart (S_part_abbreviation& elt)
{
  fCurrentPartAbbreviation = elt->getValue ();
}

void xml2PartsInformationExtractor::visitStart (S_instrument_name& elt)
{
  fCurrentPartInstrumentName = elt->getValue();
}

void xml2PartsInformationExtractor::visitEnd (S_score_part& elt)
{
  string partID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTrace)
   cerr <<
      idtr <<
      "--------------------------------------------" <<
      endl <<
      idtr <<
      "Handling part \"" << fCurrentPartID << "\"" <<
      endl;

  idtr++;

  S_msrPartgroup partgroup;

  // is there a current part group?
  if (! fPartgroupsList.size()) {
    // no, create an implicit one
    partgroup =
      createImplicitMSRPartgroup (
        inputLineNumber);
  }

  // fetch current part group
  try {
    partgroup = fPartgroupsList.front ();
  }
  catch (int e) {
    cerr <<
      "An exception number " << e << " occurred" <<
      endl;
  }
        
  // is this part already present in the current part group?
  S_msrPart
    part =
      partgroup->
        fetchPartFromPartgroup (fCurrentPartID);

  if (! part) {
    // no, add it to the current part group
    part =
      partgroup->
        addPartToPartgroupByItsID (
          inputLineNumber, fCurrentPartID);
  }
  
  // populate current part
  // fPartMSRName has already been set by the constructor // JMI
  part->
    setPartName (fCurrentPartName);
  part->
    setPartAbbreviation (fCurrentPartAbbreviation);
  part->
    setPartInstrumentName (fCurrentPartInstrumentName);

  // register it in this visitor's parts map
  fPartsMap [partID] = part;

  if (fImplicitPartgroup) {
    // force an implicit part group "stop" on it
    // fCurrentPartgroupNumber hold the value 1
    handlePartgroupStop (
      inputLineNumber);

    // forget the implicit group
    fImplicitPartgroup = 0;
  }
    
  showPartgroupsData (
    "AFTER handling part \"" + partID + "\"");

  idtr--;
}

//________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_part& elt)
{
  // fCurrentPartID is used throughout
  fCurrentPartID = elt->getAttributeValue ("id");

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  // is this part already known?
  if (fPartsMap.count (fCurrentPartID))
    fCurrentPart = // used thoughout
      fPartsMap [fCurrentPartID];
      
  else
    msrInternalError (
      inputLineNumber,
      "part " +
        fCurrentPartID +
        " is not registered in this visitor's part map");

  if (gGeneralOptions->fTrace)
    cerr <<
      idtr <<
        "--------------------------------------------" <<
        endl <<
      idtr <<
        "Analyzing part \"" << fCurrentPartID << "\" -- start" <<
      endl;

  idtr++;

  fCurrentStaffNumber = 1; // default if there are no <staff> element
  fCurrentVoiceNumber = 1; // default if there are no <voice> element

  fOnGoingRepeat = false;
}

void xml2PartsInformationExtractor::visitEnd (S_part& elt)
{
  if (gGeneralOptions->fTrace)
    cerr <<
      idtr <<
        "Analyzing part \"" << fCurrentPartID << "\" -- end" <<
        endl <<
      idtr <<
        "--------------------------------------------" <<
        endl <<
      endl;

  // all voices were generated from the start, // JMI
  // but some may have remained empty
  fCurrentPart->
    removePartEmptyVoices ();

  idtr--;
}

//______________________________________________________________________________
void xml2PartsInformationExtractor::visitStart ( S_divisions& elt ) 
{
  fCurrentDivisionsPerQuarterNote = (int)(*elt);
  
  if (fCurrentDivisionsPerQuarterNote <= 0)
    msrMusicXMLError (
      elt->getInputLineNumber (),
      "divisions per quarter note should be positive");
  
  if (gGeneralOptions->fTrace) {
    cerr << idtr;
    if (fCurrentDivisionsPerQuarterNote== 1)
      cerr << "There is 1 division";
    else
      cerr <<
        "There are " << fCurrentDivisionsPerQuarterNote<<
        " divisions";
    cerr <<
      " per quater note in part " <<
      fCurrentPart->getPartCombinedName() <<
      endl;
  }

  fCurrentPart->
    setPartDivisionsPerWholeNote (
      fCurrentDivisionsPerQuarterNote * 4);
}

//______________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_words& elt)
{
/*
          <words default-y="-73" font-style="italic" relative-x="5">cresc.</words>
*/

  fCurrentWordsContents = elt->getValue ();

  fCurrentFontStyle   = elt->getAttributeValue ("font-style");
  fCurrentFontSize    = elt->getAttributeValue ("font-size");
  fCurrentFontWeight  = elt->getAttributeValue ("font-weight");
  fCurrentFontXMLLang = elt->getAttributeValue ("xml:lanG");

  if (! fCurrentFontXMLLang.size ())
    fCurrentFontXMLLang = "it"; // default value

  if (fCurrentWordsContents.size ()) {
    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Creating words \"" << fCurrentWordsContents << "\"" <<
        ", placement = \"" << fCurrentDirectionPlacement << "\"" <<
        endl;

    S_msrWords
      words =
        msrWords::create (
          fMsrOptions,
          elt->getInputLineNumber (),
          fCurrentWordsPlacementKind,
          fCurrentWordsContents,
          fCurrentFontStyle,
          fCurrentFontSize,
          fCurrentFontWeight,
          fCurrentFontXMLLang);

    fPendingWords.push_back (words);
  }
}

//________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_staves& elt)
{
  int stavesNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  if (gGeneralOptions->fTrace) {
    switch (stavesNumber) {
      case 0:
        cerr << idtr <<
          "There isn't any explicit staff (hence 1 by default)"; // JMI
        break;
      case 1:
        cerr << idtr <<
          "There is 1 staff";
        break;
      default:
        cerr << idtr <<
          "There are " << stavesNumber << " staves";
    } // switch
    cerr <<
      " in part " << fCurrentPart->getPartCombinedName() <<
      endl;
  }

  if (stavesNumber > 1) {
    // add n-1 staves to current part since 1 already exists
    int n = 2;
    
    while (n <= stavesNumber) {
      fCurrentPart->
        addStaffToPartByItsNumber (
          inputLineNumber, n);
      n++;
    } // while
  }
}

//________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_staff& elt)
{
  /*
        <note>
        <pitch>
          <step>A</step>
          <octave>3</octave>
        </pitch>
        <duration>2</duration>
        <voice>3</voice>
        <type>eighth</type>
        <stem>down</stem>
        <staff>2</staff>
        <beam number="1">end</beam>
      </note>
*/
  fCurrentStaffNumber = int(*elt);

  int inputLineNumber =
    elt->getInputLineNumber ();

  // the staff number should be positive
  if (fCurrentStaffNumber <= 0) {
    stringstream s;

    s <<
      "staff number " << fCurrentStaffNumber <<
      " is not positive";
      
    msrAssert (false, s.str());
  }
  
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        inputLineNumber, fCurrentStaffNumber);

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
    cerr <<
      idtr <<
        "--> S_staff, fCurrentStaffNumber = " <<
        fCurrentStaffNumber << endl <<
      idtr <<
      "--> S_staff, current staff name  = " <<
        staff->getStaffName() <<
      endl;
  }

  if (fOnGoingForward) {

    fCurrentForwardStaffNumber = fCurrentStaffNumber;

  }
  
  else if (fOnGoingNote) {

    // regular staff indication in note/rest
    fCurrentNoteStaffNumber = fCurrentStaffNumber;

  }
  
  else if (fOnGoingDirection) {

    // regular staff indication in <direction/>, such as <staff/>
    fCurrentDirectionStaffNumber = fCurrentStaffNumber;
    
  }
  
  else if (fOnGoingDirectionType) {

    // JMI ???
    
  }
  
  else {
    
    stringstream s;
    
    s << "staff " << fCurrentStaffNumber << " is out of context";
    
    msrMusicXMLError (
// JMI    msrMusicXMLWarning (
      inputLineNumber,
      s.str());    
  }
}
    
//________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_voice& elt )
{
  /*
        <note>
        <pitch>
          <step>A</step>
          <octave>3</octave>
        </pitch>
        <duration>2</duration>
        <voice>3</voice>
        <type>eighth</type>
        <stem>down</stem>
        <staff>2</staff>
        <beam number="1">end</beam>
      </note>
*/
  fCurrentVoiceNumber = int(*elt);
  
  int inputLineNumber =
    elt->getInputLineNumber ();

/* JMI
  // the voice number should be in the 1..4 range
  if (fCurrentVoiceNumber < 1 || fCurrentVoiceNumber > 4) {
    stringstream s;

    s <<
      "voice number " << fCurrentVoiceNumber <<
      " is not in the 1..4 range";
      
    msrAssert (false, s.str());
  }
  */
  
  if (fOnGoingForward) {

    fCurrentForwardVoiceNumber = fCurrentVoiceNumber;

    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentForwardVoiceNumber);
  
  //  if (gGeneralOptions->fDebug)
      cerr <<
        idtr <<
          "--> S_voice, fCurrentVoiceNumber         = " <<
          fCurrentVoiceNumber << endl <<
        idtr <<
          "--> S_voice, fCurrentForwardVoiceNumber = " <<
          fCurrentForwardVoiceNumber << endl <<
        idtr <<
          "--> S_voice, current staff name  = " <<
          staff->getStaffName() <<
        endl;
  }
  
  else if (fOnGoingNote) {

    // regular voice indication in note/rest
    fCurrentNoteVoiceNumber = fCurrentVoiceNumber;

    S_msrStaff
      staff =
        createStaffInCurrentPartIfNeeded (
          inputLineNumber, fCurrentNoteStaffNumber);
  
    if (gGeneralOptions->fDebug)
      cerr <<
        idtr <<
          "--> fCurrentNoteVoiceNumber        = " <<
          fCurrentNoteVoiceNumber <<
          endl <<
        idtr <<
          "--> S_voice, fCurrentNoteStaffNumber = " <<
          fCurrentNoteStaffNumber <<
          endl <<
        idtr <<
          "--> S_voice, current staff name  = " <<
          staff->getStaffName() <<
        endl;
/*
    currentVoice = // ???
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);
  */  
  }
  
  else {
    
    stringstream s;
    
    s << "voice " << fCurrentVoiceNumber << " is out of context";
    
    msrMusicXMLError (
      inputLineNumber,
      s.str());
  }
}

//________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_lyric& elt )
{ 
  fCurrentLyricsNumber =
    elt->getAttributeIntValue ("number", 0);

  if (fCurrentLyricsNumber < 0) {
    stringstream s;

    s <<
      "lyric number " << fCurrentLyricsNumber <<
      " is not positive";

    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
  
  fCurrentLyricsHasText = false;
  fCurrentElision = false;

  fCurrentNoteHasLyrics = true;
}

void xml2PartsInformationExtractor::visitStart ( S_syllabic& elt )
{
  fCurrentSyllabic = elt->getValue();
  
  if      (fCurrentSyllabic == "single")
    fCurrentLyricschunkKind = msrLyricschunk::kSingleChunk;
  else if (fCurrentSyllabic == "begin")
    fCurrentLyricschunkKind = msrLyricschunk::kBeginChunk;
  else if (fCurrentSyllabic == "middle")
    fCurrentLyricschunkKind = msrLyricschunk::kMiddleChunk;
  else if (fCurrentSyllabic == "end")
    fCurrentLyricschunkKind = msrLyricschunk::kEndChunk;
  else {
    stringstream s;
    
    s << "--> syllabic \"" << fCurrentSyllabic << "\" is unknown";
    
    msrMusicXMLError (
      elt->getInputLineNumber (),
      s.str());
  }
}

void xml2PartsInformationExtractor::visitEnd ( S_text& elt ) 
{
  string text = elt->getValue();

  // text may be composed of only spaces, so:
  string dest;
  for_each (
    text.begin(), text.end(), stringSpaceRemover (dest));

  if (fCurrentElision)
    fCurrentText += " " + dest; // append to a list? JMI
  else
    fCurrentText = dest;

  fCurrentLyricsHasText = true;

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> line " << right << setw(5) << elt->getInputLineNumber () <<
      ", fCurrentLyricsNumber" << " = " << fCurrentLyricsNumber <<
      ", fCurrentSyllabic" << " = " << left << setw(6) << fCurrentSyllabic <<
      ", fCurrentText" << " = |" << fCurrentText << "|" << endl;
}

void xml2PartsInformationExtractor::visitEnd ( S_elision& elt ) 
{
  fCurrentElision = true;
}

void xml2PartsInformationExtractor::visitEnd ( S_lyric& elt )
{
   int inputLineNumber =
      elt->getInputLineNumber ();

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
 // JMI if (true) {  
    cerr <<
      endl <<
      idtr <<
        "Handling lyrics" <<
        ", line = " << inputLineNumber << ", with:" <<
        endl;

    idtr++;

    cerr <<
      fNoteData <<
        
      idtr <<
        setw(27) << "fCurrentLyricsNumber" << " = " << fCurrentLyricsNumber <<
        endl <<
      idtr <<
        setw(27) << "fCurrentText" << " = \"" << fCurrentText <<
        "\":" << fNoteData.fDivisions << ", " <<
        endl <<
      idtr <<
        setw(27) << "fCurrentElision" << " = " << fCurrentElision <<
        endl <<
      idtr <<
        setw(27) << "fNoteData.fStepIsARest" << " = ";
    if (fNoteData.fStepIsARest)
      cerr << "true";
    else
      cerr << "false";
    cerr <<
      endl;

    cerr <<
      idtr <<
        setw(27) << "fCurrentTieKind" << " = " <<
        msrTie::tieKindAsString (fCurrentTieKind) <<
      endl;
        
    cerr <<
      idtr <<
        setw(27) << "fCurrentSlurKind" << " = \"";
    switch (fCurrentSlurKind) {
      case msrSlur::kStartSlur:
        cerr << "start";
        break;
      case msrSlur::kContinueSlur:
        cerr << "start";
        break;
      case msrSlur::kStopSlur:
        cerr << "start";
        break;
      case msrSlur::k_NoSlur:
        cerr << "NO_SLUR";
        break;
    } // switch
    cerr << "\"" << endl;

    cerr <<
      idtr <<
        setw(27) <<
        "fOnGoingSlur" << " = " << fOnGoingSlur <<
        endl <<
      idtr <<
        setw(27) <<
        "fOnGoingSlurHasLyrics" << " = " << fOnGoingSlurHasLyrics <<
        endl;

    cerr <<
      idtr <<
        setw(27) << "fFirstLyricschunkInSlurKind" << " = \"" <<
        fFirstLyricschunkInSlurKind << // JMI->lyricschunkKindAsString () <<
        "\"" << endl;

    cerr <<
      idtr <<
        setw(27) << "fCurrentLyricschunkKind" << " = \""<<
        fCurrentLyricschunkKind << // JMI ->lyricschunkKindAsString () <<
        "\"" << endl;
        
    idtr--;
  }

  if      (fCurrentSyllabic == "single")
    fCurrentLyricschunkKind = msrLyricschunk::kSingleChunk;
  else if (fCurrentSyllabic == "begin")
    fCurrentLyricschunkKind = msrLyricschunk::kBeginChunk;
  else if (fCurrentSyllabic == "middle")
    fCurrentLyricschunkKind = msrLyricschunk::kMiddleChunk;
  else if (fCurrentSyllabic == "end")
    fCurrentLyricschunkKind = msrLyricschunk::kEndChunk;
  else {
    // no <syllabic /> specified for this note
    fCurrentLyricschunkKind = msrLyricschunk::k_NoChunk;
  }

  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentVoiceNumber);

  S_msrLyricschunk
    lyricschunk;

  if (fCurrentLyricschunkKind != msrLyricschunk::k_NoChunk) {

 //   string lyricschunkKindAsString;
    
    if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebug) {
      /*
      cerr <<
        ", type = \"" << lyricschunkKindAsString << "\"" <<
        ", elision: " << fCurrentElision << 
        " to " << getLyricsName () << endl;
*/
    }

    // create the lyrics chunk
    lyricschunk =
      currentVoice->
        addTextLyricschunkToVoice (
          inputLineNumber,
          fCurrentLyricsNumber,
          fCurrentSyllabic,
          fCurrentLyricschunkKind,
          fCurrentText,
          fCurrentElision,
          fNoteData.fDivisions);
  
    if (fOnGoingSlur)
      fOnGoingSlurHasLyrics = true;
      
    fCurrentNoteHasLyrics = true;
  }
  
  else {

    if (
      fCurrentSlurKind == msrSlur::kStartSlur
        &&
      fCurrentNoteHasLyrics) { // JMI
    }
    
    if (fCurrentTieKind != msrTie::k_NoTie) {
      fCurrentLyricschunkKind = msrLyricschunk::kTiedChunk;
      
      lyricschunk =
        currentVoice->
          addTiedLyricschunkToVoice (
            inputLineNumber,
            fCurrentLyricsNumber,
            fNoteData.fDivisions);
    }
  
    else if (fNoteData.fStepIsARest) {
      fCurrentLyricschunkKind = msrLyricschunk::kSkipChunk;

      lyricschunk =
        currentVoice->
          addSkipLyricschunkToVoice (
            inputLineNumber,
            fCurrentLyricsNumber,
            fNoteData.fDivisions);
    }
  
    else if (
      fOnGoingSlurHasLyrics
        &&
      ! fCurrentText.size ()) {
      if (fFirstLyricschunkInSlurKind == msrLyricschunk::kEndChunk) {
        fCurrentLyricschunkKind = msrLyricschunk::kSlurBeyondEndChunk;
  
        lyricschunk =
          currentVoice->
            addSlurBeyondEndLyricschunkToVoice ( 
              inputLineNumber,
              fCurrentLyricsNumber,
              fNoteData.fDivisions);
      }
      else {        
        fCurrentLyricschunkKind = msrLyricschunk::kSlurChunk;
  
        lyricschunk =
          currentVoice->
            addSlurLyricschunkToVoice ( 
              inputLineNumber,
              fCurrentLyricsNumber,
              fNoteData.fDivisions);
      }
    }
    
    else if (fOnGoingSlur) {
      if (fFirstLyricschunkInSlurKind == msrLyricschunk::kEndChunk) {
        fCurrentLyricschunkKind = msrLyricschunk::kSlurBeyondEndChunk;
  
        lyricschunk =
          currentVoice->
            addSlurBeyondEndLyricschunkToVoice (
              inputLineNumber,
              fCurrentLyricsNumber,
              fNoteData.fDivisions);
      }
      else {        
        fCurrentLyricschunkKind = msrLyricschunk::kSlurChunk;
  
        lyricschunk =
          currentVoice->
            addSlurLyricschunkToVoice ( 
              inputLineNumber,
              fCurrentLyricsNumber,
              fNoteData.fDivisions);
      }
    }
    
    else { // JMI
    }
  }

  if (lyricschunk)
    // register lyrics chunk in current note's lyric chunks list
    fCurrentNoteLyricchunks.push_back (lyricschunk);
}

//________________________________________________________________________
void xml2PartsInformationExtractor::visitStart (S_measure& elt)
{
/*
  Measures have a required number attribute (going from partwise to timewise,
  measures are grouped via the number).
*/

  int inputLineNumber =
    elt->getInputLineNumber ();

  int measureNumber =
      elt->getAttributeIntValue ("number", -1);

  // Measures with an implicit attribute set to "yes"
  // never display a measure number,
  // regardless of the measure-numbering setting.
  string
    implicit =
      elt->getAttributeValue ("implicit");

  // set part measure number
  fCurrentPart->
    setPartMeasureNumber (
      inputLineNumber, measureNumber);

  // is this measure number in the debug set?
  set<int>::iterator
    it =
      gGeneralOptions->
        fDebugMeasureNumbersSet.find (measureNumber);
        
  if (it != gGeneralOptions->fDebugMeasureNumbersSet.end ()) {
    // yes, activate debug for it
    gGeneralOptions->fSaveDebug = gGeneralOptions->fDebug;
    gGeneralOptions->fSaveDebugDebug = gGeneralOptions->fDebugDebug;
  }


  if (measureNumber > 2000) { // JMI
    cout <<
      endl <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      endl <<
      fCurrentPart <<
      "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" <<
      endl << endl;
    cout << 0/0;
  }

    /* JMI
  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentStaffNumber,
        fCurrentVoiceNumber);

  // populate current voice
  currentVoice->
    setVoiceDivisionsPerWholeNote (
      fCurrentDivisionsPerQuarterNote);
    
*/

/*


    
  if (gGeneralOptions->fDebug)
    cerr << idtr << 
      "=== MEASURE " <<
      currentVoice->
        getVoiceMeasureLocation ().fMeasureNumber <<
      " === " <<
      "PART " << fCurrentPart->getPartCombinedName () <<
      " ===" <<
      endl;

  if (measureNumber != 0) {
    // don't generate a bar check after the anacrusis
    S_msrBarCheck
      barCheck =
        msrBarCheck::create (
          fMsrOptions,
          inputLineNumber,
          currentVoice->
            getVoiceMeasureLocation ().fMeasureNumber);
              
    // append it to the voice
    if (currentVoice) {
      // it may not have been created yet JMI
      currentVoice->
        appendBarCheckToVoice (barCheck);
/ *
      // add a break chunk to the voice master lyrics
      currentVoice->
        getVoiceMasterLyrics ()->
          addBarCheckChunkToLyrics (
            inputLineNumber,
            currentVoice->getVoiceMeasureLocation ().fMeasureNumber);
            * /
    }
  }
  */
}

void xml2PartsInformationExtractor::visitEnd (S_measure& elt)
{
    // finalize last part measure to update master voice
  fCurrentPart->
    finalizeLastMeasureOfPart (
      elt->getInputLineNumber ());

  // restore debug options in case they were set in visitStart()
  gGeneralOptions->fDebug = gGeneralOptions->fSaveDebug;
  gGeneralOptions->fDebugDebug = gGeneralOptions->fSaveDebugDebug;
}

//______________________________________________________________________________
void xml2PartsInformationExtractor::visitStart ( S_note& elt ) 
{
  //  cerr << "--> xml2PartsInformationExtractor::visitStart ( S_note& elt ) " << endl;

  // initialize note data to a neutral state
  fNoteData.init ();

  // assuming staff number 1, unless S_staff states otherwise afterwards
  fCurrentStaffNumber = 1;

  // assuming voice number 1, unless S_voice states otherwise afterwards
  fCurrentVoiceNumber = 1;

  fCurrentNoteType = "";

  fCurrentLyricsNumber = -1;
  fCurrentSyllabic = "";
  fCurrentText = "";  
  // assume this note hasn't got lyrics until S_lyric is met
  fCurrentNoteHasLyrics = false;

  fCurrentBeam = 0;

  fCurrentTie = 0;
  fCurrentTiedOrientation = "";
  
  fCurrentSlurNumber = -1;
  fCurrentSlurType = "";
  fCurrentSlurPlacement = "";
  fCurrentSlurKind = msrSlur::k_NoSlur;

  fCurrentNoteStaffNumber = 1; // it may be absent
  fCurrentNoteVoiceNumber = 1; // it may be absent
  
  fOnGoingNote = true;
}

//______________________________________________________________________________
void xml2PartsInformationExtractor::visitStart ( S_rest& elt)
{
  /*
        <note>
        <rest/>
        <duration>24</duration>
        <voice>1</voice>
      </note>
*/
  //  cerr << "--> xml2PartsInformationExtractor::visitStart ( S_rest& elt ) " << endl;
  fNoteData.fStepIsARest = true;
}

//______________________________________________________________________________
void xml2PartsInformationExtractor::visitEnd ( S_note& elt )
{
  /*
  This is a complex method, due to the fact that
  dynamics, wedges, chords and tuplets
  are not ordered in the same way in MusicXML and LilyPond.

  Staff number is analyzed before voice number but occurs
  after it in the MusicXML tree.
  That's why the treatment below has been postponed until this method
  */

  /*
  Staff assignment is only needed for music notated on multiple staves.
  Used by both notes and directions.
  Staff values are numbers, with 1 referring to the top-most staff in a part.
  */
  
  int inputLineNumber =
    elt->getInputLineNumber ();

/*
  // fetch current staff
  S_msrStaff
    staff =
      createStaffInCurrentPartIfNeeded (
        inputLineNumber, fCurrentNoteStaffNumber);
*/
  // fetch current voice
  S_msrVoice
    currentVoice =
      registerVoiceInStaffInCurrentPartIfNeeded (
        inputLineNumber,
        fCurrentNoteStaffNumber,
        fCurrentNoteVoiceNumber);

  if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
    cerr <<
      endl <<
      idtr <<
        "==> BEFORE visitEnd (S_note&)" <<
        ", line " << inputLineNumber <<
        " we have:" <<
        endl;

    idtr++;

    cerr <<
      idtr <<
        setw(27) << "--> fCurrentNoteStaffNumber" << " = " <<
        fCurrentNoteStaffNumber <<
        endl <<
      idtr <<
        setw(27) << "--> fCurrentNoteVoiceNumber" << " = " <<
        fCurrentNoteVoiceNumber <<
        endl <<
      idtr <<
        setw(27) << "--> current voice" << " = \"" <<
        currentVoice->getVoiceName () << "\"" <<
      endl;

    idtr--;

    cerr <<
      idtr <<
        "<==" <<
        endl << endl;
  }

  int divisionsPerWholeNote =
    fCurrentDivisionsPerQuarterNote * 4;

  // store voice and staff numbers in MusicXML note data
  fNoteData.fStaffNumber = fCurrentStaffNumber;
  fNoteData.fVoiceNumber = fCurrentVoiceNumber;

  // set note's divisions per whole note
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "fNoteData.fDivisions = " << 
      fNoteData.fDivisions << ", " << 
      "divisionsPerWholeNote = " <<
      divisionsPerWholeNote << endl;
      
  currentVoice->setVoiceDivisionsPerWholeNote (
    divisionsPerWholeNote);

  // register current note type
  fNoteData.fType =
    fCurrentNoteType;

  if (fNoteData.fType.size ()) {
    if (fNoteData.fNoteIsAGraceNote) {
      // set current grace note divisions
      string errorMessage;
      
      fNoteData.fDivisions =
        noteTypeAsDivisions (
          fCurrentNoteType,
          divisionsPerWholeNote,
          errorMessage,
          false); // 'true' to debug it
  
      if (errorMessage.size ())
        msrMusicXMLError (
          inputLineNumber,
          errorMessage);
  
      fNoteData.fDisplayDivisions =
        fNoteData.fDivisions;
    }
  }

  // create the note
  S_msrNote
    note =
      msrNote::createFromNoteData (
        fMsrOptions,
        inputLineNumber,
        fNoteData);

  // set note's divisions per whole note
  note->
    setNoteDivisionsPerWholeNote (
      currentVoice-> getVoiceDivisionsPerWholeNote ());

  // set its tie if any
  if (fCurrentTie) {
    note->
      setNoteTie (fCurrentTie);
  }
  
  // set its stem if any
  if (fCurrentStem)
    note->
      setNoteStem (fCurrentStem);

  // add its beam if any
  if (fCurrentBeam)
    note->
      addBeamToNote (fCurrentBeam);

  // attach the articulations if any to the note
  attachCurrentArticulationsToNote (note);

  // attach the ornaments if any to the note
  attachCurrentOrnamentsToNote (note);

  /*
  A rest can be standalone or belong to a tuplet

  A note can be standalone or a member of a chord
  and the latter can belong to a tuplet,
  hence a note of a chord inside a tuplet is to be
  displayed as a note in a tuplet but outside of chord
  */

  // are the display divisions different than the duration?
  if (fNoteData.fNoteBelongsToATuplet)
    // set tuplet member note display divisions
    note->
      applyTupletMemberDisplayFactor (
        fCurrentActualNotes, fCurrentNormalNotes);

  // handle note
  if (fNoteData.fNoteBelongsToAChord) {
    // chord member note
    handleNoteBelongingToAChord (note);
  }
  
  else if (fNoteData.fNoteBelongsToATuplet) {
    // tuplet member note
    handleNoteBelongingToATuplet (note);
  }
  
  else {
    // standalone or grace note or rest
    handleStandaloneOrGraceNoteOrRest (note);
  }

 // JMI if (gGeneralOptions->fForceDebug || gGeneralOptions->fDebugDebug) {
  if (gGeneralOptions->fDebugDebug) {
    cerr <<
      endl <<
      idtr <<
        "==> AFTER visitEnd (S_note&) " <<
        note->noteAsString () <<
        ", line " << inputLineNumber <<
        " we have:" <<
        endl <<
      idtr << idtr <<
        setw(27) << "--> fCurrentNoteStaffNumber" << " = " <<
        fCurrentNoteStaffNumber <<
        endl <<
      idtr << idtr <<
        setw(27) << "--> fCurrentNoteVoiceNumber" << " = " <<
        fCurrentNoteVoiceNumber <<
        endl <<
      idtr << idtr <<
        setw(27) << "--> current voice" << " = \"" <<
        currentVoice->getVoiceName () << "\"" <<
        endl <<
        "<==" <<
        endl <<
      endl;
  }

  if (! fNoteData.fNoteBelongsToAChord) {
    if (fOnGoingNote) {
      // this is the first note after the chord

      // forget about this chord
      fCurrentChord = 0;
      fOnGoingChord = false;
    }
  }

  // keep track of current note in the current voice,
  // in case we learn later by <chord/> in the next note
  // that it is actually the first note of a chord
  if (gGeneralOptions->fDebugDebug) {
    displayLastHandledNoteInVoice (
      "############## Before fLastHandledNoteInVoice");
  }
  fLastHandledNoteInVoice [currentVoice] = note;
  if (gGeneralOptions->fDebugDebug) {
    displayLastHandledNoteInVoice (
      "############## After  fLastHandledNoteInVoice");
  }
    
  fOnGoingNote = false;
}

//______________________________________________________________________________
void xml2PartsInformationExtractor::handleLyrics (S_msrNote newNote)
{
  if (fCurrentNoteLyricchunks.size ()) {
    for (
      list<S_msrLyricschunk>::const_iterator i =
        fCurrentNoteLyricchunks.begin();
      i != fCurrentNoteLyricchunks.end();
      i++ ) {
      // set lyrics chunk note uplink to newNote
      (*i)->setLyricschunkNote (newNote);
    } // for
  }

  // forget all of newNote's lyric chunks
  fCurrentNoteLyricchunks.clear ();
}

//______________________________________________________________________________
void xml2PartsInformationExtractor::visitStart ( S_harmony& elt )
{
}

void xml2PartsInformationExtractor::visitEnd ( S_harmony& elt )
{
  // create the harmony
  S_msrHarmony
    harmony =
      msrHarmony::create (
        fMsrOptions,
        elt->getInputLineNumber (),
        fCurrentHarmonyRootStep, fCurrentHarmonyRootStep,
        fCurrentHarmonyKind, fCurrentHarmonyKindText,
        fCurrentHarmonyBassStep, fCurrentHarmonyBassAlter,
        fCurrentPart);

  // append it to current part
  fCurrentPart->
    appendHarmonyToPart (harmony);
}


} // namespace
