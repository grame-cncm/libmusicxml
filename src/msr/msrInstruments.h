/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrInstruments___
#define ___msrInstruments___

#include <list>

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrStringTuning : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStringTuning> create (
      int                  inputLineNumber,
      int                  stringTuningNumber,
      msrDiatonicPitchKind stringTuningDiatonicPitchKind,
      msrAlterationKind    stringTuningAlterationKind,
      int                  stringTuningOctave);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStringTuning (
      int                  inputLineNumber,
      int                  stringTuningNumber,
      msrDiatonicPitchKind stringTuningDiatonicPitchKind,
      msrAlterationKind    stringTuningAlterationKind,
      int                  stringTuningOctave);
      
    virtual ~msrStringTuning ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getStringTuningNumber () const
                              { return fStringTuningNumber; }

    msrDiatonicPitchKind  getStringTuningDiatonicPitchKind () const
                              { return fStringTuningDiatonicPitchKind; }

    msrAlterationKind     getStringTuningAlterationKind () const
                              { return fStringTuningAlterationKind; }

    int                   getStringTuningOctave () const
                              { return fStringTuningOctave; }

    // services
    // ------------------------------------------------------

    string                stringTuningKindAsString () const;

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

    int                   fStringTuningNumber;
    
    msrDiatonicPitchKind  fStringTuningDiatonicPitchKind;
    msrAlterationKind     fStringTuningAlterationKind;
    int                   fStringTuningOctave;
};
typedef SMARTP<msrStringTuning> S_msrStringTuning;
EXP ostream& operator<< (ostream& os, const S_msrStringTuning& elt);

//______________________________________________________________________________
class msrScordatura : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrScordaturaKind {
      kScordaturaNone,
      kScordaturaUp, kScordaturaDown,
      kScordaturaStop, kScordaturaContinue };

    static string scordaturaKindAsString (
      msrScordaturaKind scordaturaKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrScordatura> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrScordatura (
      int inputLineNumber);
      
    virtual ~msrScordatura ();
  
  public:

    // set and get
    // ------------------------------------------------------

    const list<S_msrStringTuning>&
                          getScordaturaStringTuningsList ()
                              { return fScordaturaStringTuningsList; }

    // services
    // ------------------------------------------------------

    void                  addStringTuningToScordatura (
                            S_msrStringTuning stringTuning);

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

    list<S_msrStringTuning>
                          fScordaturaStringTuningsList;
};
typedef SMARTP<msrScordatura> S_msrScordatura;
EXP ostream& operator<< (ostream& os, const S_msrScordatura& elt);

//______________________________________________________________________________
class msrAccordionRegistration : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAccordionRegistration> create (
      int inputLineNumber,
      int highDotsNumber,
      int middleDotsNumber,
      int lowDotsNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrAccordionRegistration (
      int inputLineNumber,
      int highDotsNumber,
      int middleDotsNumber,
      int lowDotsNumber);
      
    virtual ~msrAccordionRegistration ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getHighDotsNumber () const
                              { return fHighDotsNumber; }

    int                   getMiddleDotsNumber () const
                              { return fMiddleDotsNumber; }

    int                   getLowDotsNumber () const
                              { return fLowDotsNumber; }

    // services
    // ------------------------------------------------------

    string                asString () const;

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

    // see https://de.wikipedia.org/wiki/Register_%28Akkordeon%29
    // for the meaning of the dots numbers

    int                   fHighDotsNumber;
    int                   fMiddleDotsNumber;
    int                   fLowDotsNumber;
};
typedef SMARTP<msrAccordionRegistration> S_msrAccordionRegistration;
EXP ostream& operator<< (ostream& os, const S_msrAccordionRegistration& elt);

//______________________________________________________________________________
class msrHarpPedalsTuning : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarpPedalsTuning> create (
      int inputLineNumber);
    
    SMARTP<msrHarpPedalsTuning> createHarpPedalsTuningNewbornClone ();

    SMARTP<msrHarpPedalsTuning> createHarpPedalsTuningDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarpPedalsTuning (
      int inputLineNumber);
         
    ~ msrHarpPedalsTuning ();
  
  public:

    // set and get
    // ------------------------------------------------------

    const map<msrDiatonicPitchKind, msrAlterationKind>&
                          getHarpPedalsAlterationKindsMap ()
                              { return fHarpPedalsAlterationKindsMap; }

    // services
    // ------------------------------------------------------

    void                  addPedalTuning (
                            int                  intputLineNumber,
                            msrDiatonicPitchKind diatonicPitchKind,
                            msrAlterationKind    alterationKind);
                            
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
    
  public:
  
    // fields
    // ------------------------------------------------------
    
    map<msrDiatonicPitchKind, msrAlterationKind>
                          fHarpPedalsAlterationKindsMap;
};
typedef SMARTP<msrHarpPedalsTuning> S_msrHarpPedalsTuning;
EXP ostream& operator<< (ostream& os, const S_msrHarpPedalsTuning& elt);

//______________________________________________________________________________
class msrPedal : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrPedalTypeKind {
      kPedalStart, kPedalContinue, kPedalChange, kPedalStop,
      k_NoPedalType };
      
    static string pedalTypeKindAsString (
      msrPedalTypeKind pedalTypeKind);
      
    enum msrPedalLineKind {
      kPedalLineYes, kPedalLineNo};
      
    static string pedalLineKindAsString (
      msrPedalLineKind pedalLineKind);
      
    enum msrPedalSignKind {
      kPedalSignYes, kPedalSignNo};
      
    static string pedalSignKindAsString (
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

    string                pedalTypeAsString ();
    
    string                pedalLineAsString ();
    
    string                pedalSignAsString ();

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
