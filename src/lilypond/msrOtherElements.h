/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrOtherElements__
#define __msrOtherElements__

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
      int                       inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSegno (
      int                       inputLineNumber);
      
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
      int                       inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrCoda (
      int                       inputLineNumber);
      
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
class msrPedal : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrPedalTypeKind {
      kPedalStart, kPedalContinue, kPedalChange, kPedalStop };
      
    static std::string pedalTypeKindAsString (
      msrPedalTypeKind pedalTypeKind);
      
    enum msrPedalLineKind {
      kPedalLineYes, kPedalLineNo};
      
    static std::string pedalLineKindAsString (
      msrPedalLineKind pedalLineKind);
      
    enum msrPedalSignKind {
      kPedalSignYes, kPedalSignNo};
      
    static std::string pedalSignKindAsString (
      msrPedalSignKind pedalSignKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPedal> create (
      int              inputLineNumber,
      msrPedalTypeKind pedalTypeKind,
      msrPedalLineKind pedalLineKind,
      msrPedalSignKind pedalSignKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPedal (
      int              inputLineNumber,
      msrPedalTypeKind pedalTypeKind,
      msrPedalLineKind pedalLineKind,
      msrPedalSignKind pedalSignKind);
      
    virtual ~msrPedal ();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    msrPedalTypeKind      getPedalTypeKind () const
                              { return fPedalTypeKind; }
                    
    msrPedalLineKind      getPedalLineKind () const
                              { return fPedalLineKind; }
                    
    msrPedalSignKind      getPedalSignKind () const
                              { return fPedalSignKind; }
                    
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

    std::string           pedalTypeAsString ();
    
    std::string           pedalLineAsString ();
    
    std::string           pedalSignAsString ();

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrPedalTypeKind      fPedalTypeKind;
    msrPedalLineKind      fPedalLineKind;
    msrPedalSignKind      fPedalSignKind;
};
typedef SMARTP<msrPedal> S_msrPedal;
EXP ostream& operator<< (ostream& os, const S_msrPedal& elt);

//______________________________________________________________________________
class msrDamp : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDamp> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDamp (
      int inputLineNumber);
      
    virtual ~msrDamp ();
  
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
typedef SMARTP<msrDamp> S_msrDamp;
EXP ostream& operator<< (ostream& os, const S_msrDamp& elt);

//______________________________________________________________________________
class msrDampAll : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDampAll> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDampAll (
      int inputLineNumber);
      
    virtual ~msrDampAll ();
  
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
typedef SMARTP<msrDampAll> S_msrDampAll;
EXP ostream& operator<< (ostream& os, const S_msrDampAll& elt);


} // namespace MusicXML2


#endif
