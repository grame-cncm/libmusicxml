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

#include "msrSegnosAndCodas.h"

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

  public:

    // public services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    virtual void          print (ostream& os) const override;

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
class msrStaffDetails : public msrMeasureElement
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

  public:

    // public services
    // ------------------------------------------------------

    void                  addStaffTuningToStaffDetails (
                            S_msrStaffTuning staffTuning)
                              {
                                fStaffTuningsList.push_back (staffTuning);
                              }

    string                asShortString () const override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os) const override;

  private:

    // private fields
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
