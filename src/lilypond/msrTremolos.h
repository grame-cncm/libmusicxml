/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTremolos___
#define ___msrTremolos___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrSingleTremolo : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSingleTremolo> create (
      int              inputLineNumber,
      int              singleTremoloMarksNumber,
      msrPlacementKind singleTremoloPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSingleTremolo (
      int              inputLineNumber,
      int              singleTremoloMarksNumber,
      msrPlacementKind singleTremoloPlacementKind);
      
    virtual ~msrSingleTremolo ();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setSingleTremoloPlacementKind (
                            msrPlacementKind
                              SingleTremoloPlacementKind)
                              {
                                fSingleTremoloPlacementKind =
                                  SingleTremoloPlacementKind;
                              }
        
    int                   getSingleTremoloMarksNumber () const
                              { return fSingleTremoloMarksNumber; }
                
    msrPlacementKind      getSingleTremoloPlacementKind () const
                              { return fSingleTremoloPlacementKind; }
        
    void                  setSingleTremoloGraphicDurationKind (
                            msrDurationKind durationKind)
                              {
                                fSingleTremoloGraphicDurationKind =
                                  durationKind;
                              }
                
    msrDurationKind       getSingleTremoloGraphicDurationKind () const
                              {
                                return fSingleTremoloGraphicDurationKind;
                              }
        
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

    string                singleTremoloPlacementKindAsString () const;
    
    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fSingleTremoloMarksNumber;

    msrPlacementKind      fSingleTremoloPlacementKind;

    msrDurationKind       fSingleTremoloGraphicDurationKind;
};
typedef SMARTP<msrSingleTremolo> S_msrSingleTremolo;
EXP ostream& operator<< (ostream& os, const S_msrSingleTremolo& elt);


} // namespace MusicXML2


#endif
