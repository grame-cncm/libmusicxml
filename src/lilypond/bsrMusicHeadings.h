/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrMusicHeadings___
#define ___bsrMusicHeadings___

#include "bsrLines.h"

#include "bsrCellsLists.h"

#include "bsrPaginations.h"
#include "bsrNumbers.h"
#include "bsrKeys.h"
#include "bsrTimes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrMusicHeading : public bsrLine
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrMusicHeading> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrMusicHeading (
      int inputLineNumber);
      
    virtual ~bsrMusicHeading ();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
    void                  setMusicHeadingTempo (S_bsrTempo tempo)
                              { fMusicHeadingTempo = tempo; }

    S_bsrTempo            getMusicHeadingTempo () const
                              { return fMusicHeadingTempo; }

    S_bsrKey              getMusicHeadingKey () const
                              { return fMusicHeadingKey; }

    void                  setMusicHeadingKey (S_bsrKey key)
                              { fMusicHeadingKey = key; }

    S_bsrTime             getMusicHeadingTime () const
                              { return fMusicHeadingTime; }

    void                  setMusicHeadingTime (S_bsrTime time)
                              { fMusicHeadingTime = time; }

  public:

    // public services
    // ------------------------------------------------------

    S_bsrCellsList        fetchCellsList () const
                              { return buildCellsList (); }
  
  private:

    // private services
    // ------------------------------------------------------

    S_bsrCellsList        buildCellsList () const;    

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:
  
    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_bsrTempo            fMusicHeadingTempo;
    S_bsrKey              fMusicHeadingKey;
    S_bsrTime             fMusicHeadingTime;
};
typedef SMARTP<bsrMusicHeading> S_bsrMusicHeading;
EXP ostream& operator<< (ostream& os, const S_bsrMusicHeading& elt);


}


#endif
