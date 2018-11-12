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

#include <iostream>
#include <sstream>

#include "bsrKeys.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "bsrOptions.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_bsrHumdrumScotKeyItem bsrHumdrumScotKeyItem::create (
  int inputLineNumber)
{
  bsrHumdrumScotKeyItem* o =
    new bsrHumdrumScotKeyItem (
      inputLineNumber);
  assert (o!=0);

  return o;
}

bsrHumdrumScotKeyItem::bsrHumdrumScotKeyItem (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Creating Humdrum/Scot key item" <<
      ", line = " << inputLineNumber <<
      endl;
  }
#endif

/*
  fKeyDiatonicPitchKind = k_NoDiatonicPitch;
  fKeyAlterationKind    = k_NoAlteration;
  fKeyOctave            = -1; // actual MusicXML octaves are non-negative
  */
}

bsrHumdrumScotKeyItem::~bsrHumdrumScotKeyItem ()
{}

/*
bool bsrHumdrumScotKeyItem::isEqualTo (
  S_bsrHumdrumScotKeyItem
    otherHumdrumScotKeyItem) const
{
  if (! otherHumdrumScotKeyItem) {
    return false;
  }
    
  return
    fKeyDiatonicPitchKind == otherHumdrumScotKeyItem->fKeyDiatonicPitchKind
      &&
    fKeyAlterationKind == otherHumdrumScotKeyItem->fKeyAlterationKind
      &&
    fKeyOctave == otherHumdrumScotKeyItem->fKeyOctave;
}

void bsrHumdrumScotKeyItem::setKeyItemDiatonicPitchKind (
  bsrDiatonicPitchKind diatonicPitchKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Setting Humdrum/Scot key item diatonic pitch to '" <<
      bsrDiatonicPitchKindAsString (diatonicPitchKind) <<
      "'" <<
      endl;
  }
#endif
  
  fKeyDiatonicPitchKind = diatonicPitchKind;
}

void bsrHumdrumScotKeyItem::setKeyItemAlterationKind (
  bsrAlterationKind alterationKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Setting Humdrum/Scot key item alteration to '" <<
      bsrAlterationKindAsString (alterationKind) <<
      "'" <<
      endl;
  }
#endif
  
  fKeyAlterationKind = alterationKind;
}

void bsrHumdrumScotKeyItem::setKeyItemOctave (int keyOctave)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Setting Humdrum/Scot key item octave to '" <<
      keyOctave <<
      "'" <<
      endl;
  }
#endif
  
  fKeyOctave = keyOctave;
}
*/

void bsrHumdrumScotKeyItem::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrHumdrumScotKeyItem::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_bsrHumdrumScotKeyItem>*> (v)) {
        S_bsrHumdrumScotKeyItem elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrHumdrumScotKeyItem::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrHumdrumScotKeyItem::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrHumdrumScotKeyItem::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_bsrHumdrumScotKeyItem>*> (v)) {
        S_bsrHumdrumScotKeyItem elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrHumdrumScotKeyItem::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrHumdrumScotKeyItem::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_bsrHumdrumScotKeyItem& elt)
{
  elt->print (os);
  return os;
}

string bsrHumdrumScotKeyItem::asString () const
{
  stringstream s;
/*
  s <<
    "HumdrumScotKeyItem" <<
    ", KeyDiatonicPitch" << ": " <<
    bsrDiatonicPitchKindAsString (fKeyDiatonicPitchKind) <<
    ", KeyAlteration" << ": " <<
    bsrAlterationKindAsString (fKeyAlterationKind) <<
    ", KeyOctave" << ": " << fKeyOctave <<
    ", line " << fInputLineNumber;
     */
  return s.str ();
}

void bsrHumdrumScotKeyItem::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

/*
//______________________________________________________________________________
S_bsrKey bsrKey::createTraditional (
  int                      inputLineNumber,
  bsrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  bsrKeyModeKind           keyModeKind,
  int                      keyCancel)
{
  bsrKey* o =
    new bsrKey (
      inputLineNumber,
      keyTonicQuarterTonesPitchKind, keyModeKind,
      keyCancel);
  assert (o!=0);

  return o;
}

S_bsrKey bsrKey::createHumdrumScot (
  int inputLineNumber)
{
  bsrKey* o =
    new bsrKey (
      inputLineNumber);
  assert (o!=0);

  return o;
}

bsrKey::bsrKey ( // for traditional keys
  int                      inputLineNumber,
  bsrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  bsrKeyModeKind           keyModeKind,
  int                      keyCancel)
    : bsrElement (inputLineNumber)
{
  // this is a traditional key
  fKeyKind = kTraditionalKind;
  
  fKeyTonicQuarterTonesPitchKind = keyTonicQuarterTonesPitchKind;
  fKeyModeKind                   = keyModeKind;
  
  fKeyCancel = keyCancel;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;
}

bsrKey::bsrKey ( // for Humdrum/Scot keys
  int                  inputLineNumber)
    : bsrElement (inputLineNumber)
{
  // this is a Humdrum/Scot key
  fKeyKind = kHumdrumScotKind;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;
}
*/

bsrKey::~bsrKey ()
{}

/*
bool bsrKey::isEqualTo (S_bsrKey otherKey) const
{
  if (! otherKey) {
    return false;
  }
  if (
    ! (
        fKeyKind == otherKey->fKeyKind
          &&
        fKeyTonicQuarterTonesPitchKind
          ==
        otherKey->fKeyTonicQuarterTonesPitchKind
          &&
        fKeyModeKind == otherKey->fKeyModeKind
          &&
        fKeyCancel == otherKey->fKeyCancel
      )
    ) {
    return false;
  }
    
  switch (fKeyKind) {
    case bsrKey::kTraditionalKind:
      break;
    case bsrKey::kHumdrumScotKind:
      {
        if (
          ! (
              fKeyItemsOctavesAreSpecified
                ==
              otherKey->fKeyItemsOctavesAreSpecified
                  &&
              fHumdrumScotKeyItemsVector.size ()
                ==
              otherKey->fHumdrumScotKeyItemsVector.size ()
            )
          ) {
          return false;
        }

        for (unsigned int i = 0; i < fHumdrumScotKeyItemsVector.size (); i++) {
          if (
            ! (
              fHumdrumScotKeyItemsVector [i]->isEqualTo (
                otherKey->fHumdrumScotKeyItemsVector [i])
              )
            ) {
            return false;
          }
        } // for
      }
      break;
  } // switch

  return true;
}

void bsrKey::appendHumdrumScotKeyItem (
  S_bsrHumdrumScotKeyItem item)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceKeys) {
    gLogIOstream <<
      "Append item '" <<
      item->asString () <<
      "' to key '" <<
      "'" <<
      endl;
    }
#endif

  // have key items octaves been specified?
  if (item->getKeyItemOctave () >= 0) {
    fKeyItemsOctavesAreSpecified = true;
  }

  // append the item to the vector
  fHumdrumScotKeyItemsVector.insert (
    fHumdrumScotKeyItemsVector.end (), item);
}
*/

void bsrKey::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrKey::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrKey>*
    p =
      dynamic_cast<visitor<S_bsrKey>*> (v)) {
        S_bsrKey elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrKey::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrKey::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrKey::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrKey>*
    p =
      dynamic_cast<visitor<S_bsrKey>*> (v)) {
        S_bsrKey elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrKey::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrKey::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_bsrKey& elt)
{
  elt->print (os);
  return os;
}

string bsrKey::keyKindAsString (
  bsrKeyKind keyKind)
{
  string result;
  
  switch (keyKind) {
    case bsrKey::kTraditionalKind:
      result = "traditional";
      break;
    case bsrKey::kHumdrumScotKind:
      result = "Humdrum/Scot";
      break;
  } // switch

  return result;
}

/*
string bsrKey::keyModeKindAsString (
  bsrKeyModeKind keyModeKind)
{
  string result;
  
  switch (keyModeKind) {
    case bsrKey::kMajorMode:
      result = "major";
      break;
    case bsrKey::kMinorMode:
      result = "minor";
      break;
    case bsrKey::kIonianMode:
      result = "ionian";
      break;
    case bsrKey::kDorianMode:
      result = "dorian";
      break;
    case bsrKey::kPhrygianMode:
      result = "phrygian";
      break;
    case bsrKey::kLydianMode:
      result = "lydian";
      break;
    case bsrKey::kMixolydianMode:
      result = "mixolydian";
      break;
    case bsrKey::kAeolianMode:
      result = "aeolian";
      break;
    case bsrKey::kLocrianMode:
      result = "locrian";
      break;
  } // switch

  return result;
}
*/

string bsrKey::asString () const
{
  stringstream s;

  s <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ", ";
/*
  switch (fKeyKind) {
    case bsrKey::kTraditionalKind:
      s <<
        bsrQuarterTonesPitchKindAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
          fKeyTonicQuarterTonesPitchKind) <<
        " " <<
        keyModeKindAsString (fKeyModeKind);
      break;
      
    case bsrKey::kHumdrumScotKind:
      s <<
        fHumdrumScotKeyItemsVector.size () << "items" <<
        ", keyItemsOctavesAreSpecified: " <<
        booleanAsString (
          fKeyItemsOctavesAreSpecified);
      break;
  } // switch
*/

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrKey::print (ostream& os)
{
  os <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ",";

/*
  switch (fKeyKind) {
    case bsrKey::kTraditionalKind:
      os <<
        " " <<
        bsrQuarterTonesPitchKindAsString (
          gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
          fKeyTonicQuarterTonesPitchKind) <<
        " " <<
        keyModeKindAsString (fKeyModeKind) <<
        ", line " << fInputLineNumber <<
        endl;
      break;
      
    case bsrKey::kHumdrumScotKind:
      os <<
        ", keyItemsOctavesAreSpecified: " <<
        booleanAsString (
          fKeyItemsOctavesAreSpecified) <<
        ", " <<
        fHumdrumScotKeyItemsVector.size () <<
        " items" <<
        ", line " << fInputLineNumber <<
        endl;

      if (fHumdrumScotKeyItemsVector.size ()) {
        os <<
          endl;
          
        gIndenter++;
        
        vector<S_bsrHumdrumScotKeyItem>::const_iterator
          iBegin = fHumdrumScotKeyItemsVector.begin (),
          iEnd   = fHumdrumScotKeyItemsVector.end (),
          i      = iBegin;
          
        for ( ; ; ) {
          os << (*i);
          if (++i == iEnd) break;
    // JMI     os << endl;
        } // for
        
        gIndenter--;
      }
      
      else
        {
          os <<
            " none" <<
            endl;
        }
      break;
  } // switch
  */
}


}
