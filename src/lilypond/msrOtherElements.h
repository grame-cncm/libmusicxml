/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrOtherElements___
#define ___msrOtherElements___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrSegno : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegno> create (
      int  inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSegno (
      int inputLineNumber);
      
    virtual ~msrSegno ();
  
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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<msrSegno> S_msrSegno;
EXP ostream& operator<< (ostream& os, const S_msrSegno& elt);

//______________________________________________________________________________
class msrCoda : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCoda> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrCoda (
      int inputLineNumber);
      
    virtual ~msrCoda ();
  
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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<msrCoda> S_msrCoda;
EXP ostream& operator<< (ostream& os, const S_msrCoda& elt);

//______________________________________________________________________________
class msrEyeGlasses : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrEyeGlasses> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrEyeGlasses (
      int inputLineNumber);
      
    virtual ~msrEyeGlasses ();
  
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

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<msrEyeGlasses> S_msrEyeGlasses;
EXP ostream& operator<< (ostream& os, const S_msrEyeGlasses& elt);

//______________________________________________________________________________
/*
class msrPolyphony : public smartable
{  
  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);
 
  public:

    // fields
    // ------------------------------------------------------

    // what happens every measure
    list<string>          fPolyphonyMeasureActivities;

    // what happens to the voices
    list<S_msrVoice>      fPolyphonyVoiceActivities;
};
*/


} // namespace MusicXML2


#endif
