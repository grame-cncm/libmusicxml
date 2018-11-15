/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrNumbers___
#define ___bsrNumbers___

#include "bsrElements.h"

#include "bsrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrNumber : public bsrElement
{
  public:
          
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrNumber> create (
      int inputLineNumber,
      int numberValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrNumber (
      int inputLineNumber,
      int numberValue);
      
    virtual ~bsrNumber ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getNumberValue () const
                              { return fNumberValue; }
                  
    void                  setNumberSignIsNeeded (bool value)
                              { fNumberSignIsNeeded = value; }
                  
    bool                  getNumberSignIsNeeded () const
                              { return fNumberSignIsNeeded; }
                  
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

    int                   fNumberValue;

    bool                  fNumberSignIsNeeded;
};
typedef SMARTP<bsrNumber> S_bsrNumber;
EXP ostream& operator<< (ostream& os, const S_bsrNumber& elt);


} // namespace MusicXML2


#endif
