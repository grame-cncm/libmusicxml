/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrBookBlockElements___
#define ___lpsrBookBlockElements___

#include "lpsrComments.h"

#include "lpsrScheme.h"

#include "lpsrHeaders.h"
#include "lpsrPapers.h"
#include "lpsrLayouts.h"

#include "lpsrParallelMusic.h"

//#include "msrBlocks.h"

#include "msrMidi.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP lpsrBookBlockElement : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookBlockElement> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBookBlockElement (
      int inputLineNumber);

    virtual ~lpsrBookBlockElement ();

  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrHeader          getBookBlockElementHeader () const
                              { return fBookBlockElementHeader; }

    S_lpsrParallelMusicBLock
                          getBlockElementParallelMusicBLock () const
                              {
                                return
                                  fBookBlockElementParallelMusicBlock;
                              }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToBlockElement (
                            S_lpsrPartGroupBlock partGroupBlock);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  protected:

    // protected fields
    // ------------------------------------------------------

    S_lpsrHeader          fBookBlockElementHeader;

    S_lpsrParallelMusicBLock
                          fBookBlockElementParallelMusicBlock;
};
typedef SMARTP<lpsrBookBlockElement> S_lpsrBookBlockElement;
EXP ostream& operator<< (ostream& os, const S_lpsrBookBlockElement& elt);

//______________________________________________________________________________
class EXP lpsrScoreBlock : public lpsrBookBlockElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrScoreBlock> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrScoreBlock (
      int inputLineNumber);

    virtual ~lpsrScoreBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    S_lpsrLayout          getScoreBlockLayout () const
                            { return fScoreBlockLayout; }

    S_msrMidiTempo        getScoreBlockMidiTempo () const
                              { return fScoreBlockMidiTempo; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToScoreBlock (
                            S_lpsrPartGroupBlock partGroupBlock);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_lpsrLayout          fScoreBlockLayout;

    S_msrMidiTempo        fScoreBlockMidiTempo;
};
typedef SMARTP<lpsrScoreBlock> S_lpsrScoreBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrScoreBlock& elt);

//______________________________________________________________________________
class EXP lpsrBookPartBlock : public lpsrBookBlockElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookPartBlock> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBookPartBlock (
      int inputLineNumber);

    virtual ~lpsrBookPartBlock ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToBookPartBlock (
                            S_lpsrPartGroupBlock partGroupBlock);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<lpsrBookPartBlock> S_lpsrBookPartBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrBookPartBlock& elt);

//______________________________________________________________________________
class EXP lpsrBookBlock : public lpsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrBookBlock> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrBookBlock (
      int inputLineNumber);

    virtual ~lpsrBookBlock ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setBookBlockHeader (
                            S_lpsrHeader bookBlockHeader)
                              { fBookBlockHeader = bookBlockHeader; }

    S_lpsrHeader          getBookBlockHeader () const
                              { return fBookBlockHeader; }

    void                  setBookBlockPaper (
                            S_lpsrPaper bookBlockPaper)
                              { fBookBlockPaper = bookBlockPaper; }

    S_lpsrPaper           getBookBlockPaper () const
                              { return fBookBlockPaper; }

    const list<S_lpsrBookBlockElement>&
                          getBookBlockElementsList () const
                              { return fBookBlockElementsList; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendLpsrScoreBlockToBookBlockElementsList (
                            S_lpsrScoreBlock scoreBlock);

    void                  appendLpsrBookPartBlockToBookBlockElementsList (
                            S_lpsrBookPartBlock bookPartBlock);

  protected:

    // protected services
    // ------------------------------------------------------

    void                  appendBookBlockElementToBookBlockElementsList (
                            S_lpsrBookBlockElement bookBlockElement);

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    S_lpsrHeader          fBookBlockHeader;

    S_lpsrPaper           fBookBlockPaper;

    list<S_lpsrBookBlockElement>
                          fBookBlockElementsList;
};
typedef SMARTP<lpsrBookBlock> S_lpsrBookBlock;
EXP ostream& operator<< (ostream& os, const S_lpsrBookBlock& elt);


}


#endif
