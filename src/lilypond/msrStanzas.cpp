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

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_msrStanza msrStanza::create (
  int           inputLineNumber,
  string        stanzaNumber,
  S_msrVoice    stanzaVoiceUplink)
{
  msrStanza* o =
    new msrStanza (
      inputLineNumber,
      stanzaNumber,
      stanzaVoiceUplink);
  assert(o!=0);

  return o;
}

msrStanza::msrStanza (
  int           inputLineNumber,
  string        stanzaNumber,
  S_msrVoice    stanzaVoiceUplink)
    : msrElement (inputLineNumber)
{
  // set stanza number and kind
  fStanzaNumber = stanzaNumber;

  // sanity check
  msrAssert(
    stanzaVoiceUplink != nullptr,
    "stanzaVoiceUplink is null");

  // set stanza's voice uplink
  fStanzaVoiceUplink =
    stanzaVoiceUplink;
  
  // do other initializations
  initializeStanza ();
}

void msrStanza::initializeStanza ()
{
  fStanzaName =
    fStanzaVoiceUplink->getVoiceName () +
    "_Stanza_" + 
      stringNumbersToEnglishWords (
        makeSingleWordFromString (
          fStanzaNumber));
 
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Initializing stanza " << getStanzaName () <<
      endl;
  }
#endif

  fStanzaTextPresent = false;

  fStanzaCurrentMeasureLength = rational (0, 1);
}

msrStanza::~msrStanza ()
{}

S_msrStanza msrStanza::createStanzaNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a newborn clone of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
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

  // uplinks
  newbornClone->fStanzaVoiceUplink =
    containingVoice;
  
  return newbornClone;
}

S_msrStanza msrStanza::createStanzaDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Creating a deep copy of stanza \"" <<
      getStanzaName () <<
      "\" in voice \"" <<
      containingVoice->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
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
  for (int i = 0; i < n; i++) {
    stanzaDeepCopy->fSyllables.push_back (
      fSyllables [i]->
        createSyllableDeepCopy (
          containingVoice->
            fetchVoicePartUplink ()));
  } // for

  stanzaDeepCopy->fStanzaTextPresent =
    fStanzaTextPresent;

  // uplinks
  stanzaDeepCopy->fStanzaVoiceUplink =
    containingVoice;
  
  return stanzaDeepCopy;
}

void msrStanza::appendSyllableToStanza (
  S_msrSyllable syllable)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
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
      
    case msrSyllable::kSyllableSkip:
    case msrSyllable::kSyllableMeasureEnd:
    case msrSyllable::kSyllableLineBreak:
    case msrSyllable::kSyllablePageBreak:
      break;
      
    case msrSyllable::kSyllableNone:
      msrInternalError (
        gGeneralOptions->fInputSourceName,
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
        getSyllableNoteUplink ()->
          getNoteSoundingWholeNotes ();

  // update the stanza's current measure length
  fStanzaCurrentMeasureLength +=syllableSoundingWholeNotes;
  */
}

S_msrSyllable msrStanza::appendRestSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Rest' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza rest syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableSkip, // JMI ??? kSyllableRest,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendSkipSyllableToStanza (
  int      inputLineNumber,
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Skip' syllable " <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableSkip,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMeasureEndSyllableToStanza (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'Measure end' syllable " <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza skip syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableMeasureEnd,
        msrSyllable::kSyllableExtendNone,
        0, // wholeNotes
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  // reset current measure length
  fStanzaCurrentMeasureLength = rational (0, 1);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendMelismaSyllableToStanza (
  int             inputLineNumber,
  msrSyllable::msrSyllableKind
                  syllableKind,
  rational        wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending '" <<
      msrSyllable::syllableKindAsString (syllableKind) <<
      "' syllable" <<
      " to stanza " << getStanzaName () <<
      ", whole notes = " << wholeNotes <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza melisma syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        syllableKind,
        msrSyllable::kSyllableExtendNone,
        wholeNotes,
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int inputLineNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending 'line break' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza line break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableLineBreak,
        msrSyllable::kSyllableExtendNone,
        0, // whole notes
        msrTupletFactor (),
        this);

  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendLineBreakSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'LineBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllableLineBreak,
 // JMI  nextMeasureNumber,
        msrSyllable::kSyllableExtendNone,
        0, // whole notes
        msrTupletFactor (),
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

S_msrSyllable msrStanza::appendPageBreakSyllableToStanza (
  int    inputLineNumber,
  string nextMeasureNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending a 'PageBreak' syllable" <<
      " to stanza " << getStanzaName () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;
  
  // create stanza break syllable
  S_msrSyllable
    syllable =
      msrSyllable::create (
        inputLineNumber,
        msrSyllable::kSyllablePageBreak,
 // JMI  nextMeasureNumber,
        msrSyllable::kSyllableExtendNone,
        0, // whole notes
        msrTupletFactor (),
        this);
        
  // append syllable to this stanza
  appendSyllableToStanza (syllable);

  gIndenter--;
  
  // and return it
  return syllable;
}

void msrStanza::padUpToMeasureLengthInStanza (
  int      inputLineNumber,
  rational measureLength)
{
  // JMI ???
}

void msrStanza::appendPaddingNoteToStanza (
  int inputLineNumber,
  int divisions,
  int divisionsPerQuarterNote)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceLyrics || gGeneralOptions->fTraceMeasures) {
    gLogIOstream <<
      "Apending padding note of " << divisions <<
      " divisions in stanza \"" <<
      fStanzaName <<
      "\" in voice \"" <<
      fStanzaVoiceUplink->getVoiceName () <<
      "\", line " << inputLineNumber <<
      endl;
  }
#endif

  gIndenter++;

  // JMI TO DO ???
  
  gIndenter--;
}

void msrStanza::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStanza::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStanza::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStanza>*
    p =
      dynamic_cast<visitor<S_msrStanza>*> (v)) {
        S_msrStanza elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStanza::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStanza::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStanza::browseData ()" <<
      endl;
  }

  gIndenter++;
  
  // browse the syllables
  int n = fSyllables.size ();
  for (int i = 0; i < n; i++) {
    // browse the syllable
    msrBrowser<msrSyllable> browser (v);
    browser.browse (*fSyllables [i]);
  } // for

  gIndenter--;

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrStanza::browseData ()" <<
      endl;
  }
}

void msrStanza::print (ostream& os)
{
  os <<
    "Stanza " << getStanzaName () <<
    " (number \"" <<
    fStanzaNumber <<
    "\", " <<
    fSyllables.size () << " syllables)" <<
    endl;
    
  gIndenter++;

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

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrStanza& elt)
{
  elt->print (os);
  return os;
}


}
