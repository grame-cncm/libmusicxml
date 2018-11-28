/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrSpaces___
#define ___bsrSpaces___

#include "bsrBrailleElements.h"

#include "bsrCellsLists.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
const bsrCellKind
  kCellSpace = kDotsNone;

//______________________________________________________________________________
class bsrSpaces : public bsrBrailleElement
{
  public:
          
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrSpaces> create (
      int inputLineNumber,
      int numberOfSpaces);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrSpaces (
      int inputLineNumber,
      int numberOfSpaces);
      
    virtual ~bsrSpaces ();
  
  public:

    // set and get
    // ------------------------------------------------------
                  
    int                   getNumberOfSpaces () const
                              { return fNumberOfSpaces; }

    S_bsrCellsList        getSpacesCellsList () const
                              { return fSpacesCellsList; }

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

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fNumberOfSpaces;

    S_bsrCellsList        fSpacesCellsList;
};
typedef SMARTP<bsrSpaces> S_bsrSpaces;
EXP ostream& operator<< (ostream& os, const S_bsrSpaces& elt);


} // namespace MusicXML2


#endif
