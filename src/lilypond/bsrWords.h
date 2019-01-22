/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrWords___
#define ___bsrWords___

#include "bsrLineElements.h"

#include "bsrCellsLists.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// brailling characters and words
S_bsrCellsList brailleCharacter (
  int  inputLineNumber,
  char ch);

S_bsrCellsList brailleWord (
  int    inputLineNumber,
  string str);

//______________________________________________________________________________
class bsrWords : public bsrLineElement
{
  public:
          
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrWords> create (
      int    inputLineNumber,
      string wordContents);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrWords (
      int    inputLineNumber,
      string wordContents);
      
    virtual ~bsrWords ();
  
  public:

    // set and get
    // ------------------------------------------------------
                  
    string                getWordContents () const
                              { return fWordContents; }

    // services
    // ------------------------------------------------------
                  
    S_bsrCellsList        fetchCellsList () const
                              { return fWordCellsList; }

    int                   fetchCellsNumber () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asWord () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    string                fWordContents;

    S_bsrCellsList        fWordCellsList;
};
typedef SMARTP<bsrWords> S_bsrWords;
EXP ostream& operator<< (ostream& os, const S_bsrWords& elt);


} // namespace MusicXML2


#endif
