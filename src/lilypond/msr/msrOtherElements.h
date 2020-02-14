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

#include "msrMeasureElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrStaffLevelElement : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffLevelElement> create (
      int  inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaffLevelElement (
      int inputLineNumber);

    virtual ~msrStaffLevelElement ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getStaffLevelElementStaffNumber () const
                              { return fStaffLevelElementStaffNumber; }

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

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    int                   fStaffLevelElementStaffNumber;
};
typedef SMARTP<msrStaffLevelElement> S_msrStaffLevelElement;
EXP ostream& operator<< (ostream& os, const S_msrStaffLevelElement& elt);

//______________________________________________________________________________
class msrSegno : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegno> create (
      int  inputLineNumber,
      int  staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSegno (
      int inputLineNumber,
      int staffNumber);

    virtual ~msrSegno ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getfStaffNumber () const
                              { return fStaffNumber; }

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

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    int                   fStaffNumber;
};
typedef SMARTP<msrSegno> S_msrSegno;
EXP ostream& operator<< (ostream& os, const S_msrSegno& elt);

//______________________________________________________________________________
class msrDalSegno : public msrMeasureElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrDalSegnoKind {
      kDalSegnoNone,
      kDalSegno, kDalSegnoAlFine, kDalSegnoAlCoda };

    static string dalSegnoKindAsString (
      msrDalSegnoKind dalSegnoKind);

    static msrDalSegnoKind msrDalSegnoKindFromString (
      string theString);

  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDalSegno> create (
      int             inputLineNumber,
      msrDalSegnoKind dalSegnoKind,
      string          dalSegnoString,
      int             staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDalSegno (
      int             inputLineNumber,
      msrDalSegnoKind dalSegnoKind,
      string          dalSegnoString,
      int             staffNumber);

    virtual ~msrDalSegno ();

  public:

    // set and get
    // ------------------------------------------------------

    msrDalSegnoKind       getDalSegnoKind () const
                              { return fDalSegnoKind; }

    string                getDalSegnoString () const
                              { return fDalSegnoString; }

    int                   getStaffNumber () const
                              { return fStaffNumber; }

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

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------


      msrDalSegnoKind     fDalSegnoKind;

      string              fDalSegnoString;

      int                 fStaffNumber;
};
typedef SMARTP<msrDalSegno> S_msrDalSegno;
EXP ostream& operator<< (ostream& os, const S_msrDalSegno& elt);

//______________________________________________________________________________
class msrCoda : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCoda> create (
      int inputLineNumber,
      int staffNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrCoda (
      int inputLineNumber,
      int staffNumber);

    virtual ~msrCoda ();

  public:

    // set and get
    // ------------------------------------------------------

    int                   getfStaffNumber () const
                              { return fStaffNumber; }

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

    string                asString () const;

    virtual void          print (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------

    int                   fStaffNumber;
};
typedef SMARTP<msrCoda> S_msrCoda;
EXP ostream& operator<< (ostream& os, const S_msrCoda& elt);

//______________________________________________________________________________
class msrEyeGlasses : public msrMeasureElement
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

    string                asString () const;

    virtual void          print (ostream& os) const;

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

    virtual void          print (ostream& os) const;

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
