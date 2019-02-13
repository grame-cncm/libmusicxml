/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrLyrics___
#define ___lpsrLyrics___

#include "lpsrElements.h"

#include "msr.h"

#include "lpsrVariables.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lpsrNewLyricsBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrNewLyricsBlock> create (
      int         inputLineNumber,
      S_msrStanza stanza,
      S_msrVoice  voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrNewLyricsBlock (
      int         inputLineNumber,
      S_msrStanza stanza,
      S_msrVoice  voice);
      
    virtual ~lpsrNewLyricsBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStanza           getStanza () const { return fStanza; }
    S_msrVoice            getVoice  () const { return fVoice; }
    
    // services
    // ------------------------------------------------------

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    // fields
    // ------------------------------------------------------

    S_msrStanza           fStanza;
    S_msrVoice            fVoice;
};
typedef SMARTP<lpsrNewLyricsBlock> S_lpsrNewLyricsBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrNewLyricsBlock& elt);


}


#endif
