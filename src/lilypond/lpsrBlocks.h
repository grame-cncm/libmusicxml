/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrBlocks___
#define ___lpsrBlocks___

/*
#include <sstream>
#include <vector>
#include <string>
#include <list>


#include "exports.h"
#include "typedefs.h"

#include "msrBasicTypes.h"


#include "lpsrVarValAssocs.h"

#include "lpsrScheme.h"
*/

#include "lpsrComments.h"

#include "lpsrScheme.h"

#include "lpsrHeaders.h"
#include "lpsrPapers.h"
#include "lpsrLayouts.h"

#include "lpsrParallelMusic.h"
#include "lpsrVarValAssocs.h"

//#include "msrBlocks.h"

#include "msrMidi.h"


namespace MusicXML2
{

//______________________________________________________________________________
class lpsrBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBlock> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBlock (
      int inputLineNumber);

    virtual ~lpsrBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrLayout          getBlockLayout () const
                            { return fBlockLayout; }

    S_msrMidi             getBlockMidi () const
                              { return fBlockMidi; }

    // services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToBlock (
                            S_lpsrPartGroupBlock partGroupBlock);

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

    S_lpsrParallelMusicBLock
                          fBlockParallelMusicBLock;

    S_lpsrLayout          fBlockLayout;

    S_msrMidi             fBlockMidi;
};
typedef SMARTP<lpsrBlock> S_lpsrBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrBlock& elt);

//______________________________________________________________________________
class lpsrScoreBlock : public lpsrBlock
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrScoreBlock> create (
      int            inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrScoreBlock (
      int            inputLineNumber);

    virtual ~lpsrScoreBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrParallelMusicBLock
                          getBlockParallelMusicBLock () const
                              {
                                return
                                  fBlockParallelMusicBLock;
                              }

/*
    const vector<S_msrElement>&
                  getBlockElements () const
                      { return fBlockElements; }
*/
    S_lpsrLayout          getBlockLayout () const
                            { return fBlockLayout; }

    S_msrMidi             getBlockMidi () const
                              { return fBlockMidi; }

    // services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToScoreBlock (
                            S_lpsrPartGroupBlock partGroupBlock);

/* JMI
    void                  appendVoiceUseToParallelMusic (
                            S_lpsrUseVoiceCommand voiceUse);

    void                  appendLyricsUseToParallelMusic (
                            S_lpsrNewLyricsBlock lyricsUse);
                            */

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

    S_lpsrParallelMusicBLock
                          fBlockParallelMusicBLock;

    S_lpsrLayout          fBlockLayout;

    S_msrMidi             fBlockMidi;
};
typedef SMARTP<lpsrScoreBlock> S_lpsrScoreBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrScoreBlock& elt);

//______________________________________________________________________________
class lpsrBookPartBlock : public lpsrBlock
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookPartBlock> create (
      int            inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBookPartBlock (
      int            inputLineNumber);

    virtual ~lpsrBookPartBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrParallelMusicBLock
                          getBlockParallelMusicBLock () const
                              {
                                return
                                  fBlockParallelMusicBLock;
                              }

/*
    const vector<S_msrElement>&
                  getBlockElements () const
                      { return fBlockElements; }
*/
    S_lpsrLayout          getBlockLayout () const
                            { return fBlockLayout; }

    S_msrMidi             getBlockMidi () const
                              { return fBlockMidi; }

    // services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToBookPartBlock (
                            S_lpsrPartGroupBlock partGroupBlock);

/* JMI
    void                  appendVoiceUseToParallelMusic (
                            S_lpsrUseVoiceCommand voiceUse);

    void                  appendLyricsUseToParallelMusic (
                            S_lpsrNewLyricsBlock lyricsUse);
                            */

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

    S_lpsrParallelMusicBLock
                          fBlockParallelMusicBLock;

    S_lpsrLayout          fBlockLayout;

    S_msrMidi             fBlockMidi;
};
typedef SMARTP<lpsrBookPartBlock> S_lpsrBookPartBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrBookPartBlock& elt);

//______________________________________________________________________________
class lpsrBookBlock : public lpsrBlock
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookBlock> create (
      int            inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBookBlock (
      int            inputLineNumber);

    virtual ~lpsrBookBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrParallelMusicBLock
                          getBlockParallelMusicBLock () const
                              {
                                return
                                  fBlockParallelMusicBLock;
                              }

/*
    const vector<S_msrElement>&
                  getBlockElements () const
                      { return fBlockElements; }
*/
    S_lpsrLayout          getBlockLayout () const
                            { return fBlockLayout; }

    S_msrMidi             getBlockMidi () const
                              { return fBlockMidi; }

    // services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToBlock (
                            S_lpsrPartGroupBlock partGroupBlock);

/* JMI
    void                  appendVoiceUseToParallelMusic (
                            S_lpsrUseVoiceCommand voiceUse);

    void                  appendLyricsUseToParallelMusic (
                            S_lpsrNewLyricsBlock lyricsUse);
                            */

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

    S_lpsrParallelMusicBLock
                          fBlockParallelMusicBLock;

    S_lpsrLayout          fBlockLayout;

    S_msrMidi             fBlockMidi;
};
typedef SMARTP<lpsrBookBlock> S_lpsrBookBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrBookBlock& elt);


}


#endif
