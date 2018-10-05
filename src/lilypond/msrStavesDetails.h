/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrStavesDetails___
#define ___msrStavesDetails___

#include <list>

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrStaffTuning : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffTuning> create (
      int                      inputLineNumber,
      int                      staffTuningLineNumber,
      msrQuarterTonesPitchKind quarterTonesPitchKind,
      int                      staffTuningOctave);
    
    SMARTP<msrStaffTuning> createStaffTuningNewbornClone ();

    SMARTP<msrStaffTuning> createStaffTuningDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaffTuning (
      int                      inputLineNumber,
      int                      staffTuningLineNumber,
      msrQuarterTonesPitchKind quarterTonesPitchKind,
      int                      staffTuningOctave);
         
    ~ msrStaffTuning ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getStaffTuningLineNumber () const
                              { return fStaffTuningLineNumber; }

    msrQuarterTonesPitchKind
                          getStaffTuningQuarterTonesPitchKind () const
                              { return fStaffTuningQuarterTonesPitchKind; }

    int                   getStaffTuningOctave () const
                              { return fStaffTuningOctave; }

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
    
  public:
  
    // fields
    // ------------------------------------------------------
    
    int                   fStaffTuningLineNumber;
    
    msrQuarterTonesPitchKind
                          fStaffTuningQuarterTonesPitchKind;
    int                   fStaffTuningOctave;
};
typedef SMARTP<msrStaffTuning> S_msrStaffTuning;
EXP ostream& operator<< (ostream& os, const S_msrStaffTuning& elt);

//______________________________________________________________________________
class msrStaffDetails : public msrElement
{
  public:
    
    
    // data types
    // ------------------------------------------------------

    enum msrStaffTypeKind {
      kRegularStaffType,
      kOssiaStaffType, kCueStaffType, kEditorialStaffType,
      kAlternateStaffType};

    static string staffTypeKindAsString (
      msrStaffTypeKind staffTypeKind);
      
    enum msrShowFretsKind {
      kShowFretsNumbers, kShowFretsLetters};

    static string showFretsKindAsString (
      msrShowFretsKind showFretsKind);
      
    enum msrPrintObjectKind {
      kPrintObjectYes, kPrintObjectNo};

    static string printObjectKindAsString (
      msrPrintObjectKind printObjectKind);
      
    enum msrPrintSpacingKind {
      kPrintSpacingYes, kPrintSpacingNo};

    static string printSpacingKindAsString (
      msrPrintSpacingKind printSpacingKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffDetails> create (
      int                 inputLineNumber,
      msrStaffTypeKind    staffTypeKind,
      msrShowFretsKind    showFretsKind,
      msrPrintObjectKind  printObjectKind,
      msrPrintSpacingKind printSpacingKin);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaffDetails (
      int                 inputLineNumber,
      msrStaffTypeKind    staffTypeKind,
      msrShowFretsKind    showFretsKind,
      msrPrintObjectKind  printObjectKind,
      msrPrintSpacingKind printSpacingKin);
      
    virtual ~msrStaffDetails ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrStaffTypeKind      getStaffTypeKind () const
                              { return fStaffTypeKind; }
                        
    void                  setStaffLinesNumber (
                            int staffLinesNumber)
                              { fStaffLinesNumber = staffLinesNumber; }
                        
    int                   getStaffLinesNumber () const
                              { return fStaffLinesNumber; }
                        
    const list<S_msrStaffTuning>&
                          getStaffTuningsList () const
                              { return fStaffTuningsList; }
                        
    msrShowFretsKind      getShowFretsKind () const
                              { return fShowFretsKind; }
                        
    msrPrintObjectKind    getPrintObjectKind () const
                              { return fPrintObjectKind; }
                        
    msrPrintSpacingKind   getPrintSpacingKind () const
                              { return fPrintSpacingKind; }
                        
    // services
    // ------------------------------------------------------

    void                  addStaffTuningToStaffDetails (
                            S_msrStaffTuning staffTuning)
                              {
                                fStaffTuningsList.push_back (staffTuning);
                              }
                            
    string                asShortString ();
    
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

    msrStaffTypeKind      fStaffTypeKind;

    int                   fStaffLinesNumber;
    list<S_msrStaffTuning>
                          fStaffTuningsList;

    msrShowFretsKind      fShowFretsKind;
    
    msrPrintObjectKind    fPrintObjectKind;
    msrPrintSpacingKind   fPrintSpacingKind;
};
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;
EXP ostream& operator<< (ostream& os, const S_msrStaffDetails& elt);


} // namespace MusicXML2


#endif
