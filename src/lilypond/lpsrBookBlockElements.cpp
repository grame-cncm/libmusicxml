/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrBookBlockElements.h"

#include "version.h"

#include "generalOptions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrBookBlockElement lpsrBookBlockElement::create (
  int inputLineNumber)
{
  lpsrBookBlockElement* o = new lpsrBookBlockElement (
    inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrBookBlockElement::lpsrBookBlockElement (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  // create the score command parallel music
  fBookBlockElementParallelMusicBlock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);

  // create the score command layout
  fBookBlockElementLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the score command midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;

  fBookBlockElementMidi =
    msrMidi::create (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
}

lpsrBookBlockElement::~lpsrBookBlockElement ()
{}

void lpsrBookBlockElement::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookBlockElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_lpsrBookBlockElement>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlockElement>*> (v)) {
        S_lpsrBookBlockElement elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBookBlockElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrBookBlockElement::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookBlockElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrBookBlockElement>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlockElement>*> (v)) {
        S_lpsrBookBlockElement elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBookBlockElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrBookBlockElement::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookBlockElement::browseData ()" <<
      endl;
  }

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

/* JMI
  for (
    vector<S_msrElement>::const_iterator i = fBlockElements.begin ();
    i != fBlockElements.end ();
    i++) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/
  {
    // browse the score command layout
    msrBrowser<lpsrLayout> browser (v);
    browser.browse (*fBookBlockElementLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);
    browser.browse (*fBookBlockElementMidi);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrBookBlockElement::browseData ()" <<
      endl;
  }
}

void lpsrBookBlockElement::print (ostream& os)
{
  os << "Block" << endl << endl;

  gIndenter++;

  os <<
    fBookBlockElementParallelMusicBlock <<
    endl;

  os <<
    fBookBlockElementLayout <<
    endl;

  os <<
    fBookBlockElementMidi <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrBookBlockElement& scr)
{
  scr->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrScoreBlock lpsrScoreBlock::create (
  int inputLineNumber)
{
  lpsrScoreBlock* o = new lpsrScoreBlock (
    inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrScoreBlock::lpsrScoreBlock (
  int inputLineNumber)
    : lpsrBookBlockElement (inputLineNumber)
{
  // create the score block parallel music block
  fBookBlockElementParallelMusicBlock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);

  // create the score block layout
  fBookBlockElementLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the score block midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;

  fBookBlockElementMidi =
    msrMidi::create (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
}

lpsrScoreBlock::~lpsrScoreBlock ()
{}

void lpsrScoreBlock::appendPartGroupBlockToScoreBlock (
  S_lpsrPartGroupBlock partGroupBlock)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName () <<
       " to LPSR score block" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
}

/* JMI
void lpsrScoreBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrScoreBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrScoreBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::acceptIn ()" <<
      endl;
  }

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScoreBlock::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrScoreBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrScoreBlock>*
    p =
      dynamic_cast<visitor<S_lpsrScoreBlock>*> (v)) {
        S_lpsrScoreBlock elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrScoreBlock::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrScoreBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrScoreBlock::browseData ()" <<
      endl;
  }

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

/* JMI
  for (
    vector<S_msrElement>::const_iterator i = fBlockElements.begin ();
    i != fBlockElements.end ();
    i++) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/
  {
    // browse the score command layout
    msrBrowser<lpsrLayout> browser (v);
    browser.browse (*fBookBlockElementLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);
    browser.browse (*fBookBlockElementMidi);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrScoreBlock::browseData ()" <<
      endl;
  }
}

void lpsrScoreBlock::print (ostream& os)
{
  os << "Block" << endl << endl;

  gIndenter++;

  os <<
    fBookBlockElementParallelMusicBlock <<
    endl;

  os <<
    fBookBlockElementLayout <<
    endl;

  os <<
    fBookBlockElementMidi <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrScoreBlock& scr)
{
  scr->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrBookPartBlock lpsrBookPartBlock::create (
  int inputLineNumber)
{
  lpsrBookPartBlock* o = new lpsrBookPartBlock (
    inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrBookPartBlock::lpsrBookPartBlock (
  int            inputLineNumber)
    : lpsrBookBlockElement (inputLineNumber)
{
  // create the score command parallel music
  fBookBlockElementParallelMusicBlock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);

  // create the score command layout
  fBookBlockElementLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the score command midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;

  fBookBlockElementMidi =
    msrMidi::create (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
}

lpsrBookPartBlock::~lpsrBookPartBlock ()
{}

void lpsrBookPartBlock::appendPartGroupBlockToBookPartBlock (
  S_lpsrPartGroupBlock partGroupBlock)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName () <<
       " to LPSR book part block" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
}

/* JMI
void lpsrBookPartBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrBookPartBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrBookPartBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookPartBlock::acceptIn ()" <<
      endl;
  }

  if (visitor<S_lpsrBookPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookPartBlock>*> (v)) {
        S_lpsrBookPartBlock elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBookPartBlock::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrBookPartBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookPartBlock::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrBookPartBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookPartBlock>*> (v)) {
        S_lpsrBookPartBlock elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBookPartBlock::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrBookPartBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookPartBlock::browseData ()" <<
      endl;
  }

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBookBlockElementParallelMusicBlock);
  }

/* JMI
  for (
    vector<S_msrElement>::const_iterator i = fBlockElements.begin ();
    i != fBlockElements.end ();
    i++) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/
  {
    // browse the score command layout
    msrBrowser<lpsrLayout> browser (v);
    browser.browse (*fBookBlockElementLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);
    browser.browse (*fBookBlockElementMidi);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrBookPartBlock::browseData ()" <<
      endl;
  }
}

void lpsrBookPartBlock::print (ostream& os)
{
  os << "Block" << endl << endl;

  gIndenter++;

  os <<
    fBookBlockElementParallelMusicBlock <<
    endl;

  os <<
    fBookBlockElementLayout <<
    endl;

  os <<
    fBookBlockElementMidi <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrBookPartBlock& scr)
{
  scr->print (os);
  return os;
}

//______________________________________________________________________________
S_lpsrBookBlock lpsrBookBlock::create (
  int inputLineNumber)
{
  lpsrBookBlock* o = new lpsrBookBlock (
    inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrBookBlock::lpsrBookBlock (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  /* JMI
  // create the score block element parallel music
  fBookBlockElementParallelMusicBlock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);
*/

  // create the score block element header
  fBookBlockHeader =
    lpsrHeader::create (
      inputLineNumber);

  // create the score block element layout
  fBookBlockLayout =
    lpsrLayout::create (
      inputLineNumber);
}

lpsrBookBlock::~lpsrBookBlock ()
{}

/* JMI
void lpsrBookBlock::appendPartGroupBlockToBlock (
  S_lpsrPartGroupBlock partGroupBlock)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroups) {
    gLogIOstream <<
      "Appending part group block " <<
       partGroupBlock-> getPartGroup ()-> getPartGroupCombinedName () <<
       " to LPSR score" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
}
*/

void lpsrBookBlock::appendBookBlockElementToBookBlock (
  S_lpsrBookBlockElement bookBlockElement)
{
#ifdef TRACE_OPTIONS
  if (gLpsrOptions->fTraceLpsrBlocks) {
    gLogIOstream <<
      "Appending book block element  " <<
       bookBlockElement-> asString () <<
       " to LPSR book block" <<
       endl;
  }
#endif

  fBookBlockElementsList.push_back (
      bookBlockElement);
}

/* JMI
void lpsrBookBlock::appendVoiceUseToParallelMusicBLock (
  S_lpsrUseVoiceCommand voiceUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending the use of voice \"" <<
       voiceUse-> getVoice ()-> getVoiceName () <<
       "\" to LPSR score" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (voiceUse);

//     JMI             fBlockElements.push_back(voiceUse);
}

void lpsrBookBlock::appendLyricsUseToParallelMusicBLock (
  S_lpsrNewLyricsBlock lyricsUse)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceLyrics) {
    gLogIOstream <<
      "Appending the use of stanza " <<
       lyricsUse-> getStanza ()-> getStanzaName () <<
       " to LPSR score" <<
       endl;
  }
#endif

  fBookBlockElementParallelMusicBlock->
    addElementToParallelMusicBLock (lyricsUse);
}
*/

void lpsrBookBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookBlock::acceptIn ()" <<
      endl;
  }

  if (visitor<S_lpsrBookBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlock>*> (v)) {
        S_lpsrBookBlock elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBookBlock::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrBookBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookBlock::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrBookBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBookBlock>*> (v)) {
        S_lpsrBookBlock elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBookBlock::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrBookBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBookBlock::browseData ()" <<
      endl;
  }

  if (fBookBlockHeader) {
    // browse the book block header
    msrBrowser<lpsrHeader> browser (v);
    browser.browse (*fBookBlockHeader);
  }

/* JMI
  for (
    vector<S_msrElement>::const_iterator i = fBlockElements.begin ();
    i != fBlockElements.end ();
    i++) {
    // browse the element
 //   msrBrowser<msrElement> browser (v);
 //   browser.browse (*(*i));
  } // for
*/
  if (fBookBlockLayout) {
    // browse the book block layout
    msrBrowser<lpsrLayout> browser (v);
    browser.browse (*fBookBlockLayout);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrBookBlock::browseData ()" <<
      endl;
  }
}

void lpsrBookBlock::print (ostream& os)
{
  os << "Block" << endl << endl;

  gIndenter++;

  if (fBookBlockHeader) {
    os <<
      fBookBlockHeader <<
      endl;
  }

  if (fBookBlockLayout) {
    os <<
      fBookBlockLayout <<
      endl;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrBookBlock& scr)
{
  scr->print (os);
  return os;
}


}
