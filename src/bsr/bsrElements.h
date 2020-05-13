/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrElements___
#define ___bsrElements___

#include "msrElements.h"


namespace MusicXML2
{

//______________________________________________________________________________
class bsrElement : public msrElement
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<bsrElement> create (
      int inputLineNumber);

  protected:
         
    // constructors/destructor
    // ------------------------------------------------------

    bsrElement (
      int inputLineNumber);

    virtual ~bsrElement ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

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

    virtual std::string   asString () const;
    
    virtual std::string   asShortString () const;

    virtual void          print (ostream& os) const;

};
typedef SMARTP<bsrElement> S_bsrElement;
EXP ostream& operator<< (ostream& os, const S_bsrElement& elt);

//______________________________________________________________________________
template <typename T> class bsrBrowser : public browser<T> 
{
  protected:
  
    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:
    
    bsrBrowser (basevisitor* v) : fVisitor (v)
    {}
    
    virtual ~bsrBrowser ()
    {}

    virtual void          set (basevisitor* v)
                            { fVisitor = v; }
    
    virtual               void browse (T& t)
                            {
                              enter (t);

                              t.browseData (fVisitor);
                              
                              leave (t);
                            }
};


}


#endif
