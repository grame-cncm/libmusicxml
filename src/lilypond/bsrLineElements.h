/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrLineElements___
#define ___bsrLineElements___

#include "bsrElements.h"
#include "bsrCellsLists.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrLineElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

/*
    static SMARTP<bsrLineElement> create (
      int inputLineNumber);
*/

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    bsrLineElement (
      int inputLineNumber);

    virtual ~bsrLineElement ();

  public:

    // set and get
    // ------------------------------------------------------
/*
    void                  setBsrLineUplink (
                            S_bsrLine bsrLineUplink);
                              
    S_bsrLine             getBsrLineUplink () const;
*/

    void                  setSpacesBefore (int value)
                              { fSpacesBefore = value; };

    int                   getSpacesBefore () const
                              { return fSpacesBefore; }

  public:

    // services
    // ------------------------------------------------------


    virtual S_bsrCellsList
                          asCellsList () const = 0;

    virtual int           fetchCellsNumber () const = 0;

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

/*
    S_bsrLine             fBsrLineUplink;
*/

    int                   fSpacesBefore;
};
typedef SMARTP<bsrLineElement> S_bsrLineElement;
EXP std::ostream& operator<< (std::ostream& os, const S_bsrLineElement& elt);


}


#endif
