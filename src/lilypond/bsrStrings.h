/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrStrings___
#define ___bsrStrings___

#include "bsrLineElements.h"

#include "bsrCellsLists.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class bsrString : public bsrLineElement
{
  public:
          
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrString> create (
      int    inputLineNumber,
      string stringContents);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrString (
      int    inputLineNumber,
      string stringContents);
      
    virtual ~bsrString ();
  
  public:

    // set and get
    // ------------------------------------------------------
                  
    string                getStringContents () const
                              { return fStringContents; }

    S_bsrCellsList        getStringCellsList () const
                              { return fStringCellsList; }

    // services
    // ------------------------------------------------------
                  
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

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    string                fStringContents;

    S_bsrCellsList        fStringCellsList;
};
typedef SMARTP<bsrString> S_bsrString;
EXP ostream& operator<< (ostream& os, const S_bsrString& elt);


} // namespace MusicXML2


#endif
