/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>      // setw, set::precision, ...

#include "msrGeneralOptions.h"

#include "xmlPartsInfo.h"

using namespace std;

namespace MusicXML2 
{

//_______________________________________________________________________________
S_xmlPartsInfoOptions xmlPartsInfoOptions::create ()
{
  xmlPartsInfoOptions* o = new xmlPartsInfoOptions();
  assert(o!=0);
  return o;
}

xmlPartsInfoOptions::xmlPartsInfoOptions () {}

xmlPartsInfoOptions::~xmlPartsInfoOptions () {}

//______________________________________________________________________________
xmlPartsInfoElement::xmlPartsInfoElement (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber)
{
  fxmlPartsInfoOptions      = msrOpts;
  fInputLineNumber = inputLineNumber;  
}

xmlPartsInfoElement::~xmlPartsInfoElement() {}

ostream& operator<< (ostream& os, const S_xmlPartsInfoElement& elt)
{
  elt->print (os);
  return os;
}

void xmlPartsInfoElement::print (ostream& os)
{
  os << "??? Element ???" << endl;
}

//______________________________________________________________________________
S_msrPageGeometry msrPageGeometry::create (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber)
{
  msrPageGeometry* o =
    new msrPageGeometry (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageGeometry::msrPageGeometry (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber)
    : xmlPartsInfoElement (msrOpts, inputLineNumber)
{
  fMillimeters = -1;
  fTenths      = -1;
}

msrPageGeometry::~msrPageGeometry() {}

float msrPageGeometry::globalStaffSize () const
{
  if (fMillimeters > 0)
    return fMillimeters * 72.27 / 25.4;
  else
    return 20.0; // LilyPond default
}

ostream& operator<< (ostream& os, const S_msrPageGeometry& elt) {
  elt->print (os);
  return os;
}

void msrPageGeometry::print (ostream& os) {
  os << "PageGeometry" << endl;

  bool emptyGeometry = true;
  
  idtr++;
  
  if (fMillimeters > 0) {
    os <<
      idtr << setw(12) << left << "Millimeters" << " = " <<
      setprecision(4) << fMillimeters << endl;
    emptyGeometry = false;
  }

  if (fTenths > 0) {
    os <<
      idtr << setw(12) << left << "Tenths" << " = " <<
      setprecision(4) << fTenths << endl;
    emptyGeometry = false;
  }

  if (emptyGeometry)
    os << idtr <<
      " " << "nothing specified" << endl;
  
  os << endl;

  idtr--;
}

//______________________________________________________________________________
S_msrLyrics msrLyrics::create (
  S_xmlPartsInfoOptions&         msrOpts, 
  int                   inputLineNumber,
  int                   lyricsNumber,
  msrLyricsMasterStatus lyricsMasterStatus,
  S_msrVoice            lyricsVoiceUplink)
{
  msrLyrics* o =
    new msrLyrics (
      msrOpts, inputLineNumber,
      lyricsNumber,
      lyricsMasterStatus,
      lyricsVoiceUplink);
  assert(o!=0);
  return o;
}

msrLyrics::msrLyrics (
  S_xmlPartsInfoOptions&         msrOpts, 
  int                   inputLineNumber,
  int                   lyricsNumber,
  msrLyricsMasterStatus lyricsMasterStatus,
  S_msrVoice            lyricsVoiceUplink)
    : xmlPartsInfoElement (msrOpts, inputLineNumber)
{
  fLyricsNumber       = lyricsNumber;
  fLyricsMasterStatus = lyricsMasterStatus;
 
  fLyricsVoiceUplink  = lyricsVoiceUplink;
  
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating lyrics " << getLyricsName () << endl;

  fLyricsTextPresent = false;
}

string msrLyrics::getLyricsName () const
{
  // not stored in a field,
  // because the lyrics voice and staff may change name
  // when the part they belong to is re-used

  string
    lyricsNameSuffix =
 //     fLyricsMasterStatus == kMasterLyrics
      fLyricsNumber == 0
        ? "MASTER"
        : int2EnglishWord (fLyricsNumber);
        
  return
   fLyricsVoiceUplink->getVoiceName() +
    "_L_" +
    lyricsNameSuffix;
}

msrLyrics::~msrLyrics() {}

ostream& operator<< (ostream& os, const S_msrLyrics& elt)
{
  elt->print (os);
  return os;
}

void msrLyrics::print (ostream& os)
{  
  os <<
    "Lyrics" << " " << getLyricsName ();
    
  if (! fLyricsTextPresent)
    os << " (No actual text)";

  os << endl;
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::create (
  S_xmlPartsInfoOptions&         msrOpts, 
  int                   inputLineNumber,
  char                  harmonyRootStep,
  float                 harmonyRootAlter,
  msrHarmonyKind        harmonyKind,
  string                harmonyKindText,
  char                  harmonyBassStep,
  float                 harmonyBassAlter,
  S_msrPart             harmonyPartUplink)
{
  msrHarmony* o =
    new msrHarmony (
      msrOpts, inputLineNumber,
      harmonyRootStep, harmonyRootAlter,
      harmonyKind, harmonyKindText,
      harmonyBassStep, harmonyBassAlter,
      harmonyPartUplink);
  assert(o!=0);
  return o;
}

msrHarmony::msrHarmony (
  S_xmlPartsInfoOptions&         msrOpts, 
  int                   inputLineNumber,
  char                  harmonyRootStep,
  float                 harmonyRootAlter,
  msrHarmonyKind        harmonyKind,
  string                harmonyKindText,
  char                  harmonyBassStep,
  float                 harmonyBassAlter,
  S_msrPart             harmonyPartUplink)
    : xmlPartsInfoElement (msrOpts, inputLineNumber)
{
  fHarmonyRootStep   = harmonyRootStep;
  fHarmonyRootAlter  = harmonyRootAlter;
 
  fHarmonyKind       = harmonyKind;
  fHarmonyKindText   = harmonyKindText;
 
  fHarmonyBassStep   = harmonyBassStep;
  fHarmonyBassAlter  = harmonyBassAlter;
 
  fHarmonyPartUplink = harmonyPartUplink;
}

msrHarmony::~msrHarmony() {}

string msrHarmony::harmonyKindAsString () const
{
  string result;
  
  switch (fHarmonyKind) {
    case msrHarmony::kMajor:
      result = "Major";
      break;
    case msrHarmony::kMinor:
      result = "Minor";
      break;
    case msrHarmony::kDominant:
      result = "Dominant";
      break;
    case msrHarmony::kSuspendedFourth:
      result = "SuspendedFourth";
      break;
    case msrHarmony::kMajorSeventh:
      result = "MajorSeventh";
      break;
    case msrHarmony::kMinorSeventh:
      result = "MinorSeventh";
      break;
    case msrHarmony::kMajorNinth:
      result = "MajorNinth";
      break;
    case msrHarmony::kMinorNinth:
      result = "MinorNinth";
      break;
  } // switch

  return result;
}

string msrHarmony::harmonyAsString () const
{
  stringstream s;

  s <<
    fHarmonyRootStep << fHarmonyRootAlter <<
    harmonyKindAsString () << fHarmonyKindText <<
    fHarmonyBassStep << fHarmonyBassAlter;

  return s.str();
}

ostream& operator<< (ostream& os, const S_msrHarmony& elt)
{
  elt->print (os);
  return os;
}

void msrHarmony::print (ostream& os)
{  
  os <<
    "Harmony" <<
    endl;
    
  idtr++;

  os << left <<
    idtr <<
      setw(15) << "HarmonyRoot" << " = " << fHarmonyRootStep << fHarmonyRootAlter <<
      endl <<
    idtr <<
      setw(15) << "HarmonyKind" << " = " << harmonyKindAsString () <<
      endl <<
    idtr <<
      setw(15) << "HarmonyKindText" << " = " << fHarmonyKindText <<
      endl <<
    idtr <<
      setw(15) << "HarmonyBass" << " = " << fHarmonyBassStep << fHarmonyBassAlter <<
      endl;

  idtr--;
}

//______________________________________________________________________________ 
S_msrVoice msrVoice::create (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber,
  int           externalVoiceNumber,
  S_msrStaff    voiceStaffUplink)
{
  msrVoice* o =
    new msrVoice (
      msrOpts, inputLineNumber,
      externalVoiceNumber,
      voiceStaffUplink);
  assert(o!=0);
  return o;
}

msrVoice::msrVoice (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber,
  int           externalVoiceNumber,
  S_msrStaff    voiceStaffUplink)
    : xmlPartsInfoElement (msrOpts, inputLineNumber)
{
  fExternalVoiceNumber = externalVoiceNumber;
  
  fStaffRelativeVoiceNumber = externalVoiceNumber;
    // may be changed afterwards
  
  fVoiceStaffUplink = voiceStaffUplink;

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating voice \"" << getVoiceName () <<
      "\" in staff \"" << fVoiceStaffUplink->getStaffName () << "\"" <<
      endl;

  // the external voice number should not be negative
  // (0 is used for the part voice master)
  if (externalVoiceNumber < 0) {
    stringstream s;

    s <<
      "voice number " << externalVoiceNumber <<
      " is not in the 0..4 range";
      
    msrAssert (false, s.str());
  }
  
  fVoiceDivisionsPerWholeNote =
    fVoiceStaffUplink->
      getStaffDivisionsPerWholeNote ();
    
  // there may be an anacrusis, but let's start with 1 anyway
  fVoiceMeasureNumber = 1;

  fMeasureZeroHasBeenMetInVoice   = false;
  fMeasureNumberHasBeenSetInVoice = false;
  fMusicHasBeenInsertedInVoice    = false;
  
  fVoiceActualNotesCounter = 0;
}

msrVoice::~msrVoice() {}

string msrVoice::getVoiceName () const
{
  int voiceNumber =
    true // fMsrOptions-> fCreateStaffRelativeVoiceNumbers // JMI use
      ? fStaffRelativeVoiceNumber
      : fExternalVoiceNumber;

  string suffix =
    fStaffRelativeVoiceNumber == 0
      ? "MASTER"
      : int2EnglishWord (voiceNumber);
      
  return
    fVoiceStaffUplink->getStaffName() +
    "_V_" +
    suffix;
}

void msrVoice::setVoiceDivisionsPerWholeNote (
  int divisionsPerWholeNote)
{
  fVoiceDivisionsPerWholeNote =
    divisionsPerWholeNote;
}

void msrVoice::setVoiceMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
  fVoiceMeasureNumber = measureNumber;

  fMeasureNumberHasBeenSetInVoice = true;

  if (measureNumber == 0) {  
    fMeasureZeroHasBeenMetInVoice = true;
  }
}

void msrVoice::forceVoiceMeasureNumberTo (int measureNumber) // JMI
{
  fVoiceMeasureNumber = measureNumber;
};

S_msrLyrics msrVoice::addLyricsToVoiceByItsNumber (
  int inputLineNumber,
  int lyricsNumber)
{
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    stringstream s;
    
    s <<
      "lyrics " << lyricsNumber <<
      " already exists in this voice";

    msrInternalError (
      inputLineNumber,
      s.str());

// JMI    return fVoiceLyricsMap [lyricsNumber];
  }

  // create the lyrics
  S_msrLyrics
    lyrics =
      msrLyrics::create (
        fMsrOptions,
        inputLineNumber,
        lyricsNumber,
        msrLyrics::kRegularLyrics,
        this);

  // add it to this voice
  addLyricsToVoice (lyrics);

  // return it
  return lyrics;
}

void msrVoice::addLyricsToVoice (S_msrLyrics lyrics)
{
  // get lyrics number
  int lyricsNumber =
    lyrics->getLyricsNumber ();
    
  // register lyrics in this voice
  if (gGeneralOptions->fForceDebug || gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding lyrics " << lyrics->getLyricsName () <<
      " (" << lyricsNumber <<
      ") to voice \"" << getVoiceName () << "\"" <<
      endl;

  fVoiceLyricsMap [lyricsNumber] = lyrics;

  // catch up with fVoiceLyricsmaster
  // in case the lyrics does not start upon the first voice note
  vector<S_msrLyricschunk>
    masterChunks =
      fVoiceLyricsmaster->getLyricschunks ();

  if (masterChunks.size()) {
// JMI    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Copying current contents of voice master lyrics to new lyrics" << endl;
    for (
      vector<S_msrLyricschunk>::const_iterator i =
        masterChunks.begin();
      i != masterChunks.end();
      i++) {
      // add chunk to lyrics
      lyrics->addChunkToLyrics ((*i));
    } // for
  }
}

S_msrLyrics msrVoice::createLyricsInVoiceIfNeeded (
  int inputLineNumber,
  int lyricsNumber)
{
  if (false && gGeneralOptions->fTrace) // JMI
    cerr << idtr <<
      "### --> createLyricsInVoiceIfNeeded (" << inputLineNumber <<
      ", " << lyricsNumber << ")" <<
      ", fVoiceLyricsMap.size() = " << fVoiceLyricsMap.size () <<
      endl;

  S_msrLyrics lyrics;

  // is lyricsNumber already known in voice?
  if (fVoiceLyricsMap.count (lyricsNumber)) {
    // yes, use it
    lyrics = fVoiceLyricsMap [lyricsNumber];
  }
  
  else {
    // no, add it to the voice
    lyrics =
      addLyricsToVoiceByItsNumber (
        inputLineNumber, lyricsNumber);
  }

  return lyrics;
}

S_xmlPartsInfoElement msrVoice::removeLastElementFromVoice (
  int inputLineNumber)
{
  if (gGeneralOptions->fDebugDebug)
    cerr << idtr <<
      "Removing last element " <<
      " from voice " << getVoiceName () << endl;

  return
    fVoiceVoicechunk->
      removeLastElementFromVoicechunk (inputLineNumber);
}

void msrVoice::finalizeLastMeasureOfVoice (int inputLineNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> finalizing last measure in voice " <<
      getVoiceName () <<
      ", line " << inputLineNumber <<
      endl;

  fVoiceVoicechunk->
    finalizeLastMeasureOfVoicechunk (inputLineNumber);
}

ostream& operator<< (ostream& os, const S_msrVoice& elt)
{
  elt->print (os);
  return os;
}

void msrVoice::print (ostream& os)
{
  os <<
    "Voice" " \"" << getVoiceName () <<
    "\" (" << fVoiceActualNotesCounter <<
    " actual notes" <<
     ", " << fVoiceLyricsMap.size() << " lyrics" <<
    ")" <<
    endl;

  idtr++;

  os <<
    idtr <<
      setw(32) << "(fMeasureZeroHasBeenMetInVoice" << " = " <<
      string (
        fMeasureZeroHasBeenMetInVoice
          ? "true"
          : "false") <<
      ")" <<
      endl <<
    idtr <<
      setw(32) << "(fMeasureNumberHasBeenSetInVoice" << " = " <<
      string (
        fMeasureNumberHasBeenSetInVoice
          ? "true"
          : "false") <<
      ")" <<
      endl <<
    idtr <<
      setw(32) << "(fMusicHasBeenInsertedInVoice" << " = " <<
      string (
        fMusicHasBeenInsertedInVoice
          ? "true"
          : "false") <<
      ")" <<
      endl;

  idtr--;
}

//______________________________________________________________________________
int msrStaff::gMaxStaffVoices = 4;

S_msrStaff msrStaff::create (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber,
  int           staffNumber,
  S_msrPart     staffPartUplink)
{
  msrStaff* o =
    new msrStaff (
      msrOpts, inputLineNumber, staffNumber, staffPartUplink);
  assert(o!=0);
  return o;
}

msrStaff::msrStaff (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber,
  int           staffNumber,
  S_msrPart     staffPartUplink)
    : xmlPartsInfoElement (msrOpts, inputLineNumber)
{
  fStaffKind = kRegularStaff;
  
  fStaffNumber = staffNumber;
  fStaffPartUplink   = staffPartUplink;

  fRegisteredVoicesCounter = 0;

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating staff \"" << getStaffName () <<
      "\" in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // the staff number should not be negative
  // (0 is used for hidden staff containing the part voice master)
  if (staffNumber < 0) {
    stringstream s;

    s <<
      "staff number " << staffNumber <<
      " is not positive";
      
    msrAssert (false, s.str());
  }

  // populate the staff
  fStaffDivisionsPerWholeNote =
    fStaffPartUplink->
      getPartDivisionsPerWholeNote ();

  // create the staff voice master with relative number 0
  // and all 'gMaxStaffVoices' voices for this staff
  // those that remain without music will be removed later
  for (int i = 0; i <= gMaxStaffVoices; i++) {
    addVoiceToStaffByItsRelativeNumber (
      fInputLineNumber, i);
  } // for

  // set staff voice master
  fStaffVoiceMaster =
    fStaffVoicesMap [0];

  // mark it as containing music, to prevent it being removed
  fStaffVoiceMaster->
    setMusicHasBeenInsertedInVoice ();

  // get the initial clef from the staff if any
  {
    S_msrClef
      clef =
        fStaffPartUplink->getPartClef ();
  
    if (clef) {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting staff clef '" << clef->clefAsString () <<
          "' in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;

      fStaffClef = clef;

      // is this a tablature or percussion staff?
      if (clef->clefIsATablatureClef ())
        fStaffKind = kTablatureStaff;
      else if (clef->clefIsAPercussionClef ())
        fStaffKind = kPercussionStaff;

      appendClefToAllStaffVoices (clef);
    }
    
    else {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting default treble clef " <<
          " in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;

      // create the implicit initial G line 2 clef
      fStaffClef =
        msrClef::create (
          msrOpts,
          inputLineNumber,
          msrClef::kTrebleClef);
    }
  }
    
  // get the initial key from the staff if any
  {
    S_msrKey
      key =
        fStaffPartUplink->getPartKey ();
  
    if (key) {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting staff key '" << key->keyAsString () <<
          "' in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;

      fStaffKey = key;
      
      appendKeyToAllStaffVoices (key);
    }
    else {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting default C major key " <<
          " in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;
          
      // create the implicit initial C major key
      fStaffKey =
        msrKey::create (
          msrOpts,
          inputLineNumber,
          0, "major", 0);
    }
  }
  
  // get the initial time from the staff if any
  {
    S_msrTime
      time =
        fStaffPartUplink->getPartTime ();

    if (time) {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting staff time '" << time->timeAsString () <<
          "' in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;

      fStaffTime = time;

      appendTimeToAllStaffVoices (time);
    }
    else {
      if (gGeneralOptions->fTrace)
        cerr << idtr <<
          "Setting default 4/4 time " <<
          " in staff " << fStaffNumber <<
          " in part " << fStaffPartUplink->getPartCombinedName () <<
          endl;
          
      // create the implicit initial 4/4 time signature
      fStaffTime =
        msrTime::create (
          msrOpts,
          inputLineNumber,
          4, 4);
    }
  }
  
  // get the initial transpose from the staff if any
  {
    S_msrTranspose
      transpose =
        fStaffPartUplink->getPartTranspose ();
        
    if (transpose) {
      fStaffTranspose = transpose;
      appendTransposeToAllStaffVoices (transpose);
    }
  }
}

msrStaff::~msrStaff()
{}

string msrStaff::getStaffName () const
  {
  return
    fStaffNumber == 0
      ?
        fStaffPartUplink->getPartMSRName () +
        "_S_" +
        "(MASTER)"
      :
        fStaffPartUplink->getPartMSRName () +
        "_S_" +
        int2EnglishWord (fStaffNumber);
  }

void msrStaff::setStaffDivisionsPerWholeNote (
  int divisionsPerWholeNote)
{
  fStaffDivisionsPerWholeNote =
    divisionsPerWholeNote;

  setAllStaffVoicesDivisionsPerWholeNote (
    divisionsPerWholeNote);
}

void msrStaff::setAllStaffVoicesDivisionsPerWholeNote (int divisions)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->setVoiceDivisionsPerWholeNote (divisions);
  } // for
}

void msrStaff::setStaffMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> setStaffMeasureNumber(), " <<
      ", line " << inputLineNumber <<
      ", measureNumber = " << measureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      endl;

  // set part measure location
  fStaffMeasureNumber = measureNumber;

  // propagate it to all staves
  setAllStaffVoicesMeasureNumber (
    inputLineNumber, measureNumber);  
}

/*
void msrStaff::setStaffMeasureLocation (
  int                       inputLineNumber,
  const msrMeasureLocation& measureLocation)
{
  // set staff measure location
  fStaffMeasureLocation = measureLocation;

  // propagate it to all staves
  setAllStaffVoicesMeasureLocation (
    inputLineNumber, measureLocation);  
}

void msrStaff::setAllStaffVoicesMeasureLocation (
  int                       inputLineNumber,
  const msrMeasureLocation& measureLocation)
{
  for (
    map<int, S_msrVoice>::iterator i =
      fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->
      setVoiceMeasureLocation (
        inputLineNumber, measureLocation);
  } // for
}
*/

S_msrVoice msrStaff::addVoiceToStaffByItsRelativeNumber (
  int inputLineNumber,
  int voiceRelativeNumber)
{
  // create the voice
  S_msrVoice
    voice =
      msrVoice::create (
        fMsrOptions,
        inputLineNumber,
        voiceRelativeNumber,
        this);

  // add it to this staff
  fStaffVoicesMap [voiceRelativeNumber] = voice;
  
  // return the voice
  return voice;
}

S_msrVoice msrStaff::registerVoiceInStaffByItsExternalNumber (
  int inputLineNumber,
  int externalVoiceNumber)
{
  // take this new voice into account
  fRegisteredVoicesCounter++;

  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Registering voice " << externalVoiceNumber <<
       " as relative voice " << fRegisteredVoicesCounter <<
     " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
 // JMI     " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // are there too many voices in this staff? 
  if (fRegisteredVoicesCounter > msrStaff::gMaxStaffVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " << msrStaff::gMaxStaffVoices <<
      " voices, voice " << externalVoiceNumber << " overflows it" <<
      endl;
      
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      inputLineNumber,
      s.str());
  }

  // fetch the voice
  S_msrVoice
    voice =
      fStaffVoicesMap [fRegisteredVoicesCounter];
      
  // update it's voice number
  voice->
    setExternalVoiceNumber (externalVoiceNumber);

  // register it by its number
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Voice " << externalVoiceNumber <<
      " in staff " << getStaffName () <<
      " gets staff relative number " << fRegisteredVoicesCounter <<
      endl;
      
  fStaffVoicesCorrespondanceMap [externalVoiceNumber] =
    voice;

  return voice;
}

S_msrVoice msrStaff::fetchVoiceFromStaff (
  int inputLineNumber, int externalVoiceNumber)
{
  S_msrVoice result;

  if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "--> fetching external voice number " << externalVoiceNumber <<
     " in staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesCorrespondanceMap.begin();
    i != fStaffVoicesCorrespondanceMap.end();
    i++) {
    if (
      (*i).second->getExternalVoiceNumber ()
        ==
      externalVoiceNumber  ) {
      if (gGeneralOptions->fDebug) {
        cerr << idtr <<
          "--> voice " << externalVoiceNumber <<
          " in staff " << getStaffName () <<
          " has staff relative number " << (*i).first <<
          endl;
      }
        
      result = (*i).second;
      break;
    }
  } // for

  return result;
}

void msrStaff::registerVoiceInStaff (
  int inputLineNumber, S_msrVoice voice)
{
  // take this new voice into account
  fRegisteredVoicesCounter++;

  // are there too many voices in this staff? 
  if (fRegisteredVoicesCounter > msrStaff::gMaxStaffVoices) {
    stringstream s;
    
    s <<
      "staff \"" << getStaffName () <<
      "\" is already filled up with " << msrStaff::gMaxStaffVoices <<
      " voices, voice " << voice->getVoiceName () << " overflows it" <<
      endl;
      
    msrMusicXMLError (
// JMI    msrMusicXMLWarning ( JMI
      inputLineNumber,
      s.str());
  }

  // register voice in this staff
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Registering voice \"" << voice->getVoiceName () <<
      "\" as relative voice " << fRegisteredVoicesCounter <<
      " of staff \"" << getStaffName () <<
      "\", line " << inputLineNumber <<
// JMI       " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // register is by its relative number
  fStaffVoicesMap [fRegisteredVoicesCounter] = voice;

  // register it by its number
  fStaffVoicesCorrespondanceMap [voice->getExternalVoiceNumber ()] =
    voice;
}

void msrStaff::setStaffClef (S_msrClef clef)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting staff clef '" << clef->clefAsString () <<
      "' in staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // set staff clef
  fStaffClef = clef;

  // is this a tablature or percussion staff?
  if (clef->clefIsATablatureClef ())
    fStaffKind = kTablatureStaff;
  else if (clef->clefIsAPercussionClef ())
    fStaffKind = kPercussionStaff;
  
  // propagate clef to all voices
  appendClefToAllStaffVoices (clef);
}

void msrStaff::setStaffKey  (S_msrKey  key)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting key '" << key->keyAsString () <<
      "' in staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // set staff key
  fStaffKey = key;

  // propagate it to all voices
  appendKeyToAllStaffVoices (key);
}

void msrStaff::setStaffTime (S_msrTime time)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting time '" << time->timeAsString () <<
      "' in staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // set staff time
  fStaffTime = time;

  // propagate it to all voices
  appendTimeToAllStaffVoices (time);
}

void msrStaff::setStaffTranspose (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting transpose '" << transpose->transposeAsString () <<
      "' in staff " << fStaffNumber <<
      " in part " << fStaffPartUplink->getPartCombinedName () <<
      endl;

  // set staff transpose
  fStaffTranspose = transpose;

  // propagate it to all voices
  appendTransposeToAllStaffVoices (transpose);
}

void msrStaff::appendClefToAllStaffVoices (S_msrClef clef)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendClefToVoice (clef);
  } // for
}

void msrStaff::appendKeyToAllStaffVoices (S_msrKey  key)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendKeyToVoice (key);
  } // for
}

void msrStaff::appendTimeToAllStaffVoices (S_msrTime time)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendTimeToVoice (time);
  } // for
}

void msrStaff::appendTransposeToAllStaffVoices (S_msrTranspose transpose)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->appendTransposeToVoice (transpose);
  } // for
}

void msrStaff::setAllStaffVoicesMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> setAllStaffVoicesMeasureNumber(), " <<
      ", line " << inputLineNumber <<
      ", measureNumber = " << measureNumber <<
      ", in staff \"" << getStaffName () << "\"" <<
      endl;

  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->
      setVoiceMeasureNumber (
        inputLineNumber, measureNumber);
  } // for
}
/*
void msrStaff::bringAllStaffVoicesToPosition (
  int inputLineNumber,
  int measurePosition)
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->bringVoiceToPosition (
      inputLineNumber,
      measurePosition);
  } // for
}
*/

void msrStaff::removeStaffEmptyVoices ()
{
  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
 // JMI   if (! (*i).second->getVoiceActualNotesCounter ()) {
    if (! (*i).second->getMusicHasBeenInsertedInVoice ()) {
      fStaffVoicesMap.erase (i);
    }
  } // for
}

void msrStaff::finalizeLastMeasureOfStaff (int inputLineNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> finalizing last measure in staff " <<
      getStaffName () <<
      ", line " << inputLineNumber <<
      endl;

  for (
    map<int, S_msrVoice>::iterator i = fStaffVoicesMap.begin();
    i != fStaffVoicesMap.end();
    i++) {
    (*i).second->finalizeLastMeasureOfVoice (inputLineNumber);
  } // for
}

ostream& operator<< (ostream& os, const S_msrStaff& elt)
{
  elt->print (os);
  return os;
}

string msrStaff::staffKindAsString () const
{
  string result;
  
  switch (fStaffKind) {
    case msrStaff::kRegularStaff:
      result = "regular";
      break;
    case msrStaff::kTablatureStaff:
      result = "tablature";
      break;
    case msrStaff::kPercussionStaff:
      result = "percussion";
      break;
  } // switch

  return result;
}

void msrStaff::print (ostream& os)
{
  os <<
    "Staff" " " << getStaffName () <<
    ", " << staffKindAsString () <<
    " (" << fStaffVoicesMap.size() << " voices)" <<
    endl;

  idtr++;

  if (fStaffClef)
    os << idtr << fStaffClef;
  else
    os << idtr << "NO_CLEF" << endl;

  if (fStaffKey)
    os << idtr << fStaffKey;
  else
    os << idtr << "NO_KEY" << endl;

  if (fStaffTime)
    os << idtr << fStaffTime;
  else
    os << idtr << "NO_TIME" << endl;

  os <<
    idtr << "StaffInstrumentName: \"" <<
    fStaffInstrumentName << "\"" << endl;

  if (fStafftuningsList.size ()) {
    os <<
      idtr << "Staff tunings:" <<
      endl;
      
    list<S_msrStafftuning>::const_iterator
      iBegin = fStafftuningsList.begin(),
      iEnd   = fStafftuningsList.end(),
      i      = iBegin;
      
    idtr++;
    for ( ; ; ) {
      cerr << idtr << (*i)->stafftuningAsString ();
      if (++i == iEnd) break;
      cerr << endl;
    } // for
    cerr << endl;
    idtr--;
  }

  os << endl;

  // print the registered voices
  if (fStaffVoicesMap.size ()) {
    map<int, S_msrVoice>::const_iterator
      iBegin = fStaffVoicesMap.begin(),
      iEnd   = fStaffVoicesMap.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      cerr << idtr << (*i).second;
      if (++i == iEnd) break;
      cerr << endl;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________ 
S_msrPart msrPart::create (
  S_xmlPartsInfoOptions&  msrOpts, 
  int            inputLineNumber,
  string         partID,
  S_msrPartgroup partPartgroupUplink)
{
  msrPart* o =
    new msrPart (
      msrOpts, inputLineNumber, partID, partPartgroupUplink);
  assert(o!=0);
  return o;
}

msrPart::msrPart (
  S_xmlPartsInfoOptions&  msrOpts, 
  int            inputLineNumber,
  string         partID,
  S_msrPartgroup partPartgroupUplink)
    : xmlPartsInfoElement (msrOpts, inputLineNumber)
{
  // replace spaces in part ID
  for_each (
    partID.begin(),
    partID.end(),
    stringSpaceReplacer (fPartID, '_'));
 
  fPartPartgroupUplink = partPartgroupUplink;

  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      msrOpts->fPartsRenaming.find (fPartID);
        
  if (it != msrOpts->fPartsRenaming.end ()) {
    // yes, rename the part accordinglingly
    fPartMSRName = (*it).second;
  }
  else {
    // coin the name from the argument
    fPartMSRName =
      "P_"+stringNumbersToEnglishWords (fPartID);
  }
    
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating part " << getPartCombinedName () << endl;

  fPartDivisionsPerWholeNote = 0;

  fMeasureZeroHasBeenMetInPart = false;
  
  fPartMeasurePositionHighTide = 1;

/* JMI
  // create a first staff for the part
  this->
    addStaffToPart (
      inputLineNumber, 1);

  // create the part voice master
  S_msrStaff
    hiddenMasterStaff =
      msrStaff::create (
        fMsrOptions, 
        0,            // inputLineNumber
        0,            // staffNumber
        this);        // fStaffPartUplink

  fPartVoicemaster =
    msrVoice::create (
      fMsrOptions, 
      0,            // inputLineNumber
      0,            // staffRelativeVoiceNumber
      hiddenMasterStaff); // voiceStaffUplink
      *
*/
}

msrPart::~msrPart() {}

void msrPart::updatePartMeasurePositionHighTide (
  int inputLineNumber,
  int measurePosition)
{
  if (measurePosition > fPartMeasurePositionHighTide)
    fPartMeasurePositionHighTide = measurePosition;
}

void msrPart::setPartMSRName (string partMSRName)
{
  // is this part name in the part renaming map?
  map<string, string>::iterator
    it =
      fMsrOptions->fPartsRenaming.find (fPartMSRName);
        
  if (it != fMsrOptions->fPartsRenaming.end ()) {
    // yes, rename the part accordinglingly
    fPartMSRName = (*it).second;

    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Setting part name of " << getPartCombinedName () <<
        " to \"" << fPartMSRName << "\"" <<
         endl;
  }
  else {
    // use the argument
    fPartMSRName = partMSRName;

    if (gGeneralOptions->fTrace)
      cerr << idtr <<
        "Keeping partID \"" << partMSRName <<
        "\" as part name  for " << getPartCombinedName () <<
      endl;
  }
}

string msrPart::getPartCombinedName () const
{
  return
    "\"" + fPartMSRName + "\"" + " (" + fPartID + ")";
}

void msrPart::setPartDivisionsPerWholeNote (
  int divisionsPerWholeNote)
{
  fPartDivisionsPerWholeNote =
    divisionsPerWholeNote;

  setAllPartStavesDivisionsPerWholeNote (
    divisionsPerWholeNote);
}

void msrPart::setPartMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> setPartMeasureNumber()" <<
      ", line " << inputLineNumber <<
      ", measureNumber = " << measureNumber <<
      ", in part " << getPartCombinedName () <<
      endl;

  // set part measure location
  fPartMeasureNumber = measureNumber;

  if (measureNumber == 0) {  
    fMeasureZeroHasBeenMetInPart = true;
  }

  // propagate it to all staves
  setAllPartStavesMeasureNumber (
    inputLineNumber, measureNumber);  
}

void msrPart::setPartClef (S_msrClef clef)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting part clef \"" << clef->clefAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part clef
  fPartClef = clef;

  // propagate it to all staves
  setAllPartStavesClef (clef);
}

void msrPart::setPartKey  (S_msrKey  key)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting part key \"" << key->keyAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part key
  fPartKey = key;

  // propagate it to all staves
  setAllPartStavesKey (key);
}

void msrPart::setPartTime (S_msrTime time)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting part time \"" << time->timeAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part time
  fPartTime = time;

  // propagate it to all staves
  setAllPartStavesTime (time);
}

void msrPart::setPartTranspose (S_msrTranspose transpose)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Setting part transpose \"" << transpose->transposeAsString () <<
      "\" in part " << getPartCombinedName () <<
    endl;

  // set part transpose
  fPartTranspose = transpose;

  // propagate it to all staves
  setAllPartStavesTranspose (transpose);
}

void msrPart::setAllPartStavesDivisionsPerWholeNote (int divisions)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffDivisionsPerWholeNote (divisions);
  } // for
}

/*
void msrPart::setAllPartStavesMeasureLocation (
  int                       inputLineNumber,
  const msrMeasureLocation& measureLocation)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffMeasureLocation (
      inputLineNumber, measureLocation);
  } // for
}
*/

void msrPart::setAllPartStavesMeasureNumber (
  int inputLineNumber,
  int measureNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> setAllPartStavesMeasureNumber()" <<
      ", line " << inputLineNumber <<
      ", measureNumber = " << measureNumber <<
      ", in part " << getPartCombinedName () <<
      endl;

  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->
      setStaffMeasureNumber (
        inputLineNumber, measureNumber);
  } // for
}

void msrPart::setAllPartStavesClef (S_msrClef clef)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffClef (clef);
  } // for
}

void msrPart::setAllPartStavesKey (S_msrKey  key)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffKey (key);
  } // for
}
          
void msrPart::setAllPartStavesTime  (S_msrTime time)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffTime (time);
  } // for
}
          
void msrPart::setAllPartStavesTranspose (S_msrTranspose transpose)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->setStaffTranspose (transpose);
  } // for
}

S_msrStaff msrPart::addStaffToPartByItsNumber (
  int inputLineNumber,
  int staffNumber)
{
  if (fPartStavesMap.count (staffNumber)) {
    cerr << idtr <<
      "### Internal error: staffNumber " << staffNumber <<
      " already exists in part " << " " << getPartCombinedName () <<
      endl;

    return fPartStavesMap [staffNumber];
  }

// JMI  if (gGeneralOptions->fForceDebug || gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding staff " << staffNumber <<
      " to part " << getPartCombinedName () <<
      endl;
  
  // create the staff
  S_msrStaff
    staff =
      msrStaff::create (
        fMsrOptions,
        inputLineNumber,
        staffNumber,
        this);

  // register it in this part
  fPartStavesMap [staffNumber] = staff;

  // return it
  return staff;
}

void msrPart::addStaffToPart (S_msrStaff staff)
{
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Adding staff " << staff->getStaffName () <<
      " to part " << getPartCombinedName () <<
      endl;

  // register staff in this part
  fPartStavesMap [staff->getStaffNumber ()] = staff;
}

S_msrStaff msrPart::fetchStaffFromPart (
  int staffNumber)
{
  S_msrStaff result;
  
  if (fPartStavesMap.count (staffNumber)) {
    result = fPartStavesMap [staffNumber];
  }

  return result;
}

void msrPart::appendHarmonyToPart (S_msrHarmony harmony)
{
  // JMI
}

void msrPart:: handleBackup (int divisions)
{
 // JMI 
}

/*
void msrPart::bringAllPartVoicesToPosition (
  int inputLineNumber,
  int measurePosition)
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->bringAllStaffVoicesToPosition (
      inputLineNumber,
      measurePosition);
  } // for
}
*/
void msrPart::removePartEmptyVoices ()
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->removeStaffEmptyVoices ();
  } // for
}

void msrPart::finalizeLastMeasureOfPart (int inputLineNumber)
{
 // if (gGeneralOptions->fDebug)
    cerr << idtr <<
      "### --> finalizing last measure in part " <<
      getPartName () <<
      ", line " << inputLineNumber <<
      endl;

  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->finalizeLastMeasureOfStaff (inputLineNumber);
  } // for
}

ostream& operator<< (ostream& os, const S_msrPart& elt)
{
  elt->print (os);
  return os;
}

void msrPart::print (ostream& os)
{
  os <<
    "Part" << " " << getPartCombinedName () <<
    " (" << fPartStavesMap.size() << " staves" <<
    ", position high tide " << fPartMeasurePositionHighTide <<
    ")" <<
    endl;
    
  idtr++;
  
  os << left <<
    idtr <<
      setw(25) << "PartDivisionsPerWholeNote" << ": " <<
      fPartDivisionsPerWholeNote << endl <<
    idtr <<
      setw(25) << "PartMSRName" << ": \"" <<
      fPartMSRName << "\"" << endl <<
    idtr <<
      setw(25) << "PartName" << ": \"" <<
      fPartName << "\"" << endl <<
    idtr <<
      setw(25) << "PartAbbrevation" << ": \"" <<
      fPartAbbreviation << "\"" << endl <<
    idtr <<
      setw(25) << "PartInstrumentName" << ": \"" <<
      fPartInstrumentName << "\"" << endl;

  if (fPartStavesMap.size()) {
    os << endl;
    for (
      map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
      i != fPartStavesMap.end();
      i++) {
      os <<
        idtr << (*i).second <<
        endl;
    } // for
  }

  idtr--;
}

//______________________________________________________________________________
S_xmlPartsInfo xmlPartsInfo::create (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber)
{
  xmlPartsInfo* o =
    new xmlPartsInfo (
      msrOpts, inputLineNumber);
  assert(o!=0);
  return o;
}

xmlPartsInfo::xmlPartsInfo (
  S_xmlPartsInfoOptions& msrOpts, 
  int           inputLineNumber)
    : xmlPartsInfoElement (msrOpts, inputLineNumber)
{
  fIdentification =
    msrIdentification::create (
      msrOpts, inputLineNumber);
      
  fPageGeometry =
    msrPageGeometry::create (
      msrOpts, inputLineNumber);
}

xmlPartsInfo::~xmlPartsInfo() {}

void xmlPartsInfo::addPartgroupToScore (S_msrPartgroup partgroup)
{
  // register it in this score
  fPartgroupsList.push_back (partgroup);
}

ostream& operator<< (ostream& os, const S_xmlPartsInfo& elt)
{
  elt->print (os);
  return os;
}

void xmlPartsInfo::print (ostream& os)
{
  os <<
    "MSR Score" <<
    " (" << fPartgroupsList.size() << " part groups)" <<
    endl << endl;

  idtr++;
  
  if (fIdentification) {
    os << idtr << fIdentification;
  }
  
  if (fPageGeometry) {
    os << idtr << fPageGeometry;
  }
  
  for (
    list<S_msrPartgroup>::iterator i = fPartgroupsList.begin();
    i != fPartgroupsList.end();
    i++) {
    os << idtr << (*i);
  } // for
  
  idtr--;
}


}
