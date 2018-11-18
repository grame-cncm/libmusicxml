/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrTimes___
#define ___bsrTimes___

#include "bsrElements.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
class bsrTimeItem;
typedef SMARTP<bsrTimeItem> S_bsrTimeItem;

class bsrTime;
typedef SMARTP<bsrTime> S_bsrTime;

//______________________________________________________________________________
class bsrTimeItem : public bsrElement
{
  public:
    
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTimeItem> create (
      int inputLineNumber);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTimeItem (
      int inputLineNumber);
            
    virtual ~bsrTimeItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const vector<int>&    getTimeBeatsNumbersVector ()
                              { return fTimeBeatsNumbersVector; }

    void                  setTimeBeatValue (int timeBeatValue)
                              { fTimeBeatValue = timeBeatValue; }
                              
    int                   getTimeBeatValue () const
                              { return fTimeBeatValue; }

    // services
    // ------------------------------------------------------

    bool                  isEqualTo (S_bsrTimeItem otherTimeItem) const;
                            
    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeBeatsNumber () const;
                              
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

    vector<int>           fTimeBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeBeatValue;
};
typedef SMARTP<bsrTimeItem> S_bsrTimeItem;
EXP ostream& operator<< (ostream& os, const S_bsrTimeItem& elt);

//______________________________________________________________________________
class bsrTime : public bsrElement
{
  public:
          
    // creation
    // ------------------------------------------------------

    static SMARTP<bsrTime> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTime (
      int inputLineNumber);
      
    virtual ~bsrTime ();
  
  public:

    // set and get
    // ------------------------------------------------------

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

};
typedef SMARTP<bsrTime> S_bsrTime;
EXP ostream& operator<< (ostream& os, const S_bsrTime& elt);


} // namespace MusicXML2


#endif
