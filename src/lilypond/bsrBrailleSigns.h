/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrBrailleSigns___
#define ___bsrBrailleSigns___

#include "bsrElements.h"

#include "bsrBasicTypes.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class bsrBrailleSign;
typedef SMARTP<bsrBrailleSign> S_bsrBrailleSign;

class bsrBrailleSign : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrBrailleSign> create (
      int         inputLineNumber);

    static SMARTP<bsrBrailleSign> create (
      int         inputLineNumber,
      bsrCellKind cellKind1);

    static SMARTP<bsrBrailleSign> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2);

    static SMARTP<bsrBrailleSign> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3);

    static SMARTP<bsrBrailleSign> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4);

    static SMARTP<bsrBrailleSign> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4,
      bsrCellKind cellKind5);

    static SMARTP<bsrBrailleSign> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4,
      bsrCellKind cellKind5,
      bsrCellKind cellKind6);

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    bsrBrailleSign (
      int         inputLineNumber);

    bsrBrailleSign (
      int         inputLineNumber,
      bsrCellKind cellKind1);

    bsrBrailleSign (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2);

    bsrBrailleSign (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3);

    bsrBrailleSign (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4);

    bsrBrailleSign (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4,
      bsrCellKind cellKind5);

    bsrBrailleSign (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4,
      bsrCellKind cellKind5,
      bsrCellKind cellKind6);

    virtual ~bsrBrailleSign ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<bsrCellKind>&
                          getCellsList () const
                              { return fCellsList; }

    bool                  getASpaceIsNeededBefore () const
                              { return fASpaceIsNeededBefore; }

    bool                  getASpaceIsNeededAfter () const
                              { return fASpaceIsNeededAfter; }

    // services
    // ------------------------------------------------------

    void                  appendCellKindToBrailleSign (bsrCellKind cell)
                              { fCellsList.push_back (cell); }

    void                  appendBrailleSignToBrailleSign (
                            S_bsrBrailleSign otherBrailleSign);
    void                  prependBrailleSignToBrailleSign (
                            S_bsrBrailleSign otherBrailleSign);

    int                   fetchCellsNumber () const
                              { return fCellsList.size (); }

    void                  generateBrailleCode (ostream& os);

  public:
  
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual std::string   asString () const;
    
    virtual std::string   asShortString () const;

    virtual void          print (std::ostream& os);

  protected:
  
    // fields
    // ------------------------------------------------------

    list<bsrCellKind>     fCellsList;
    
    bool                  fASpaceIsNeededBefore;
    bool                  fASpaceIsNeededAfter;
};
typedef SMARTP<bsrBrailleSign> S_bsrBrailleSign;
EXP std::ostream& operator<< (std::ostream& os, const S_bsrBrailleSign& elt);


} // namespace MusicXML2


#endif
