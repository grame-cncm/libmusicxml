/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrBlocks.h"

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
S_lpsrBlock lpsrBlock::create (
  int inputLineNumber)
{
  lpsrBlock* o = new lpsrBlock (
    inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrBlock::lpsrBlock (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  // create the score command parallel music
  fBlockParallelMusicBLock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);

  // create the score command layout
  fBlockLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the score command midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;

  fBlockMidi =
    msrMidi::create (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
}

lpsrBlock::~lpsrBlock ()
{}

void lpsrBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBlock::acceptIn ()" <<
      endl;
  }

  if (visitor<S_lpsrBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBlock>*> (v)) {
        S_lpsrBlock elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBlock::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBlock::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrBlock>*
    p =
      dynamic_cast<visitor<S_lpsrBlock>*> (v)) {
        S_lpsrBlock elem = this;

        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrBlock::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrBlock::browseData ()" <<
      endl;
  }

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBlockParallelMusicBLock);
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
    browser.browse (*fBlockLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);
    browser.browse (*fBlockMidi);
  }

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrBlock::browseData ()" <<
      endl;
  }
}

void lpsrBlock::print (ostream& os)
{
  os << "Block" << endl << endl;

  gIndenter++;

  os <<
    fBlockParallelMusicBLock <<
    endl;

  os <<
    fBlockLayout <<
    endl;

  os <<
    fBlockMidi <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrBlock& scr)
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
    : lpsrBlock (inputLineNumber)
{
  // create the score block parallel music block
  fBlockParallelMusicBLock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);

  // create the score block layout
  fBlockLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the score block midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;

  fBlockMidi =
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

  fBlockParallelMusicBLock->
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

  fBlockParallelMusicBLock->
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

  fBlockParallelMusicBLock->
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
    browser.browse (*fBlockParallelMusicBLock);
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
    browser.browse (*fBlockLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);
    browser.browse (*fBlockMidi);
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
    fBlockParallelMusicBLock <<
    endl;

  os <<
    fBlockLayout <<
    endl;

  os <<
    fBlockMidi <<
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
    : lpsrBlock (inputLineNumber)
{
  // create the score command parallel music
  fBlockParallelMusicBLock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);

  // create the score command layout
  fBlockLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the score command midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;

  fBlockMidi =
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

  fBlockParallelMusicBLock->
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

  fBlockParallelMusicBLock->
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

  fBlockParallelMusicBLock->
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
    browser.browse (*fBlockParallelMusicBLock);
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
    browser.browse (*fBlockLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);
    browser.browse (*fBlockMidi);
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
    fBlockParallelMusicBLock <<
    endl;

  os <<
    fBlockLayout <<
    endl;

  os <<
    fBlockMidi <<
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
    : lpsrBlock (inputLineNumber)
{
  // create the score command parallel music
  fBlockParallelMusicBLock =
    lpsrParallelMusicBLock::create (
      inputLineNumber,
      lpsrParallelMusicBLock::kEndOfLine);

  // create the score command layout
  fBlockLayout =
    lpsrLayout::create (
      inputLineNumber);

  // create the score command midi
  string midiTempoDuration  = gLilypondOptions->fMidiTempo.first;
  int    midiTempoPerSecond = gLilypondOptions->fMidiTempo.second;

  fBlockMidi =
    msrMidi::create (
      inputLineNumber,
      midiTempoDuration,
      midiTempoPerSecond);
}

lpsrBookBlock::~lpsrBookBlock ()
{}

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

  fBlockParallelMusicBLock->
    appendPartGroupBlockToParallelMusicBLock (
      partGroupBlock);

//               fBlockElements.push_back(partGroupBlock);
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

  fBlockParallelMusicBLock->
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

  fBlockParallelMusicBLock->
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

  {
    // browse the score command parallel music
    msrBrowser<lpsrParallelMusicBLock> browser (v);
    browser.browse (*fBlockParallelMusicBLock);
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
    browser.browse (*fBlockLayout);
  }

  {
    // browse the score command midi
    msrBrowser<msrMidi> browser (v);
    browser.browse (*fBlockMidi);
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

  os <<
    fBlockParallelMusicBLock <<
    endl;

  os <<
    fBlockLayout <<
    endl;

  os <<
    fBlockMidi <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrBookBlock& scr)
{
  scr->print (os);
  return os;
}


}
