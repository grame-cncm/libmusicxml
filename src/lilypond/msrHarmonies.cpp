/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrMutualDependencies.h"

#include "generalOptions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrHarmonyDegree msrHarmonyDegree::create (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlterationKind        harmonyDegreeAlterationKind,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  msrHarmonyDegree* o =
    new msrHarmonyDegree (
      inputLineNumber,
      harmonyDegreeValue,
      harmonyDegreeAlterationKind,
      harmonyDegreeTypeKind);
  assert(o!=0);

  return o;
}

msrHarmonyDegree::msrHarmonyDegree (
  int                      inputLineNumber,
  int                      harmonyDegreeValue,
  msrAlterationKind        harmonyDegreeAlterationKind,
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
    : msrElement (inputLineNumber)
{
  fHarmonyDegreeValue          = harmonyDegreeValue;
  fHarmonyDegreeAlterationKind = harmonyDegreeAlterationKind;
  fHarmonyDegreeTypeKind       = harmonyDegreeTypeKind;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    glogIOstream <<
      "Creating harmony degree '" <<
      asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif
}

msrHarmonyDegree::~msrHarmonyDegree ()
{}

void msrHarmonyDegree::setHarmonyDegreeHarmonyUplink (
  S_msrHarmony harmonyUplink)
{
  // sanity check
  msrAssert(
    harmonyUplink != nullptr,
     "harmonyUplink is null");

  fHarmonyDegreeHarmonyUplink =
    harmonyUplink;
}

int msrHarmonyDegree::harmonyDegreeAsSemitones () const
{
/*
  Kind indicates the type of chord. Degree elements
  can then add, subtract, or alter from these
  starting points.
*/

  int result = -1;

  // determine the value for an unaltered degree value
  switch (fHarmonyDegreeValue) {
    case 0:
      result = 0;
      break;
    case 1:
      result = 0;
      break;
    case 2:
      result = 0;
      break;
    case 3:
      result = 0;
      break;
    case 4:
      result = 0;
      break;
    case 5:
      result = 0;
      break;
    case 6:
      result = 0;
      break;
    case 7:
      result = 0;
      break;
    case 8:
      result = 0;
      break;
    case 9:
      result = 0;
      break;
    case 10:
      result = 0;
      break;
    case 11:
      result = 0;
      break;
    case 12:
      result = 0;
      break;
    case 13:
      result = 0;
      break;
  } // switch

  return result;
}

void msrHarmonyDegree::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    glogIOstream <<
      "% ==> msrHarmonyDegree::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          glogIOstream <<
            "% ==> Launching msrHarmonyDegree::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmonyDegree::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    glogIOstream <<
      "% ==> msrHarmonyDegree::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmonyDegree>*
    p =
      dynamic_cast<visitor<S_msrHarmonyDegree>*> (v)) {
        S_msrHarmonyDegree elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          glogIOstream <<
            "% ==> Launching msrHarmonyDegree::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmonyDegree::browseData (basevisitor* v)
{}

string msrHarmonyDegree::harmonyDegreeTypeKindAsString (
  msrHarmonyDegreeTypeKind harmonyDegreeTypeKind)
{
  string result;

  switch (harmonyDegreeTypeKind) {
    case msrHarmonyDegree::kHarmonyDegreeTypeAdd:
      result = "harmonyDegreeTypeAdd";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
      result = "harmonyDegreeTypeAlter";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeSubstract:
      result = "harmonyDegreeTypeSubstract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::harmonyDegreeKindAsShortString () const
{
  string result;

  switch (fHarmonyDegreeTypeKind) {
    case msrHarmonyDegree::kHarmonyDegreeTypeAdd:
      result = "degreeAdd";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeAlter:
      result = "degreeAlter";
      break;
    case msrHarmonyDegree::kHarmonyDegreeTypeSubstract:
      result = "degreeSubtract";
      break;
  } // switch

  return result;
}

string msrHarmonyDegree::asString () const
{
  stringstream s;

  s <<
    "HarmonyDegree" <<
    ", type: " << harmonyDegreeKindAsShortString () <<
    ", value: " << fHarmonyDegreeValue <<
    ", alteration: " <<
    msrAlterationKindAsString (
      fHarmonyDegreeAlterationKind) <<
    ", line: " << fInputLineNumber;

  return s.str ();
}

void msrHarmonyDegree::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrHarmonyDegree& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrHarmony msrHarmony::create (
  int                      inputLineNumber,
  // no harmonyVoiceUplink yet
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  string                   harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  rational                 harmonySoundingWholeNotes,
  rational                 harmonyDisplayWholeNotes,
  int                      harmonyStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  rational                 harmonyWholeNotesOffset)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      nullptr,
      harmonyRootQuarterTonesPitchKind,
      harmonyKind,
      harmonyKindText,
      harmonyInversion,
      harmonyBassQuarterTonesPitchKind,
      harmonySoundingWholeNotes,
      harmonyDisplayWholeNotes,
      harmonyStaffNumber,
      harmonyTupletFactor,
      harmonyWholeNotesOffset);
  assert(o!=0);

  return o;
}

S_msrHarmony msrHarmony::create (
  int                      inputLineNumber,
  S_msrVoice               harmonyVoiceUplink,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  string                   harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  rational                 harmonySoundingWholeNotes,
  rational                 harmonyDisplayWholeNotes,
  int                      harmonyStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  rational                 harmonyWholeNotesOffset)
{
  msrHarmony* o =
    new msrHarmony (
      inputLineNumber,
      harmonyVoiceUplink,
      harmonyRootQuarterTonesPitchKind,
      harmonyKind,
      harmonyKindText,
      harmonyInversion,
      harmonyBassQuarterTonesPitchKind,
      harmonySoundingWholeNotes,
      harmonyDisplayWholeNotes,
      harmonyStaffNumber,
      harmonyTupletFactor,
      harmonyWholeNotesOffset);
  assert(o!=0);

  return o;
}

msrHarmony::msrHarmony (
  int                      inputLineNumber,
  S_msrVoice               harmonyVoiceUplink,
  msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
  msrHarmonyKind           harmonyKind,
  string                   harmonyKindText,
  int                      harmonyInversion,
  msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
  rational                 harmonySoundingWholeNotes,
  rational                 harmonyDisplayWholeNotes,
  int                      harmonyStaffNumber,
  msrTupletFactor          harmonyTupletFactor,
  rational                 harmonyWholeNotesOffset)
    : msrMeasureElement (inputLineNumber),
     fHarmonyTupletFactor (harmonyTupletFactor)
{
  /* JMI
  // sanity check
  msrAssert(
    harmonyVoiceUplink != nullptr,
     "harmonyVoiceUplink is null");
     */

  // set harmony's voice uplink
  fHarmonyVoiceUplink =
    harmonyVoiceUplink;

  fHarmonyRootQuarterTonesPitchKind =
    harmonyRootQuarterTonesPitchKind;

  fHarmonyKind     = harmonyKind;
  fHarmonyKindText = harmonyKindText;

  fHarmonyInversion = harmonyInversion;

  fHarmonyBassQuarterTonesPitchKind =
    harmonyBassQuarterTonesPitchKind;

  fHarmonySoundingWholeNotes =
    harmonySoundingWholeNotes;
  fHarmonyDisplayWholeNotes =
    harmonyDisplayWholeNotes;

  fHarmonyStaffNumber = harmonyStaffNumber;

  fHarmonyWholeNotesOffset = harmonyWholeNotesOffset;

  // handle harmony inversion if any
  /*
    Inversion is a number indicating which inversion is used:
    0 for root position, 1 for first inversion, etc.
  */
  if (fHarmonyInversion > 0) {
    // fetch the chord intervals
    S_msrChordStructure
      chordStructure =
        msrChordStructure::create (
  // JMI        inputLineNumber,
          fHarmonyKind);

    // fetch the bass chord item for the inversion
    S_msrChordInterval
      bassChordInterval =
        chordStructure->
          bassChordIntervalForChordInversion (
            inputLineNumber,
            fHarmonyInversion);

    // fetch the inverted chord bass semitones pitch
    msrQuarterTonesPitchKind
      invertedChordBassQuarterTonesPitchKind =
        noteAtIntervalFromQuarterTonesPitch (
          inputLineNumber,
          bassChordInterval->getChordIntervalIntervalKind (),
          fHarmonyRootQuarterTonesPitchKind);

    // is this compatible with bass quartertones pitch if specified?
    if (fHarmonyBassQuarterTonesPitchKind != k_NoQuarterTonesPitch_QTP) {
      if (
        invertedChordBassQuarterTonesPitchKind
          !=
        fHarmonyBassQuarterTonesPitchKind
        ) {
        stringstream s;

        s <<
          "inversion '" <<
          fHarmonyInversion <<
          "' is not compatible with bass quaternotes pitch '" <<
          msrQuarterTonesPitchKindAsString (
            gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
            fHarmonyBassQuarterTonesPitchKind) <<
          "'";

        msrMusicXMLError (
          gGeneralOptions->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
    }

    // set the bass quartertones pitch according to the inversion
    fHarmonyBassQuarterTonesPitchKind =
      invertedChordBassQuarterTonesPitchKind;
  }

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    glogIOstream <<
      "Creating harmony '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrHarmony::~msrHarmony ()
{}

S_msrHarmony msrHarmony::createHarmonyNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    glogIOstream <<
      "Creating a newborn clone of harmony '" <<
      msrHarmonyKindAsShortString (fHarmonyKind) <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrHarmony
    newbornClone =
      msrHarmony::create (
        fInputLineNumber,
        containingVoice,
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind,
        fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
        fHarmonySoundingWholeNotes,
        fHarmonyDisplayWholeNotes,
        fHarmonyStaffNumber,
        fHarmonyTupletFactor,
        fHarmonyWholeNotesOffset);

  return newbornClone;
}

S_msrHarmony msrHarmony::createHarmonyDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceHarmonies) {
    glogIOstream <<
      "Creating a deep copy of harmony '" <<
      msrHarmonyKindAsShortString (fHarmonyKind) <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrHarmony
    harmonyDeepCopy =
      msrHarmony::create (
        fInputLineNumber,
        containingVoice,
        fHarmonyRootQuarterTonesPitchKind,
        fHarmonyKind, fHarmonyKindText,
        fHarmonyInversion,
        fHarmonyBassQuarterTonesPitchKind,
        fHarmonySoundingWholeNotes,
        fHarmonyDisplayWholeNotes,
        fHarmonyStaffNumber,
        fHarmonyTupletFactor,
        fHarmonyWholeNotesOffset);

  return harmonyDeepCopy;
}

bool msrHarmony::compareHarmoniesByIncreasingOffset (
  const SMARTP<msrHarmony>& first,
  const SMARTP<msrHarmony>& second)
{
  return
    first->getHarmonyWholeNotesOffset ()
      <
    second->getHarmonyWholeNotesOffset ();
}

void msrHarmony::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    glogIOstream <<
      "% ==> msrHarmony::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          glogIOstream <<
            "% ==> Launching msrHarmony::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrHarmony::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    glogIOstream <<
      "% ==> msrHarmony::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrHarmony>*
    p =
      dynamic_cast<visitor<S_msrHarmony>*> (v)) {
        S_msrHarmony elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          glogIOstream <<
            "% ==> Launching msrHarmony::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrHarmony::browseData (basevisitor* v)
{
  // browse harmony degrees if any
  if (fHarmonyDegreesList.size ()) {
    for (
      list<S_msrHarmonyDegree>::const_iterator i = fHarmonyDegreesList.begin ();
      i != fHarmonyDegreesList.end ();
      i++
    ) {
      // browse the harmony degree
      msrBrowser<msrHarmonyDegree> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrHarmony::asString () const
{
  stringstream s;

  s <<
    "Harmony" <<
    ", line " << fInputLineNumber <<
    ": harmonyRootQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsString ( // JMI XXL
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fHarmonyRootQuarterTonesPitchKind) <<
    ", harmonyKind: " <<
    msrHarmonyKindAsShortString (fHarmonyKind) <<
    " (" << fHarmonyKind << ")" <<

    ", harmonySoundingWholeNotes: " <<
    fHarmonySoundingWholeNotes <<
    ", harmonyDisplayWholeNotes: " <<
    fHarmonyDisplayWholeNotes <<

    ", harmonyKindText: \"" <<
    fHarmonyKindText << "\"";

  s << ", inversion: ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION) {
    s << "none";
  }
  else {
    s << fHarmonyInversion;
  }

  if (fHarmonyBassQuarterTonesPitchKind != k_NoQuarterTonesPitch_QTP) {
    s <<
      ", harmonyBassQuarterTonesPitchKind: " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fHarmonyBassQuarterTonesPitchKind);
  }

  if (fHarmonyDegreesList.size ()) {
    s <<
      ", harmonyDegreesList: [";

    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << " ";
    } // for

    s <<
      "]";
  }

  // print the harmony staff number
  s <<
    ", harmonyStaffNumber: ";
  if (fHarmonyStaffNumber == K_NO_STAFF_NUMBER)
    s << "none";
  else
    s << fHarmonyStaffNumber;

  // print the harmony tuplet factor
  s <<
    ", harmonyTupletFactor: " <<
    fHarmonyTupletFactor.asString ();

  // print the harmony offset
  s <<
    ", harmonyWholeNotesOffset: " << fHarmonyWholeNotesOffset;

  // print the harmony position in measure
  s <<
    ", positionInMeasure: " << fPositionInMeasure;
  return s.str ();
}

void msrHarmony::print (ostream& os)
{
  os <<
    "Harmony" <<
     ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 26;

  os <<
    setw (fieldWidth) <<
    "harmonyNoteUplink" << " : ";
  if (fHarmonyNoteUplink) {
    os << fHarmonyNoteUplink->asString ();
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "harmonyRoot" << " : " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fHarmonyRootQuarterTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "harmonyKind" << " : " <<
    msrHarmonyKindAsString (fHarmonyKind) <<
    endl <<

/* JMI
    setw (fieldWidth) <<
    "harmonySoundingWholeNotes " << " : " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fHarmonySoundingWholeNotes) <<
    endl <<
    setw (fieldWidth) <<
    "harmonyDisplayWholeNotes " << " : " <<
    wholeNotesAsMsrString (
      fInputLineNumber,
      fHarmonyDisplayWholeNotes) <<
    endl <<
*/
    setw (fieldWidth) <<
    "harmonySoundingWholeNotes" << " : " <<
    fHarmonySoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "harmonyDisplayWholeNotes" << " : " <<
    fHarmonyDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "harmonyKindText" << " = \"" <<
    fHarmonyKindText <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "harmonyBass" << " : " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fHarmonyBassQuarterTonesPitchKind) <<
    endl;

  os <<
    setw (fieldWidth) <<
    "harmonyInversion" << " : ";
  if (fHarmonyInversion == K_HARMONY_NO_INVERSION) {
    os << "none";
  }
  else {
    os << fHarmonyInversion;
  }
  os << endl;

  // print harmony degrees if any
  os <<
    setw (fieldWidth) <<
    "harmonyDegrees";

  if (fHarmonyDegreesList.size ()) {
    os << endl;

    gIndenter++;

    list<S_msrHarmonyDegree>::const_iterator
      iBegin = fHarmonyDegreesList.begin (),
      iEnd   = fHarmonyDegreesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os <<
        (*i)->asString ();
      if (++i == iEnd) break;
      os << endl;
    } // for

    os << endl;

    gIndenter--;
  }
  else {
    os <<
      " : " <<
      "none" <<
      endl;
  }

  // print the harmony staff number
  os <<
    setw (fieldWidth) <<
    "harmonyStaffNumber" << " : ";
  if (fHarmonyStaffNumber == K_NO_STAFF_NUMBER)
    os << "none";
  else
    os << fHarmonyStaffNumber;
  os << endl;

  // print the harmony tuplet factor
  os <<
    setw (fieldWidth) <<
    "harmonyTupletFactor" << " : " << fHarmonyTupletFactor.asString () <<
    endl;

  // print the harmony whole notes offset
  os <<
    setw (fieldWidth) <<
    "harmonyWholeNotesOffset" << " : " << fHarmonyWholeNotesOffset <<
    endl;

  // print the harmony position in measure
  os <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fPositionInMeasure <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrHarmony& elt)
{
  elt->print (os);
  return os;
}


}
