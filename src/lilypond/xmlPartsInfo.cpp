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
  fXmlPartsInfoOptions      = msrOpts;
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
    true // fXmlPartsInfoOptions-> fCreateStaffRelativeVoiceNumbers // JMI use
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
        fXmlPartsInfoOptions,
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
        fXmlPartsInfoOptions,
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
  string         partID)
{
  msrPart* o =
    new msrPart (
      msrOpts, inputLineNumber, partID);
  assert(o!=0);
  return o;
}

msrPart::msrPart (
  S_xmlPartsInfoOptions&  msrOpts, 
  int            inputLineNumber,
  string         partID)
    : xmlPartsInfoElement (msrOpts, inputLineNumber)
{
  // replace spaces in part ID
  for_each (
    partID.begin(),
    partID.end(),
    stringSpaceReplacer (fPartID, '_'));
     
  if (gGeneralOptions->fTrace)
    cerr << idtr <<
      "Creating part " << getPartCombinedName () << endl;

  fPartDivisionsPerWholeNote = 0;

  fMeasureZeroHasBeenMetInPart = false;
}

msrPart::~msrPart() {}

void msrPart::setPartMSRName (string partMSRName)
{
  // JMI
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
        fXmlPartsInfoOptions,
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

void msrPart::removePartEmptyVoices ()
{
  for (
    map<int, S_msrStaff>::iterator i = fPartStavesMap.begin();
    i != fPartStavesMap.end();
    i++) {
    (*i).second->removeStaffEmptyVoices ();
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
  fPageGeometry =
    msrPageGeometry::create (
      msrOpts, inputLineNumber);
}

xmlPartsInfo::~xmlPartsInfo() {}

ostream& operator<< (ostream& os, const S_xmlPartsInfo& elt)
{
  elt->print (os);
  return os;
}

void xmlPartsInfo::print (ostream& os)
{
  os <<
    "XML parts info" <<
    endl << endl;

  idtr++;
  
  if (fPageGeometry) {
    os << idtr << fPageGeometry;
  }
  
  idtr--;
}


}
