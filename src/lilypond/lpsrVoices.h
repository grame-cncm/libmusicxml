/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrVoices___
#define ___lpsrVoices___

#include <iostream>

#include "lpsrElements.h"

#include "msr.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class lpsrUseVoiceCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrUseVoiceCommand> create (
      int        inputLineNumber,
      S_msrVoice voice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrUseVoiceCommand (
      int        inputLineNumber,
      S_msrVoice voice);
      
    virtual ~lpsrUseVoiceCommand ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getVoice () const { return fVoice; }

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

    S_msrVoice            fVoice;
};
typedef SMARTP<lpsrUseVoiceCommand> S_lpsrUseVoiceCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrUseVoiceCommand& elt);


}


#endif
