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
#include <sstream>

#include "utilities.h"
#include "messagesHandling.h"

#include "msrKeys.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrHumdrumScotKeyItem msrHumdrumScotKeyItem::create (
  int inputLineNumber)
{
  msrHumdrumScotKeyItem* o =
    new msrHumdrumScotKeyItem (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrHumdrumScotKeyItem::msrHumdrumScotKeyItem (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Creating Humdrum/Scot key item" <<
      ", line = " << inputLineNumber <<
      endl;
  }
#endif

  fKeyDiatonicPitchKind = k_NoDiatonicPitch;
  fKeyAlterationKind    = k_NoAlteration;
  fKeyOctave            = -1; // actual MusicXML octaves are non-negative
}

msrHumdrumScotKeyItem::~msrHumdrumScotKeyItem ()
{}

bool msrHumdrumScotKeyItem::isEqualTo (
  S_msrHumdrumScotKeyItem
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

void msrHumdrumScotKeyItem::setKeyItemDiatonicPitchKind (
  msrDiatonicPitchKind diatonicPitchKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Setting Humdrum/Scot key item diatonic pitch to '" <<
      msrDiatonicPitchKindAsString (diatonicPitchKind) <<
      "'" <<
      endl;
  }
#endif

  fKeyDiatonicPitchKind = diatonicPitchKind;
}

void msrHumdrumScotKeyItem::setKeyItemAlterationKind (
  msrAlterationKind alterationKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Setting Humdrum/Scot key item alteration to '" <<
      msrAlterationKindAsString (alterationKind) <<
      "'" <<
      endl;
  }
#endif

  fKeyAlterationKind = alterationKind;
}

void msrHumdrumScotKeyItem::setKeyItemOctave (int keyOctave)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Setting Humdrum/Scot key item octave to '" <<
      keyOctave <<
      "'" <<
      endl;
  }
#endif

  fKeyOctave = keyOctave;
}

void msrHumdrumScotKeyItem::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHumdrumScotKeyItem::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHumdrumScotKeyItem::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHumdrumScotKeyItem::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrHumdrumScotKeyItem::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHumdrumScotKeyItem>*
    p =
      dynamic_cast<visitor<S_msrHumdrumScotKeyItem>*> (v)) {
        S_msrHumdrumScotKeyItem elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrHumdrumScotKeyItem::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHumdrumScotKeyItem::browseData (basevisitor* v)
{}

string msrHumdrumScotKeyItem::asString () const
{
  stringstream s;

  s <<
    "HumdrumScotKeyItem" <<
    ", KeyDiatonicPitch" << ": " <<
    msrDiatonicPitchKindAsString (fKeyDiatonicPitchKind) <<
    ", KeyAlteration" << ": " <<
    msrAlterationKindAsString (fKeyAlterationKind) <<
    ", KeyOctave" << ": " << fKeyOctave <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrHumdrumScotKeyItem::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrHumdrumScotKeyItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrKey msrKey::createTraditional (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  msrKeyModeKind           keyModeKind,
  int                      keyCancel)
{
  msrKey* o =
    new msrKey (
      inputLineNumber,
      keyTonicQuarterTonesPitchKind, keyModeKind,
      keyCancel);
  assert (o!=0);

  return o;
}

S_msrKey msrKey::createHumdrumScot (
  int inputLineNumber)
{
  msrKey* o =
    new msrKey (
      inputLineNumber);
  assert (o!=0);

  return o;
}

msrKey::msrKey ( // for traditional keys
  int                      inputLineNumber,
  msrQuarterTonesPitchKind keyTonicQuarterTonesPitchKind,
  msrKeyModeKind           keyModeKind,
  int                      keyCancel)
    : msrMeasureElement (inputLineNumber)
{
  // this is a traditional key
  fKeyKind = kTraditionalKind;

  fKeyModeKind = keyModeKind;

  /* caution:
    <key>
      <fifths>0</fifths>
      <mode>minor</mode>
    </key>
  is A minor actually, not C minor!
  */

  fKeyTonicQuarterTonesPitchKind = keyTonicQuarterTonesPitchKind;

  switch (fKeyModeKind) {
    case k_NoKeyMode:
      break;
    case kMajorMode:
      break;
    case kMinorMode:
      fKeyTonicQuarterTonesPitchKind =
        noteAtIntervalFromQuarterTonesPitch (
          inputLineNumber,
          kMajorSixth, // a minor third below actually
          fKeyTonicQuarterTonesPitchKind);
      break;
    case kIonianMode:
      break;
    case kDorianMode:
      break;
    case kPhrygianMode:
      break;
    case kLydianMode:
      break;
    case kMixolydianMode:
      break;
    case kAeolianMode:
      break;
    case kLocrianMode:
      break;
  } // switch

  fKeyCancel = keyCancel;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Creating traditional key '" <<
      this->asString () <<
      "'" <<
      endl;
    }
#endif
}

msrKey::msrKey ( // for Humdrum/Scot keys
  int                  inputLineNumber)
    : msrMeasureElement (inputLineNumber)
{
  // this is a Humdrum/Scot key
  fKeyKind = kHumdrumScotKind;

  // initialization in all cases
  fKeyItemsOctavesAreSpecified = false;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
      "Creating Humdrum/Scot key '" <<
      this->asString () <<
      "'" <<
      endl;
    }
#endif
}

msrKey::~msrKey ()
{}

bool msrKey::isEqualTo (S_msrKey otherKey) const
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
    case kTraditionalKind:
      break;
    case kHumdrumScotKind:
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

void msrKey::appendHumdrumScotKeyItem (
  S_msrHumdrumScotKeyItem item)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceKeys ()) {
    gLogStream <<
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

void msrKey::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrKey::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrKey::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrKey::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrKey::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrKey>*
    p =
      dynamic_cast<visitor<S_msrKey>*> (v)) {
        S_msrKey elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrKey::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrKey::browseData (basevisitor* v)
{}

string msrKey::asString () const
{
  stringstream s;

  s <<
    "[Key " <<
    keyKindAsString (fKeyKind) <<
    ", ";

  switch (fKeyKind) {
    case kTraditionalKind:
      s <<
        msrQuarterTonesPitchKindAsString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          fKeyTonicQuarterTonesPitchKind) <<
        " " <<
        keyModeKindAsString (fKeyModeKind);
      break;

    case kHumdrumScotKind:
      s <<
        fHumdrumScotKeyItemsVector.size () << "items" <<
        ", keyItemsOctavesAreSpecified: " <<
        booleanAsString (
          fKeyItemsOctavesAreSpecified);
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrKey::print (ostream& os) const
{
  os <<
    "Key" <<
    ", " << keyKindAsString (fKeyKind) <<
    ",";

  switch (fKeyKind) {
    case kTraditionalKind:
      os <<
        " " <<
        msrQuarterTonesPitchKindAsString (
          gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
          fKeyTonicQuarterTonesPitchKind) <<
        " " <<
        keyModeKindAsString (fKeyModeKind) <<
        ", line " << fInputLineNumber <<
        endl;
      break;

    case kHumdrumScotKind:
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
        os << endl;

        gIndenter++;

        vector<S_msrHumdrumScotKeyItem>::const_iterator
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
}

ostream& operator<< (ostream& os, const S_msrKey& elt)
{
  elt->print (os);
  return os;
}


}
