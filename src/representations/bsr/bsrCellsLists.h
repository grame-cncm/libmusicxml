/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrCellsLists___
#define ___bsrCellsLists___

#include "smartpointer.h"
#include "basevisitor.h"

#include "bsrBasicTypes.h"
#include "bsrCellsLists.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
class bsrCellsList;
typedef SMARTP<bsrCellsList> S_bsrCellsList;

//______________________________________________________________________________
class EXP bsrCellsList : public smartable
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrCellsList> create (
      int         inputLineNumber);

    static SMARTP<bsrCellsList> create (
      int         inputLineNumber,
      bsrCellKind cellKind1);

    static SMARTP<bsrCellsList> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2);

    static SMARTP<bsrCellsList> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3);

    static SMARTP<bsrCellsList> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4);

    static SMARTP<bsrCellsList> create (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4,
      bsrCellKind cellKind5);

    static SMARTP<bsrCellsList> create (
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

    bsrCellsList (
      int         inputLineNumber);

    bsrCellsList (
      int         inputLineNumber,
      bsrCellKind cellKind1);

    bsrCellsList (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2);

    bsrCellsList (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3);

    bsrCellsList (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4);

    bsrCellsList (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4,
      bsrCellKind cellKind5);

    bsrCellsList (
      int         inputLineNumber,
      bsrCellKind cellKind1,
      bsrCellKind cellKind2,
      bsrCellKind cellKind3,
      bsrCellKind cellKind4,
      bsrCellKind cellKind5,
      bsrCellKind cellKind6);

    virtual ~bsrCellsList ();

  public:

    // set and get
    // ------------------------------------------------------

    const list<bsrCellKind>&
                          getCellsListElements () const
                              { return fCellsListElements; }

  public:

    // public services
    // ------------------------------------------------------

    void                  appendCellKindToCellsList (bsrCellKind cellKind)
                              { fCellsListElements.push_back (cellKind); }

    void                  appendCellsListToCellsList (
                            S_bsrCellsList otherCellsList);

    void                  prependCellsListToCellsList (
                            S_bsrCellsList otherCellsList);

    int                   fetchCellsNumber () const
                              { return fCellsListElements.size (); }

// JMI    void                  generateBrailleCode (ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    virtual string        asShortString () const;

    virtual void          print (ostream& os) const;

  protected:

    // protected fields
    // ------------------------------------------------------

    list<bsrCellKind>     fCellsListElements;
};
typedef SMARTP<bsrCellsList> S_bsrCellsList;
EXP ostream& operator<< (ostream& os, const S_bsrCellsList& elt);


} // namespace MusicXML2


#endif
