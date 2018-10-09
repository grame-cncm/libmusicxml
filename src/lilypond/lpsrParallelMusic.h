/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lpsrParallelMusic___
#define ___lpsrParallelMusic___
/*
#include <sstream>
#include <vector>


#include "exports.h"
#include "typedefs.h"

#include "msrBasicTypes.h"

#include "msr.h"

#include "lpsrVarValAssocs.h"

#include "lpsrScheme.h"
*/

#include <string>
#include <list>

#include "lpsrPartGroups.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class lpsrParallelMusicBLock : public lpsrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum lpsrElementsSeparatorKind {
      kEndOfLine, kSpace};

    static string elementsSeparatorKindAsString (
      lpsrElementsSeparatorKind elementsSeparatorKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<lpsrParallelMusicBLock> create (
      int                       inputLineNumber,
      lpsrElementsSeparatorKind elementsSeparatorKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lpsrParallelMusicBLock (
      int                       inputLineNumber,
      lpsrElementsSeparatorKind elementsSeparatorKind);
      
    virtual ~lpsrParallelMusicBLock ();
    
  public:

    // set and get
    // ------------------------------------------------------

    const list<S_lpsrPartGroupBlock>&
                          getParallelMusicBLockPartGroupBlocks () const
                              { return fParallelMusicBLockPartGroupBlocks; }

    // services
    // ------------------------------------------------------

    void                  appendPartGroupBlockToParallelMusicBLock ( // JMI
                            S_lpsrPartGroupBlock partGroupBlock)
                              {
                                fParallelMusicBLockPartGroupBlocks.push_back (
                                  partGroupBlock);
                              }
                    
/* JMI
    S_lpsrPartGroupBlock  getLastPartGroupBlockOfParallelMusicBLock ()
                              {
                                return
                                  fParallelMusicBLockPartGroupBlocks.back ();
                              }
                    
    void                  removeLastPartGroupBlockOfParallelMusicBLock () // JMI
                              {
                                fParallelMusicBLockPartGroupBlocks.pop_back ();
                              }
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

    list<S_lpsrPartGroupBlock>
                          fParallelMusicBLockPartGroupBlocks;
    
    lpsrElementsSeparatorKind
                          fElementsSeparatorKind;
};
typedef SMARTP<lpsrParallelMusicBLock> S_lpsrParallelMusicBLock;
EXP ostream& operator<< (ostream& os, const S_lpsrParallelMusicBLock& elt);


}


#endif
