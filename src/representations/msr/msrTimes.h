/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTimes___
#define ___msrTimes___

#include "msrSegnosAndCodas.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msrTimeItem;
typedef SMARTP<msrTimeItem> S_msrTimeItem;

class msrTime;
typedef SMARTP<msrTime> S_msrTime;

//______________________________________________________________________________
class EXP msrTimeItem : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTimeItem> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTimeItem (
      int inputLineNumber);

    virtual               ~msrTimeItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const vector<int>&    getTimeBeatsNumbersVector () const
                              { return fTimeBeatsNumbersVector; }

    void                  setTimeBeatValue (int timeBeatValue);

    int                   getTimeBeatValue () const
                              { return fTimeBeatValue; }

  public:

    // public services
    // ------------------------------------------------------

    bool                  isEqualTo (S_msrTimeItem otherTimeItem) const;

    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeBeatsNumber () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    void                  print (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    vector<int>           fTimeBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeBeatValue;
};
typedef SMARTP<msrTimeItem> S_msrTimeItem;
EXP ostream& operator<< (ostream& os, const S_msrTimeItem& elt);

//______________________________________________________________________________
class EXP msrTime : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTime> create (
      int               inputLineNumber,
      msrTimeSymbolKind timeSymbolKind);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrTime> createTwoEightsTime (
      int inputLineNumber);

    static SMARTP<msrTime> createThreeEightsTime (
      int inputLineNumber);

    static SMARTP<msrTime> createSixEightsTime (
      int inputLineNumber);

    static SMARTP<msrTime> createTwoQuartersTime (
      int inputLineNumber);

    static SMARTP<msrTime> createThreeQuartersTime (
      int inputLineNumber);

    static SMARTP<msrTime> createFourQuartersTime (
      int inputLineNumber);

    static SMARTP<msrTime> createFiveQuartersTime (
      int inputLineNumber);

    static SMARTP<msrTime> createTwoHalvesTime (
      int inputLineNumber);

    static SMARTP<msrTime> createThreeHalvesTime (
      int inputLineNumber);

    static SMARTP<msrTime> createFourHalvesTime (
      int inputLineNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrTime> createTimeFromString (
      int    inputLineNumber,
      string timeString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTime (
      int               inputLineNumber,
      msrTimeSymbolKind timeSymbolKind);

    virtual               ~msrTime ();

  public:

    // set and get
    // ------------------------------------------------------

    msrTimeSymbolKind     getTimeSymbolKind () const
                              { return fTimeSymbolKind; }

    bool                  getTimeIsCompound () const
                              { return fTimeIsCompound; }

    const vector<S_msrTimeItem>&
                          getTimeItemsVector () const
                              { return fTimeItemsVector; }

  public:

    // public services
    // ------------------------------------------------------

    bool                  isEqualTo (S_msrTime otherTime) const;

    void                  appendTimeItem (
                            S_msrTimeItem timeItem);

    rational              wholeNotesPerMeasure () const;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    string                asString () const override;

    string                asShortString () const override;

    void                  print (ostream& os) const override;

    void                  printShort (ostream& os) const override;

  private:

    // private fields
    // ------------------------------------------------------

    msrTimeSymbolKind     fTimeSymbolKind;

    vector<S_msrTimeItem> fTimeItemsVector;

    // a time is compound if it contains several items
    // or if the only one has several beats numbers
    // i.e. 3/4 is not, (3+4)/8 is, and 2/4+3/4 is too
    bool                  fTimeIsCompound;
};
typedef SMARTP<msrTime> S_msrTime;
EXP ostream& operator<< (ostream& os, const S_msrTime& elt);


} // namespace MusicXML2


#endif
