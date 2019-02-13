/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTechnicals___
#define ___msrTechnicals___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrTechnical : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTechnicalKind {
      kArrow, // rich JMI
      kDoubleTongue,
      kDownBow,
      kFingernails,
      kHarmonic,
      kHeel,
      kHole, // rich JMI
      kOpenString,
      kSnapPizzicato,
      kStopped,
      kTap,
      kThumbPosition,
      kToe,
      kTripleTongue,
      kUpBow};

    static string technicalKindAsString (
      msrTechnicalKind technicalKind);
      
/* JMI
    enum msrTechnicalAccidentalMarkKind {
      kNatural, kSharp, kFlat};

    static string technicalAccidentalMarkKindAsString (
      msrTechnicalAccidentalMarkKind technicalAccidentalMarkKind);
  */
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnical> create (
      int              inputLineNumber,
      msrTechnicalKind technicalKind,
      msrPlacementKind technicalPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTechnical (
      int              inputLineNumber,
      msrTechnicalKind technicalKind,
      msrPlacementKind technicalPlacementKind);
      
    virtual ~msrTechnical ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalKind      getTechnicalKind () const
                              { return fTechnicalKind; }
        
    void                  setTechnicalPlacementKind (
                            msrPlacementKind technicalPlacementKind)
                              {
                                fTechnicalPlacementKind =
                                  technicalPlacementKind;
                              }
        
    msrPlacementKind      getTechnicalPlacementKind () const
                              { return fTechnicalPlacementKind; }

        /* JMI
    void                  setTechnicalAccidentalMarkKind (
                            msrTechnicalAccidentalMarkKind
                              technicalAccidentalMarkKind)
                              {
                                fTechnicalAccidentalMarkKind =
                                  technicalAccidentalMarkKind;
                              }
        
    msrTechnicalAccidentalMarkKind
                          getTechnicalAccidentalMarkKind () const
                              { return fTechnicalAccidentalMarkKind; }
*/

    // services
    // ------------------------------------------------------

    string                technicalKindAsString () const;

    string                technicalPlacementKindAsString () const;

    string                technicalAccidentalMarkKindAsString () const;
    
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

    msrTechnicalKind      fTechnicalKind;

    msrPlacementKind      fTechnicalPlacementKind;
    
// JMI    msrTechnicalAccidentalMarkKind  fTechnicalAccidentalMarkKind;
};
typedef SMARTP<msrTechnical> S_msrTechnical;
EXP ostream& operator<< (ostream& os, const S_msrTechnical& elt);

//______________________________________________________________________________
class msrTechnicalWithInteger : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTechnicalWithIntegerKind {
      kFingering,
      kFret,
      kString };

    static string technicalWithIntegerKindAsString (
      msrTechnicalWithIntegerKind technicalWithIntegerKind);
                  
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnicalWithInteger> create (
      int                         inputLineNumber,
      msrTechnicalWithIntegerKind technicalWithIntegerKind,
      int                         technicalWithIntegerValue,
      msrPlacementKind            technicalWithIntegerPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTechnicalWithInteger (
      int                         inputLineNumber,
      msrTechnicalWithIntegerKind technicalWithIntegerKind,
      int                         technicalWithIntegerValue,
      msrPlacementKind            technicalWithIntegerPlacementKind);
      
    virtual ~msrTechnicalWithInteger ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalWithIntegerKind
                          getTechnicalWithIntegerKind () const
                              { return fTechnicalWithIntegerKind; }
        
    int                   getTechnicalWithIntegerValue () const
                              { return fTechnicalWithIntegerValue; }
        
    void                  setTechnicalWithIntegerPlacementKind (
                            msrPlacementKind
                              technicalWithIntegerPlacementKind)
                              {
                                fTechnicalWithIntegerPlacementKind =
                                  technicalWithIntegerPlacementKind;
                              }
        
    msrPlacementKind      getTechnicalWithIntegerPlacementKind () const
                              { return fTechnicalWithIntegerPlacementKind; }
        
    // services
    // ------------------------------------------------------

    string                technicalWithIntegerKindAsString () const;

    string                technicalWithIntegerPlacementKindAsString () const;

    string                technicalWithIntegerAccidentalMarkKindAsString () const;
    
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

    msrTechnicalWithIntegerKind     fTechnicalWithIntegerKind;

    int                             fTechnicalWithIntegerValue;

    msrPlacementKind                fTechnicalWithIntegerPlacementKind;
};
typedef SMARTP<msrTechnicalWithInteger> S_msrTechnicalWithInteger;
EXP ostream& operator<< (ostream& os, const S_msrTechnicalWithInteger& elt);

//______________________________________________________________________________
class msrTechnicalWithFloat : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTechnicalWithFloatKind {
      kBend };

    static string technicalWithFloatKindAsString (
      msrTechnicalWithFloatKind technicalWithFloatKind);
                  
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnicalWithFloat> create (
      int                       inputLineNumber,
      msrTechnicalWithFloatKind technicalWithFloatKind,
      float                     technicalWithFloatValue,
      msrPlacementKind          technicalWithFloatPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTechnicalWithFloat (
      int                       inputLineNumber,
      msrTechnicalWithFloatKind technicalWithFloatKind,
      float                     technicalWithFloatValue,
      msrPlacementKind          technicalWithFloatPlacementKind);
      
    virtual ~msrTechnicalWithFloat ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalWithFloatKind
                          getTechnicalWithFloatKind () const
                              { return fTechnicalWithFloatKind; }
        
    float                 getTechnicalWithFloatValue () const
                              { return fTechnicalWithFloatValue; }
        
    void                  setTechnicalWithFloatPlacementKind (
                            msrPlacementKind
                              technicalWithFloatPlacementKind)
                              {
                                fTechnicalWithFloatPlacementKind =
                                  technicalWithFloatPlacementKind;
                              }
        
    msrPlacementKind      getTechnicalWithFloatPlacementKind () const
                              { return fTechnicalWithFloatPlacementKind; }

    // services
    // ------------------------------------------------------

    string                technicalWithFloatKindAsString () const;

    string                technicalWithFloatPlacementKindAsString () const;

    string                technicalWithFloatAccidentalMarkKindAsString () const;
    
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

    msrTechnicalWithFloatKind       fTechnicalWithFloatKind;

    float                           fTechnicalWithFloatValue;

    msrPlacementKind                fTechnicalWithFloatPlacementKind;
};
typedef SMARTP<msrTechnicalWithFloat> S_msrTechnicalWithFloat;
EXP ostream& operator<< (ostream& os, const S_msrTechnicalWithFloat& elt);

//______________________________________________________________________________
class msrTechnicalWithString : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTechnicalWithStringKind {
      kHammerOn,
      kHandbell,
      kOtherTechnical,
      kPluck,
      kPullOff};

    static string technicalWithStringKindAsString (
      msrTechnicalWithStringKind technicalWithStringKind);
                  
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnicalWithString> create (
      int                        inputLineNumber,
      msrTechnicalWithStringKind technicalWithStringKind,
      msrTechnicalTypeKind       technicalWithStringTypeKind,
      string                     technicalWithStringValue,
      msrPlacementKind           technicalWithStringPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTechnicalWithString (
      int                        inputLineNumber,
      msrTechnicalWithStringKind technicalWithStringKind,
      msrTechnicalTypeKind       technicalWithStringTypeKind,
      string                     technicalWithStringValue,
      msrPlacementKind           technicalWithStringPlacementKind);
      
    virtual ~msrTechnicalWithString ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalWithStringKind
                          getTechnicalWithStringKind () const
                              { return fTechnicalWithStringKind; }

    msrTechnicalTypeKind  getTechnicalWithStringTypeKind () const
                              { return fTechnicalWithStringTypeKind; }

    string                getTechnicalWithStringValue () const
                              { return fTechnicalWithStringValue; }
        
    void                  setTechnicalWithStringPlacementKind (
                            msrPlacementKind
                              technicalWithStringPlacementKind)
                              {
                                fTechnicalWithStringPlacementKind =
                                  technicalWithStringPlacementKind;
                              }
        
    msrPlacementKind      getTechnicalWithStringPlacementKind () const
                              { return fTechnicalWithStringPlacementKind; }

    // services
    // ------------------------------------------------------

    string                technicalWithStringKindAsString () const;
    
    string                technicalWithStringTypeKindAsString () const;

    string                technicalWithStringPlacementKindAsString () const;

    string                technicalWithStringAccidentalMarkKindAsString () const;
    
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

    msrTechnicalWithStringKind
                          fTechnicalWithStringKind;

    msrTechnicalTypeKind  fTechnicalWithStringTypeKind;

    string                fTechnicalWithStringValue;

    msrPlacementKind      fTechnicalWithStringPlacementKind;
};
typedef SMARTP<msrTechnicalWithString> S_msrTechnicalWithString;
EXP ostream& operator<< (ostream& os, const S_msrTechnicalWithString& elt);


} // namespace MusicXML2


#endif
