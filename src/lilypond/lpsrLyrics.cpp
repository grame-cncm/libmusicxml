/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrLyrics.h"

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_lpsrNewLyricsBlock lpsrNewLyricsBlock::create (
  int         inputLineNumber,
  S_msrStanza stanza,
  S_msrVoice  voice)
{
  lpsrNewLyricsBlock* o =
    new lpsrNewLyricsBlock (
      inputLineNumber,
      stanza, voice);
  assert(o!=0);
  return o;
}

lpsrNewLyricsBlock::lpsrNewLyricsBlock (
  int         inputLineNumber,
  S_msrStanza stanza,
  S_msrVoice  voice)
    : lpsrElement (inputLineNumber)
{
  fStanza = stanza;
  fVoice  = voice;
}

lpsrNewLyricsBlock::~lpsrNewLyricsBlock ()
{}

void lpsrNewLyricsBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewLyricsBlock::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrNewLyricsBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewLyricsBlock>*> (v)) {
        S_lpsrNewLyricsBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewLyricsBlock::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrNewLyricsBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrNewLyricsBlock::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrNewLyricsBlock>*
    p =
      dynamic_cast<visitor<S_lpsrNewLyricsBlock>*> (v)) {
        S_lpsrNewLyricsBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrNewLyricsBlock::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrNewLyricsBlock::browseData (basevisitor* v)
{}

void lpsrNewLyricsBlock::print (ostream& os)
{
  os <<
    "NewLyricsBlock" << " " <<
    fStanza->getStanzaName () << " " <<
    fVoice->getVoiceName () <<
    endl;
}

ostream& operator<< (ostream& os, const S_lpsrNewLyricsBlock& nlc)
{
  nlc->print (os);
  return os;
}


}
