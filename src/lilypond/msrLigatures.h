/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrLigatures___
#define ___msrLigatures___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrLigature;
typedef SMARTP<msrLigature> S_msrLigature;

class msrLigature : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrLigatureKind {
      kLigatureNone,
      kLigatureStart, kLigatureContinue, kLigatureStop};
    
    static string ligatureKindAsString (
      msrLigatureKind ligatureKind);
      
    enum msrLigatureLineEndKind {
      kLigatureLineEndNone,
      kLigatureLineEndUp, kLigatureLineEndDown,
      kLigatureLineEndBoth, kLigatureLineEndArrow };
    
    static string ligatureLineEndKindAsString (
      msrLigatureLineEndKind ligatureLineEndKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLigature> create (
      int                    inputLineNumber,
      int                    ligatureNumber,
      msrLigatureKind        ligatureKind,
      msrLigatureLineEndKind ligatureLineEndKind,
      msrLineTypeKind        ligatureLineTypeKind,
      msrPlacementKind       ligaturePlacementKind);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrLigature (
      int                    inputLineNumber,
      int                    ligatureNumber,
      msrLigatureKind        ligatureKind,
      msrLigatureLineEndKind ligatureLineEndKind,
      msrLineTypeKind        ligatureLineTypeKind,
      msrPlacementKind       ligaturePlacementKind);
      
    virtual ~msrLigature ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // sidelinks
    void                  setLigatureOtherEndSidelink (
                            S_msrLigature otherEndSideLink);
      
    S_msrLigature         getLigatureOtherEndSidelink () const
                              { return fLigatureOtherEndSidelink; }

    int                   getLigatureNumber () const
                              { return fLigatureNumber; }
    
    msrLigatureKind       getLigatureKind () const
                              { return fLigatureKind; }

    msrLigatureLineEndKind
                          getLigatureLineEndKind () const
                              { return fLigatureLineEndKind; }

    msrLineTypeKind       getLigatureLineTypeKind () const
                              { return fLigatureLineTypeKind; }

    msrPlacementKind      getLigaturePlacementKind () const
                              { return fLigaturePlacementKind; }

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

    string                ligatureKindAsString ();

    virtual void          print (ostream& os);

  private:

    // sidelinks
    // ------------------------------------------------------
    S_msrLigature         fLigatureOtherEndSidelink; // two-way

    // fields
    // ------------------------------------------------------

    int                   fLigatureNumber;

    msrLigatureKind       fLigatureKind;

    msrLigatureLineEndKind
                          fLigatureLineEndKind;
                          
    msrLineTypeKind       fLigatureLineTypeKind;
    
    msrPlacementKind      fLigaturePlacementKind;
};
typedef SMARTP<msrLigature> S_msrLigature;
EXP ostream& operator<< (ostream& os, const S_msrLigature& elt);


} // namespace MusicXML2


#endif
