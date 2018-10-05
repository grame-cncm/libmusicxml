/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrArticulations___
#define ___msrArticulations___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrArticulation : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrArticulationKind {
        kAccent, kBreathMark, kCaesura, 
        kSpiccato,
        kStaccato, kStaccatissimo,
        kStress, kUnstress,
        kDetachedLegato,
        kStrongAccent, kTenuto,
        kFermata, // barline ??? JMI
        kArpeggiato, kNonArpeggiato,
        kDoit, kFalloff, kPlop, kScoop};

    static string articulationKindAsString (
      msrArticulationKind articulationKind);
            
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrArticulation> create (
      int                 inputLineNumber,
      msrArticulationKind articulationKind,
      msrPlacementKind    articulationPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrArticulation (
      int                 inputLineNumber,
      msrArticulationKind articulationKind,
      msrPlacementKind    articulationPlacementKind);
      
    virtual ~msrArticulation ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrArticulationKind   getArticulationKind () const
                              { return fArticulationKind; }
        
    msrPlacementKind      getArticulationPlacementKind () const
                              { return fArticulationPlacementKind; }
                        
    // services
    // ------------------------------------------------------

    virtual string        articulationKindAsString () const;

    virtual string        articulationPlacementKindAsString () const;

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

  protected:

    // fields
    // ------------------------------------------------------

    msrArticulationKind   fArticulationKind;

    msrPlacementKind      fArticulationPlacementKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;
EXP ostream& operator<< (ostream& os, const S_msrArticulation& elt);

//______________________________________________________________________________
class msrFermata : public msrArticulation
{
  public:
          
    // data types
    // ------------------------------------------------------

    enum msrFermataKind {
        kNormalFermataKind, kAngledFermataKind, kSquareFermataKind};

    static string fermataKindAsString (
      msrFermataKind fermataKind);

    enum msrFermataTypeKind {
        kFermataTypeNone,
        kFermataTypeUpright, kFermataTypeInverted };

    static string fermataTypeKindAsString (
      msrFermataTypeKind fermataTypeKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFermata> create (
      int                inputLineNumber,
      msrFermataKind     fermataKind,
      msrFermataTypeKind fermataTypeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFermata (
      int                inputLineNumber,
      msrFermataKind     fermataKind,
      msrFermataTypeKind fermataTypeKind);
      
    virtual ~msrFermata ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrFermataKind        getFermataKind () const
                              { return fFermataKind; }
        
    msrFermataTypeKind    getFermataTypeKind () const
                              { return fFermataTypeKind; }
        
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

    msrFermataKind        fFermataKind;
    
    msrFermataTypeKind    fFermataTypeKind;
};
typedef SMARTP<msrFermata> S_msrFermata;
EXP ostream& operator<< (ostream& os, const S_msrFermata& elt);

//______________________________________________________________________________
class msrArpeggiato : public msrArticulation
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrArpeggiato> create (
      int              inputLineNumber,
      msrPlacementKind arpeggiatoPlacementKind,
      msrDirectionKind arpeggiatoDirectionKind,
      int              arpeggiatoNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrArpeggiato (
      int              inputLineNumber,
      msrPlacementKind arpeggiatoPlacementKind,
      msrDirectionKind arpeggiatoDirectionKind,
      int              arpeggiatoNumber);
      
    virtual ~msrArpeggiato ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrDirectionKind      getArpeggiatoDirectionKind () const
                              { return fArpeggiatoDirectionKind; }
        
    int                   getArpeggiatoNumber () const
                              { return fArpeggiatoNumber; }
        
    // services
    // ------------------------------------------------------

    virtual string        arpeggiatoDirectionKindAsString () const;

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

    msrDirectionKind      fArpeggiatoDirectionKind;

    int                   fArpeggiatoNumber;
};
typedef SMARTP<msrArpeggiato> S_msrArpeggiato;
EXP ostream& operator<< (ostream& os, const S_msrArpeggiato& elt);

//______________________________________________________________________________
class msrNonArpeggiato : public msrArticulation
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrNonArpeggiatoTypeKind {
      kNonArpeggiatoTypeNone,
      kNonArpeggiatoTypeTop, kNonArpeggiatoTypeBottom };

    static string nonArpeggiatoTypeKindAsString (
      msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNonArpeggiato> create (
      int                      inputLineNumber,
      msrPlacementKind         nonArpeggiatoPlacementKind,
      msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
      int                      nonArpeggiatoNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrNonArpeggiato (
      int                      inputLineNumber,
      msrPlacementKind         nonArpeggiatoPlacementKind,
      msrNonArpeggiatoTypeKind nonArpeggiatoTypeKind,
      int                      nonArpeggiatoNumber);
      
    virtual ~msrNonArpeggiato ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrNonArpeggiatoTypeKind
                          getNonArpeggiatoTypeKind () const
                              { return fNonArpeggiatoTypeKind; }
        
    int                   getNonArpeggiatoNumber () const
                              { return fNonArpeggiatoNumber; }
        
    // services
    // ------------------------------------------------------

    virtual string        nonArpeggiatoTypeKindAsString () const;

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
                          
    msrNonArpeggiatoTypeKind
                          fNonArpeggiatoTypeKind;

    int                   fNonArpeggiatoNumber;
};
typedef SMARTP<msrNonArpeggiato> S_msrNonArpeggiato;
EXP ostream& operator<< (ostream& os, const S_msrNonArpeggiato& elt);


} // namespace MusicXML2


#endif
