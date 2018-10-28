/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrScores___
#define ___bsrScores___

#include "bsrElements.h"

/*


#include "exports.h"
#include "typedefs.h"

#include "msrBasicTypes.h"


#include "bsrVarValAssocs.h"

#include "bsrScheme.h"


#include "bsrComments.h"

#include "bsrScheme.h"

#include "bsrHeaders.h"
#include "bsrPapers.h"
#include "bsrLayouts.h"

#include "bsrParallelMusic.h"
#include "bsrVarValAssocs.h"


#include "msrMidi.h"
*/

#include <sstream>
//#include <vector>
#include <string>
#include <list>

#include "msrScores.h"

#include "bsrOptions.h"


namespace MusicXML2 
{

/*
//______________________________________________________________________________
class bsrScoreBlock : public bsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<bsrScoreBlock> create (
      int            inputLineNumber);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrScoreBlock (
      int            inputLineNumber);
      
    virtual ~bsrScoreBlock ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_bsrParallelMusicBLock
                          getScoreBlockParallelMusicBLock () const
                              {
                                return
                                  fScoreBlockParallelMusicBLock;
                              }

/ *
    const vector<S_msrElement>&
                  getScoreBlockElements () const
                      { return fScoreBlockElements; }
* /
    S_bsrLayout          getScoreBlockLayout () const
                            { return fScoreBlockLayout; }

    S_msrMidi             getScoreBlockMidi () const
                              { return fScoreBlockMidi; }

    // services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToScoreBlock (
                            S_bsrPartGroupBlock partGroupBlock);

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

    S_bsrParallelMusicBLock
                          fScoreBlockParallelMusicBLock;
    
    S_bsrLayout          fScoreBlockLayout;
    
    S_msrMidi             fScoreBlockMidi;    
};
typedef SMARTP<bsrScoreBlock> S_bsrScoreBlock;
EXP ostream& operator<< (ostream& os, const S_bsrScoreBlock& elt);
*/

//______________________________________________________________________________
class bsrScore : public bsrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<bsrScore> create (
      int            inputLineNumber,
      S_msrScore     mScore);
     
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrScore (
      int            inputLineNumber,
      S_msrScore     mScore);
      
    virtual ~bsrScore ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrScore            getMsrScore () const
                              { return fMsrScore; }

  public:
  
    // public services
    // ------------------------------------------------------

  private:
  
    // private services
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

    // MSR data
    S_msrScore            fMsrScore;

/*
    // general information
    S_bsrVarValAssoc
                          fLilypondVersion;
    
    S_bsrComment         fInputSourceNameComment;
    S_bsrComment         fTranslationDateComment;
    S_bsrComment         fCommandLineLongOptionsComment;
    S_bsrComment         fCommandLineShortOptionsComment;
    
    S_bsrSchemeVariable
                          fGlobalStaffSizeAssoc;
    
    S_bsrHeader          fHeader;
    S_bsrPaper           fPaper;
    S_bsrLayout          fScoreLayout;

    // to keep the original line breaks
    S_bsrVarValAssoc
                          fMyBreakIsBreakAssoc;
    S_bsrVarValAssoc
                          fMyBreakIsEmptyAssoc;

    // to keep the original page breaks
    S_bsrVarValAssoc
                          fMyPageBreakIsPageBreakAssoc;
    S_bsrVarValAssoc
                          fMyPageBreakIsEmptyAssoc;

    // to generate 'global' variable
    S_bsrVarValAssoc
                          fGlobalAssoc;

    // variables, voices and stanzas
    list<S_msrElement>    fScoreElements;

    // score command
    S_bsrScoreBlock      fScoreBlock;

    map<string, S_bsrSchemeFunction>
                          fScoreSchemeFunctionsMap;
                          */
};
typedef SMARTP<bsrScore> S_bsrScore;
EXP ostream& operator<< (ostream& os, const S_bsrScore& elt);


}


#endif
