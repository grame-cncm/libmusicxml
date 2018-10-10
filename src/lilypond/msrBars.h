/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrBars___
#define ___msrBars___

#include <list>

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrBarCheck : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarCheck> create (
      int    inputLineNumber);

    static SMARTP<msrBarCheck> createWithNextBarNumber (
      int    inputLineNumber,
      string nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBarCheck (
      int    inputLineNumber);
      
    msrBarCheck (
      int    inputLineNumber,
      string nextBarNumber);
      
    virtual ~msrBarCheck ();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setNextBarNumber ( string number);
                              
    string                getNextBarNumber () const
                              { return fNextBarNumber; }

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

    string                fNextBarNumber;
};
typedef SMARTP<msrBarCheck> S_msrBarCheck;
EXP ostream& operator<< (ostream& os, const S_msrBarCheck& elt);

//______________________________________________________________________________
class msrBarNumberCheck : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarNumberCheck> create (
      int    inputLineNumber,
      string nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBarNumberCheck (
      int    inputLineNumber,
      string nextBarNumber);
      
    virtual ~msrBarNumberCheck ();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getNextBarNumber () const
                              { return fNextBarNumber; }

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

    string                fNextBarNumber;
};
typedef SMARTP<msrBarNumberCheck> S_msrBarNumberCheck;
EXP ostream& operator<< (ostream& os, const S_msrBarNumberCheck& elt);

//______________________________________________________________________________
class msrBarline : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    // location
    enum msrBarlineLocationKind {
      kBarlineLocationNone,
      kBarlineLocationLeft, kBarlineLocationMiddle, kBarlineLocationRight };
        // kBarlineLocationRight by default
        
    static string barlineLocationKindAsString (
      msrBarlineLocationKind barlineLocationKind);

    // style
    enum msrBarlineStyleKind {
      kBarlineStyleNone,
      kBarlineStyleRegular,  // by default
      kBarlineStyleDotted, kBarlineStyleDashed, kBarlineStyleHeavy,
      kBarlineStyleLightLight, kBarlineStyleLightHeavy,
      kBarlineStyleHeavyLight, kBarlineStyleHeavyHeavy,
      kBarlineStyleTick, kBarlineStyleShort };

    static string barlineStyleKindAsString (
      msrBarlineStyleKind barlineStyleKind);

    // ending type
    enum msrBarlineEndingTypeKind {
      kBarlineEndingNone,
      kBarlineEndingTypeStart, kBarlineEndingTypeStop, kBarlineEndingTypeDiscontinue };

    static string barlineEndingTypeKindAsString (
      msrBarlineEndingTypeKind barlineEndingTypeKind);

    // repeat direction
    enum msrBarlineRepeatDirectionKind {
      kBarlineRepeatDirectionNone,
      kBarlineRepeatDirectionForward, kBarlineRepeatDirectionBackward };

    static string barlineRepeatDirectionKindAsString (
      msrBarlineRepeatDirectionKind barlineRepeatDirectionKind);

    // repeat winged
    enum msrBarlineRepeatWingedKind {
      kBarlineRepeatWingedNone,
      kBarlineRepeatWingedStraight, kBarlineRepeatWingedCurved,
      kBarlineRepeatWingedDoubleStraight, kBarlineRepeatWingedDoubleCurved };

    static string barlineRepeatWingedKindAsString (
      msrBarlineRepeatWingedKind barlineRepeatWingedKind);

    // category
    enum msrBarlineCategoryKind {
      k_NoBarlineCategory,
      kBarlineCategoryStandalone,
      kBarlineCategoryRepeatStart, kBarlineCategoryRepeatEnd,
      kBarlineCategoryHookedEndingStart, kBarlineCategoryHookedEndingEnd,
      kBarlineCategoryHooklessEndingStart, kBarlineCategoryHooklessEndingEnd };
      
    static string barlineCategoryKindAsString (
      msrBarlineCategoryKind barlineCategoryKind);

    // segno
    enum msrBarlineHasSegnoKind {
      kBarlineHasSegnoYes, kBarlineHasSegnoNo};

    static string barlineHasSegnoKindAsString (
      msrBarlineHasSegnoKind barlineHasSegnoKind);

    // coda
    enum msrBarlineHasCodaKind {
      kBarlineHasCodaYes, kBarlineHasCodaNo};

    static string barlineHasCodaKindAsString (
      msrBarlineHasCodaKind barlineHasCodaKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarline> create (
      int                           inputLineNumber,
      msrBarlineCategoryKind        barlineCategoryKind,
      msrBarlineHasSegnoKind        barlineHasSegnoKind,
      msrBarlineHasCodaKind         barlineHasCodaKind,
      msrBarlineLocationKind        location,
      msrBarlineStyleKind           style,
      msrBarlineEndingTypeKind      endingType,
      string                        endingNumber,
      msrBarlineRepeatDirectionKind repeatDirection,
      msrBarlineRepeatWingedKind    repeatWinged,
      int                           barlineTimes);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBarline (
      int                           inputLineNumber,
      msrBarlineCategoryKind        barlineCategoryKind,
      msrBarlineHasSegnoKind        barlineHasSegnoKind,
      msrBarlineHasCodaKind         barlineHasCodaKind,
      msrBarlineLocationKind        location,
      msrBarlineStyleKind           style,
      msrBarlineEndingTypeKind      endingType,
      string                        endingNumber,
      msrBarlineRepeatDirectionKind repeatDirection,
      msrBarlineRepeatWingedKind    repeatWinged,
      int                           barlineTimes);
      
    virtual ~msrBarline ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrBarlineHasSegnoKind
                          getBarlineHasSegnoKind () const
                              { return fBarlineHasSegnoKind; }
    msrBarlineHasCodaKind getBarlineHasCodaKind () const
                              { return fBarlineHasCodaKind; }

    msrBarlineLocationKind
                          getLocation () const // JMI
                              { return fLocationKind; }
                    
    msrBarlineStyleKind   getBarlineStyleKind () const
                              { return fStyleKind; }
                    
    msrBarlineEndingTypeKind
                          getEndingType () const
                              { return fEndingTypeKind; }
                    
    string                getEndingNumber () const
                              { return fEndingNumber; }
                    
    msrBarlineRepeatDirectionKind
                          getRepeatDirection () const
                              { return fRepeatDirectionKind; }
                    
    msrBarlineRepeatWingedKind
                          getRepeatWinged () const
                              { return fRepeatWingedKind; }
                    
    int                   getBarlineTimes () const
                              { return fBarlineTimes; }
                    
    const list<int>&      getEndingNumbersList () const
                              { return fEndingNumbersList; }
                        
    msrBarlineCategoryKind
                          getBarlineCategory () const
                              { return fBarlineCategoryKind; }

    void                  setBarlineCategory (
                            msrBarlineCategoryKind barlineCategoryKind)
                              { fBarlineCategoryKind = barlineCategoryKind; }
    
    // measure number
    void                  setBarlineMeasureNumber (
                            rational positionInMeasure) // JMI virer???
                              {
                                fBarlineMeasureNumber =
                                  positionInMeasure;
                              }
                      
    int                   getBarlineMeasureNumber () const
                              { return fBarlineMeasureNumber; }

    // position in measure
    void                  setBarlinePositionInMeasure (
                            rational positionInMeasure)
                              {
                                fBarlinePositionInMeasure =
                                  positionInMeasure;
                              }
                      
    rational              getBarlinePositionInMeasure ()
                              { return fBarlinePositionInMeasure; }

    // services
    // ------------------------------------------------------

    string                endingNumbersListAsString () const;
    
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

    msrBarlineHasSegnoKind
                          fBarlineHasSegnoKind;
    msrBarlineHasCodaKind fBarlineHasCodaKind;

    msrBarlineLocationKind
                          fLocationKind;
    msrBarlineStyleKind   fStyleKind;
    msrBarlineEndingTypeKind
                          fEndingTypeKind;
                          
    string                fEndingNumber; // may be "1, 2"
    
    msrBarlineRepeatDirectionKind
                          fRepeatDirectionKind;
    msrBarlineRepeatWingedKind
                          fRepeatWingedKind;

    int                   fBarlineTimes;

    msrBarlineCategoryKind
                          fBarlineCategoryKind;

    int                   fBarlineMeasureNumber;
    rational              fBarlinePositionInMeasure;

    // the numbers extracted from fEndingNumber
    list<int>             fEndingNumbersList;
};
typedef SMARTP<msrBarline> S_msrBarline;
EXP ostream& operator<< (ostream& os, const S_msrBarline& elt);


} // namespace MusicXML2


#endif
