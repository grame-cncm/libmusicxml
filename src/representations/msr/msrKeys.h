/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrKeys___
#define ___msrKeys___

#include <list>

#include "msrSegnosAndCodas.h"

#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrHumdrumScotKeyItem;
typedef SMARTP<msrHumdrumScotKeyItem> S_msrHumdrumScotKeyItem;

//______________________________________________________________________________
class EXP msrHumdrumScotKeyItem : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHumdrumScotKeyItem> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHumdrumScotKeyItem (
      int inputLineNumber);

    virtual ~msrHumdrumScotKeyItem ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setKeyItemDiatonicPitchKind (
                            msrDiatonicPitchKind diatonicPitchKind);

    msrDiatonicPitchKind  getKeyItemDiatonicPitchKind () const
                              { return fKeyDiatonicPitchKind; }

    void                  setKeyItemAlterationKind (
                            msrAlterationKind alterationKind);

    msrAlterationKind     getKeyItemAlterationKind () const
                              { return fKeyAlterationKind; }

    void                  setKeyItemOctaveKind (msrOctaveKind keyOctaveKind);

    msrOctaveKind         getKeyItemOctaveKind () const
                              { return fKeyOctaveKind; }

  public:

    // public services
    // ------------------------------------------------------

    bool                  isEqualTo (
                            S_msrHumdrumScotKeyItem
                              otherHumdrumScotKeyItem) const;

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

  private:

    // fields
    // ------------------------------------------------------

    msrDiatonicPitchKind  fKeyDiatonicPitchKind;
    msrAlterationKind     fKeyAlterationKind;
    msrOctaveKind         fKeyOctaveKind;
};
typedef SMARTP<msrHumdrumScotKeyItem> S_msrHumdrumScotKeyItem;
EXP ostream& operator<< (ostream& os, const S_msrHumdrumScotKeyItem& elt);

//______________________________________________________________________________
class EXP msrKey;
typedef SMARTP<msrKey> S_msrKey;

class EXP msrKey : public msrMeasureElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrKey> createTraditional (
      int                      inputLineNumber,
      msrQuarterTonesPitchKind keyTonicPitchKind,
      msrModeKind           modeKind,
      int                      keyCancel);

    static SMARTP<msrKey> createHumdrumScot (
      int                  inputLineNumber);

    // creation from the applications
    // ------------------------------------------------------

    static SMARTP<msrKey> createTraditionalKeyFromString (
      int    inputLineNumber,
      string keyString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrKey ( // for traditional keys
      int                      inputLineNumber,
      msrQuarterTonesPitchKind keyTonicPitchKind,
      msrModeKind           modeKind,
      int                      keyCancel);

    msrKey ( // for Humdrum/Scot keys
      int                  inputLineNumber);

    virtual ~msrKey ();

  public:

    // set and get
    // ------------------------------------------------------

    msrKeyKind            getKeyKind () const
                              { return fKeyKind; }

    // traditional keys

    msrQuarterTonesPitchKind
                          getKeyTonicQuarterTonesPitchKind () const
                              { return fKeyTonicQuarterTonesPitchKind; }

    msrModeKind        getModeKind () const
                              { return fModeKind; }

    int                   getKeyCancel () const
                              { return fKeyCancel; }

    // Humdrum/Scot keys

 //   void                  setKeyItemsOctavesAreSpecified ()
 //                           { fKeyItemsOctavesAreSpecified = true; }

    bool                  getKeyItemsOctavesAreSpecified () const
                              { return fKeyItemsOctavesAreSpecified; }

    const vector<S_msrHumdrumScotKeyItem>&
                          getHumdrumScotKeyItemsVector () const
                              { return fHumdrumScotKeyItemsVector; }


  public:

    // public services
    // ------------------------------------------------------

    bool                  isEqualTo (S_msrKey otherKey) const;

    void                  appendHumdrumScotKeyItem (
                            S_msrHumdrumScotKeyItem item);

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

  private:

    // private fields
    // ------------------------------------------------------

    msrKeyKind            fKeyKind;

    // traditional keys

    msrQuarterTonesPitchKind
                          fKeyTonicQuarterTonesPitchKind;
    msrModeKind        fModeKind;
    int                   fKeyCancel;

    // Humdrum/Scot keys
    vector<S_msrHumdrumScotKeyItem>
                          fHumdrumScotKeyItemsVector;
    bool                  fKeyItemsOctavesAreSpecified;
};
typedef SMARTP<msrKey> S_msrKey;
EXP ostream& operator<< (ostream& os, const S_msrKey& elt);


} // namespace MusicXML2


#endif
