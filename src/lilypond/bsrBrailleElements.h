/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrBrailleElements___
#define ___bsrBrailleElements___

#include "bsrElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrBrailleElement : public bsrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrBrailleElement> create (
      int inputLineNumber);

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    bsrBrailleElement (
      int inputLineNumber);

    virtual ~bsrBrailleElement ();

  public:

    // set and get
    // ------------------------------------------------------

    bool                  getASpaceIsNeededBefore () const
                              { return fASpaceIsNeededBefore; }

    bool                  getASpaceIsNeededAfter () const
                              { return fASpaceIsNeededAfter; }

    // services
    // ------------------------------------------------------

    int                   getBrailleCellsNumber () const;

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

    wchar_t               fBrailleCellsList;
    
    bool                  fASpaceIsNeededBefore;
    bool                  fASpaceIsNeededAfter;
};
typedef SMARTP<bsrBrailleElement> S_bsrBrailleElement;
EXP std::ostream& operator<< (std::ostream& os, const S_bsrBrailleElement& elt);


}


#endif
