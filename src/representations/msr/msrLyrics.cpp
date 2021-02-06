/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>

#include "messagesHandling.h"

#include "msrLyrics.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrSyllable msrSyllable::create (
  int                   inputLineNumber,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  string                syllableStanzaNumber,
  rational              syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableStanzaUpLink)
{
  msrSyllable* o =
    new msrSyllable (
      inputLineNumber,
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableStanzaUpLink);
  assert (o != nullptr);

  return o;
}

S_msrSyllable msrSyllable::createWithNextMeasurePuristNumber (
  int                   inputLineNumber,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  string                syllableStanzaNumber,
  rational              syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableStanzaUpLink,
  int                   syllableNextMeasurePuristNumber)
{
  msrSyllable* o =
    new msrSyllable (
      inputLineNumber,
      syllableKind,
      syllableExtendKind,
      syllableStanzaNumber,
      syllableWholeNotes,
      syllableTupletFactor,
      syllableStanzaUpLink,
      syllableNextMeasurePuristNumber);
  assert (o != nullptr);

  return o;
}

msrSyllable::msrSyllable (
  int                   inputLineNumber,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  string                syllableStanzaNumber,
  rational              syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableStanzaUpLink)
    : msrMeasureElement (inputLineNumber)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    syllableStanzaUpLink != nullptr,
    "syllableStanzaUpLink is null");

  // set syllable's stanza upLink
  fSyllableStanzaUpLink =
    syllableStanzaUpLink;

  fSyllableKind = syllableKind;

  fSyllableExtendKind = syllableExtendKind;

  fSyllableStanzaNumber = syllableStanzaNumber;

  // fSyllableNoteUpLink will be set
  // by appendSyllableToNoteAndSetItsNoteUpLink () later

  fSyllableWholeNotes = syllableWholeNotes;

  fSyllableTupletFactor = syllableTupletFactor;

  fSyllableNextMeasurePuristNumber = -1;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a syllable containing:" <<
      endl;

    ++gIndenter;

    print (gLogStream);

    --gIndenter;
  }
#endif
}

msrSyllable::msrSyllable (
  int                   inputLineNumber,
  msrSyllableKind       syllableKind,
  msrSyllableExtendKind syllableExtendKind,
  string                syllableStanzaNumber,
  rational              syllableWholeNotes,
  msrTupletFactor       syllableTupletFactor,
  S_msrStanza           syllableStanzaUpLink,
  int                   syllableNextMeasurePuristNumber)
    : msrSyllable (
        inputLineNumber,
        syllableKind,
        syllableExtendKind,
        syllableStanzaNumber,
        syllableWholeNotes,
        syllableTupletFactor,
        syllableStanzaUpLink)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Setting syllable next measure purist number to " <<
      fSyllableNextMeasurePuristNumber <<
      endl;
  }
#endif

  switch (fSyllableKind) {
    case msrSyllable::kSyllableNone:
    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
    case msrSyllable::kSyllableOnRestNote:
    case msrSyllable::kSyllableSkipRestNote:
    case msrSyllable::kSyllableSkipNonRestNote:
    case msrSyllable::kSyllableMeasureEnd:
      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable with next measure purist number is no line nor page break");
      break;

    case msrSyllable::kSyllableLineBreak:
    case msrSyllable::kSyllablePageBreak:
      break;
  } // switch

  fSyllableNextMeasurePuristNumber = syllableNextMeasurePuristNumber;
}

msrSyllable::~msrSyllable ()
{}

S_msrSyllable msrSyllable::createSyllableNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrSyllable
    newbornClone =
      msrSyllable::create (
        fInputLineNumber,
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableStanzaUpLink);

  // append the lyric texts to the syllable clone
  for (
    list<string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    ++i
  ) {
    newbornClone->
      appendLyricTextToSyllable ((*i));
  } // for

  // dont't set 'newbornClone->fSyllableStanzaUpLink'
  // nor 'newbornClone->fSyllableNoteUpLink',
  // this will be done by the caller

  newbornClone->fSyllableNoteUpLink =
    fSyllableNoteUpLink; // TEMP

  return newbornClone;
}

S_msrSyllable msrSyllable::createSyllableDeepCopy (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrSyllable
    syllableDeepCopy =
      msrSyllable::create (
        fInputLineNumber,
        fSyllableKind,
        fSyllableExtendKind,
        fSyllableStanzaNumber,
        fSyllableWholeNotes,
        fSyllableTupletFactor,
        fSyllableStanzaUpLink);

  // append the lyric texts to the syllable deep copy
  for (
    list<string>::const_iterator i = fSyllableTextsList.begin ();
    i!=fSyllableTextsList.end ();
    ++i
  ) {
    syllableDeepCopy->
      appendLyricTextToSyllable ((*i));
  } // for

  // dont't set 'newbornClone->fSyllableStanzaUpLink'
  // nor 'newbornClone->fSyllableNoteUpLink',
  // this will be done by the caller

  syllableDeepCopy->fSyllableNoteUpLink =
    fSyllableNoteUpLink; // TEMP

  return syllableDeepCopy;
}

void msrSyllable::appendLyricTextToSyllable (string text)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending text \"" <<
      text <<
      "\" to the texts list of syllable '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fSyllableTextsList.push_back (
    text);
}

void msrSyllable::appendSyllableToNoteAndSetItsNoteUpLink (
  S_msrNote note)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    note != nullptr,
    "note is empty");

  fSyllableNoteUpLink = note;

/*
  // sanity check JMI ???
  msgAssert (
    __FILE__, __LINE__,
    fSyllableTextsList.size () != 0,
    "fSyllableTextsList is empty");
    */

  // append syllable to note
  note->
    appendSyllableToNote (this);

  // set it upLink to note
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Setting syllable note upLink for:" <<
      endl;

    ++gIndenter;

    gLogStream <<
      asString () <<
    // JMI    "to '" << note->asString () <<
      ", line " << note->getInputLineNumber () <<
      endl;

    --gIndenter;
  }
#endif
}

void msrSyllable::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSyllable::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSyllable::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSyllable::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrSyllable::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSyllable>*
    p =
      dynamic_cast<visitor<S_msrSyllable>*> (v)) {
        S_msrSyllable elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrSyllable::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSyllable::browseData (basevisitor* v)
{}

string msrSyllable::syllableWholeNotesAsMsrString () const
{
  string result;

  if (fSyllableNoteUpLink) { // JMI
    switch (fSyllableNoteUpLink->getNoteKind ()) {
      case k_NoNote:
      case kNoteRestInMeasure:
      case kNoteUnpitchedInMeasure:
      case kNoteRegularInMeasure:
      case kNoteInDoubleTremolo:
      case kNoteRegularInGraceNotesGroup:
      case kNoteSkipInGraceNotesGroup:
      case kNoteInChordInGraceNotesGroup:
      case kNoteRegularInChord:
        result =
          wholeNotesAsMsrString (
            fInputLineNumber,
            fSyllableWholeNotes);
        break;

      case kNoteSkipInMeasure:
      case kNoteRegularInTuplet:
      case kNoteRestInTuplet:
      case kNoteInTupletInGraceNotesGroup:
      case kNoteUnpitchedInTuplet:
        {
          stringstream s;

          s <<
            fSyllableNoteUpLink->
      // JMI        noteSoundingWholeNotesAsMsrString () <<
              noteDisplayWholeNotesAsMsrString () <<
            "*" <<
            fSyllableTupletFactor.getTupletNormalNotes () <<
            "/" <<
            fSyllableTupletFactor.getTupletActualNotes ();

          result = s.str ();
        }
        break;
    } // switch
  }

  else {
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
          fSyllableWholeNotes);
  }

  return result;
}

string msrSyllable::syllableKindAsString (
  msrSyllableKind syllableKind)
{
  string result;

  switch (syllableKind) {
    case msrSyllable::kSyllableNone:
      result = "syllableNone";
      break;

    case msrSyllable::kSyllableSingle:
      result = "syllableSingle";
      break;

    case msrSyllable::kSyllableBegin:
      result = "syllableBegin";
      break;
    case msrSyllable::kSyllableMiddle:
      result = "syllableMiddle";
      break;
    case msrSyllable::kSyllableEnd:
      result = "syllableEnd";
      break;

    case msrSyllable::kSyllableOnRestNote:
      result = "syllableOnRestNote";
      break;

    case msrSyllable::kSyllableSkipRestNote:
      result = "syllableSkipRestNote";
      break;
    case msrSyllable::kSyllableSkipNonRestNote:
      result = "syllableSkipNonRestNote";
      break;

    case msrSyllable::kSyllableMeasureEnd:
      result = "syllableMeasureEnd";
      break;

    case msrSyllable::kSyllableLineBreak:
      result = "syllableLineBreak";
      break;
    case msrSyllable::kSyllablePageBreak:
      result = "syllablePageBreak";
      break;
  } // switch

  return result;
}

string msrSyllable::syllableKindAsString () const
{
  return syllableKindAsString (fSyllableKind);
}

string msrSyllable::syllableExtendKindAsString (
  msrSyllableExtendKind syllableExtendKind)
{
  string result;

  switch (syllableExtendKind) {
    case msrSyllable::kSyllableExtendNone:
      result = "syllableExtendNone";
      break;
    case msrSyllable::kSyllableExtendEmpty:
      result = "syllableExtendEmpty";
      break;
    case msrSyllable::kSyllableExtendSingle:
      result = "syllableExtendSingle";
      break;
    case msrSyllable::kSyllableExtendStart:
      result = "syllableExtendStart";
      break;
    case msrSyllable::kSyllableExtendContinue:
      result = "syllableExtendContinue";
      break;
    case msrSyllable::kSyllableExtendStop:
      result = "syllableExtendStop";
      break;
  } // switch

  return result;
}

string msrSyllable::syllableExtendKindAsString () const
{
  return syllableExtendKindAsString (fSyllableExtendKind);
}

string msrSyllable::syllableNoteUpLinkAsString () const
{
  string result;

  if (fSyllableNoteUpLink) {
    result = fSyllableNoteUpLink->asString ();
  }
  else {
    result = "none";
  }

  return result;
}

string msrSyllable::syllableTextsListAsString () const
{
  stringstream s;

  if (fSyllableTextsList.size ()) {
    list<string>::const_iterator
      iBegin = fSyllableTextsList.begin (),
      iEnd   = fSyllableTextsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << doubleQuoteStringIfNonAlpha (*i);
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  return s.str ();
}

void msrSyllable::writeTextsList (
  const list<string>& textsList,
  ostream&            os)
{
  os << "[";

  if (textsList.size ()) {
    list<string>::const_iterator
      iBegin = textsList.begin (),
      iEnd   = textsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << doubleQuoteStringIfNonAlpha (*i);
      if (++i == iEnd) break;
      os << ", ";
    } // for
  }

  os << "]";
}

string msrSyllable::asString () const
{
  stringstream s;

  s <<
    "Syllable '" <<
    syllableKindAsString () <<
    "', syllableExtendKind: " <<
      syllableExtendKindAsString (fSyllableExtendKind) <<
    ", syllableStanzaNumber: \"" << fSyllableStanzaNumber << "\"" <<
    ", syllableWholeNotes: " << fSyllableWholeNotes <<
    ", syllableTupletFactor: " << fSyllableTupletFactor <<
    ", syllableNextMeasurePuristNumber: " << fSyllableNextMeasurePuristNumber <<
    ", line " << fInputLineNumber <<
    ", syllableTextsList: ";

  writeTextsList (
    fSyllableTextsList,
    s);

  s <<
    ", " <<
    syllableNoteUpLinkAsString ();

  switch (fSyllableKind) {
    case msrSyllable::kSyllableNone:
      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;

    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
    case msrSyllable::kSyllableOnRestNote:
    case msrSyllable::kSyllableSkipRestNote:
    case msrSyllable::kSyllableSkipNonRestNote:
    case msrSyllable::kSyllableMeasureEnd:
    case msrSyllable::kSyllableLineBreak:
    case msrSyllable::kSyllablePageBreak:
      break;
  } // switch

  s <<
    ", attached to note " <<
    fSyllableNoteUpLink->asShortString () <<
    ", in stanza " <<
    fSyllableStanzaUpLink->getStanzaName ();

  return s.str ();
}

void msrSyllable::print (ostream& os) const
{
  os <<
    "Syllable" <<
    ", syllableKind: " <<
    syllableKindAsString () <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 32;
  os << left <<
    setw (fieldWidth) <<
    "syllableNoteUpLink" << " : " <<
    syllableNoteUpLinkAsString () <<
    endl <<
    setw (fieldWidth) <<
    "syllableStanzaUpLink" << " : " <<
    fSyllableStanzaUpLink->getStanzaName () <<
    endl <<

    setw (fieldWidth) <<
    "syllableKind" << " : " <<
    syllableKindAsString (fSyllableKind) <<
    endl <<
    setw (fieldWidth) <<
    "syllableExtendKind" << " : " <<
    syllableExtendKindAsString (
      fSyllableExtendKind) <<
    endl <<

    setw (fieldWidth) <<
    "syllableStanzaNumber" << " : \"" <<
    fSyllableStanzaNumber <<
    "\"" <<
    endl <<

    setw (fieldWidth) <<
    "syllableWholeNotes" << " : " <<
    fSyllableWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "syllableTupletFactor" << " : " <<
    fSyllableTupletFactor.asString () <<
    endl <<

    setw (fieldWidth) <<
    "syllableNextMeasurePuristNumber" << " : " <<
    fSyllableNextMeasurePuristNumber <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "syllableTextsList" << " : ";

  writeTextsList (
    fSyllableTextsList,
    os);

  switch (fSyllableKind) { // JMI
    case msrSyllable::kSyllableNone:
      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;

    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
    case msrSyllable::kSyllableOnRestNote:
    case msrSyllable::kSyllableSkipRestNote:
    case msrSyllable::kSyllableSkipNonRestNote:
    case msrSyllable::kSyllableMeasureEnd:
    case msrSyllable::kSyllableLineBreak:
    case msrSyllable::kSyllablePageBreak:
      break;
  } // switch
  os << endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrSyllable& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrStanza msrStanza::create (
  int           inputLineNumber,
  string        stanzaNumber,
  S_msrVoice    stanzaVoiceUpLink)
{
  msrStanza* o =
    new msrStanza (
      inputLineNumber,
      stanzaNumber,
      stanzaVoiceUpLink);
  assert (o != nullptr);

  return o;
}

msrStanza::msrStanza (
  int           inputLineNumber,
  string        stanzaNumber,
  S_msrVoice    stanzaVoiceUpLink)
    : msrElement (inputLineNumber)
{
  // set stanza number and kind
  fStanzaNumber = stanzaNumber;

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    stanzaVoiceUpLink != nullptr,
    "stanzaVoiceUpLink is null");

  // set stanza's voice upLink
  fStanzaVoiceUpLink =
    stanzaVoiceUpLink;

  // do other initializations
  initializeStanza ();
}

void msrStanza::initializeStanza ()
{
  fStanzaName =
    fStanzaVoiceUpLink->getVoiceName () +
    "_Stanza_" +
      stringNumbersToEnglishWords (
        makeSingleWordFromString (
          fStanzaNumber));

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Initializing stanza " << getStanzaName () <<
      endl;
  }
#endif

  fStanzaTextPresent = false;

  fStanzaCurrentMeasureWholeNotesDuration = rational (0, 1);
}

msrStanza::~msrStanza ()
{}

S_msrStanza msrStanza::createStanzaNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a newborn clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrStanza
    newbornClone =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // name
  newbornClone->fStanzaName =
    fStanzaName;

  // contents
  newbornClone->fStanzaTextPresent =
    fStanzaTextPresent;

  // upLinks
  newbornClone->fStanzaVoiceUpLink =
    containingVoice;

  return newbornClone;
}

S_msrStanza msrStanza::createStanzaDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Creating a deep copy of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrStanza
    stanzaDeepCopy =
      msrStanza::create (
        fInputLineNumber,
        fStanzaNumber,
        containingVoice);

  // number

  // kind

  // name
  stanzaDeepCopy->fStanzaName =
    fStanzaName;

  // contents
  int n = fSyllables.size ();
  for (int i = 0; i < n; ++i) {
    stanzaDeepCopy->fSyllables.push_back (
      fSyllables [i]->
        createSyllableDeepCopy (
          containingVoice->
            fetchVoicePartUpLink ()));
  } // for

  stanzaDeepCopy->fStanzaTextPresent =
    fStanzaTextPresent;

  // upLinks
  stanzaDeepCopy->fStanzaVoiceUpLink =
    containingVoice;

  return stanzaDeepCopy;
}

void msrStanza::appendSyllableToStanza (
  S_msrSyllable syllable)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending syllable '" << syllable->asString () <<
      "' to stanza " << getStanzaName () <<
      endl;
  }
#endif

  // append the syllable to this stanza
  fSyllables.push_back (syllable);

  // does this stanza contain text?
  switch (syllable->getSyllableKind ()) {

    case msrSyllable::kSyllableSingle:
    case msrSyllable::kSyllableBegin:
    case msrSyllable::kSyllableMiddle:
    case msrSyllable::kSyllableEnd:
      // only now, in case addSyllableToStanza () is called
      // from LPSR for example
      fStanzaTextPresent = true;
      break;

    case msrSyllable::kSyllableOnRestNote:
    case msrSyllable::kSyllableSkipRestNote:
    case msrSyllable::kSyllableSkipNonRestNote:
    case msrSyllable::kSyllableMeasureEnd:
    case msrSyllable::kSyllableLineBreak:
    case msrSyllable::kSyllablePageBreak:
      break;

    case msrSyllable::kSyllableNone:
      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "syllable type has not been set");
      break;
  } // switch

/* JMI
  // get the syllable's sounding whole notes
  rational
    syllableSoundingWholeNotes =
      syllable->
        getSyllableNoteUpLink ()->
          getNoteSoundingWholeNotes ();

  // update the stanza's current measure whole notes
  fStanzaCurrentMeasureWholeNotesDuration +=syllableSoundingWholeNotes;
  */
}

S_msrSyllable msrStanza::appendRestSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending 'Rest' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create stanza rest syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableSkipRestNote,
        msrSyllable::kSyllableExtendNone,
        fStanzaNumber,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSkipSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending 'Skip' syllable " <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableSkipRestNote,
        msrSyllable::kSyllableExtendNone,
        fStanzaNumber,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMeasureEndSyllableToStanza (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending 'Measure end' syllable " <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableMeasureEnd,
        msrSyllable::kSyllableExtendNone,
        fStanzaNumber,
        0, // wholeNotes
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // reset current measure whole notes
  fStanzaCurrentMeasureWholeNotesDuration = rational (0, 1);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMelismaSyllableToStanza (
  int             inputLineNumber,
  msrSyllable::msrSyllableKind
                  syllableKind,
  rational        wholeNotes)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending '" <<
      msrSyllable::syllableKindAsString (syllableKind) <<
      "' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create stanza melisma syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        syllableKind,
        msrSyllable::kSyllableExtendNone,
        fStanzaNumber,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int inputLineNumber,
  int nextMeasurePuristNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending a 'LineBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", nextMeasurePuristNumber: " << nextMeasurePuristNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create line break syllable
  S_msrSyllable
    syllable =
      msrSyllable::createWithNextMeasurePuristNumber (
        inputLineNumber,
        msrSyllable::kSyllableLineBreak,
        msrSyllable::kSyllableExtendNone,
        fStanzaNumber,
        0, // whole notes
        msrTupletFactor (),
        this,
        nextMeasurePuristNumber);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendPageBreakSyllableToStanza (
  int inputLineNumber,
  int nextMeasurePuristNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending a 'PageBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", nextMeasurePuristNumber: " << nextMeasurePuristNumber <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // create page break syllable
  S_msrSyllable
    syllable =
      msrSyllable::createWithNextMeasurePuristNumber (
        inputLineNumber,
        msrSyllable::kSyllablePageBreak,
        msrSyllable::kSyllableExtendNone,
        fStanzaNumber,
        0, // whole notes
        msrTupletFactor (),
        this,
        nextMeasurePuristNumber);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  --gIndenter;

  // and return it
  return syllable;
}

void msrStanza::padUpToCurrentMeasureWholeNotesDurationInStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
  // JMI ???
}

void msrStanza::appendPaddingNoteToStanza (
  int      inputLineNumber,
  rational forwardStepLength)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending padding note" <<
      ", forwardStepLength: " <<
      forwardStepLength <<
      ", to stanza \"" <<
      fStanzaName <<
      "\" in voice \"" <<
      fStanzaVoiceUpLink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  ++gIndenter;

  // JMI TO DO ???

  --gIndenter;
}

void msrStanza::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStanza::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStanza::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStanza::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStanza::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStanza::browseData ()" <<
      endl;
  }

  ++gIndenter;

  // browse the syllables
  int n = fSyllables.size ();
  for (int i = 0; i < n; ++i) {
    // browse the syllable
    msrBrowser<msrSyllable> browser (v);
    browser.browse (*fSyllables [i]);
  } // for

  --gIndenter;

  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% <== msrStanza::browseData ()" <<
      endl;
  }
}

void msrStanza::print (ostream& os) const
{
  os <<
    "Stanza " << getStanzaName () <<
    " (number \"" <<
    fStanzaNumber <<
    "\", " <<
    fSyllables.size () << " syllables)" <<
    endl;

  ++gIndenter;

  if (! fStanzaTextPresent) {
    os <<
      "(No actual text)" <<
      endl;
  }

  else {
    if (fSyllables.size ()) {
      vector<S_msrSyllable>::const_iterator
        iBegin = fSyllables.begin (),
        iEnd   = fSyllables.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here
      } // for
    }
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrStanza& elt)
{
  elt->print (os);
  return os;
}


}
