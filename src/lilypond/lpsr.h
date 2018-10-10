/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsr___
#define ___lpsr___

#include <sstream>
#include <vector>
#include <string>
#include <list>


#include "exports.h"
#include "typedefs.h"

#include "msrBasicTypes.h"

#include "msr.h"

#include "lpsrComments.h"

#include "lpsrVarValAssocs.h"

#include "lpsrScheme.h"

#include "lpsrHeaders.h"
#include "lpsrPapers.h"
#include "lpsrLayouts.h"

#include "lpsrBarNumbers.h"

#include "lpsrContexts.h"
#include "lpsrVariables.h"

#include "lpsrVoices.h"
#include "lpsrLyrics.h"

#include "lpsrStaves.h"
#include "lpsrParts.h"
#include "lpsrPartGroups.h"
#include "lpsrScores.h"


namespace MusicXML2 
{

/* JMI ???
//______________________________________________________________________________
class lpsrUseLyricsCommand : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrUseLyricsCommand> create (
      int          inputLineNumber,
      S_msrStanza& stanza);

  public:

    // set and get
    // ------------------------------------------------------

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

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrUseLyricsCommand (
      int            inputLineNumber,
      S_msrStanza&   stanza);
      
    virtual ~lpsrUseLyricsCommand ();
  
  private:
  
    // fields
    // ------------------------------------------------------

    S_msrStanza           fStanza;
};
typedef SMARTP<lpsrUseLyricsCommand> S_lpsrUseLyricsCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrUseLyricsCommand& elt);
*/

//______________________________________________________________________________
/* JMI
class lpsrMelismaCommand : public lpsrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum lpsrMelismaKind {
      kMelismaStart, kMelismaEnd};

    static string melismaKindAsString (
      lpsrMelismaKind melismaKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrMelismaCommand> create (
      int             inputLineNumber,
      lpsrMelismaKind melismaKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrMelismaCommand (
      int             inputLineNumber,
      lpsrMelismaKind melismaKind);
      
    virtual ~lpsrMelismaCommand ();
  
  public:

    // set and get
    // ------------------------------------------------------

    lpsrMelismaKind       getMelismaKind () const
                              { return fMelismaKind; }

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

    lpsrMelismaKind fMelismaKind;

};
typedef SMARTP<lpsrMelismaCommand> S_lpsrMelismaCommand;
EXP ostream& operator<< (ostream& os, const S_lpsrMelismaCommand& elt);
*/

//______________________________________________________________________________
EXP void initializeLPSR ();


}


#endif
