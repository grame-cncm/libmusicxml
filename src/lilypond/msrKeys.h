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

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrHumdrumScotKeyItem;
typedef SMARTP<msrHumdrumScotKeyItem> S_msrHumdrumScotKeyItem;

//______________________________________________________________________________
class msrHumdrumScotKeyItem : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrHumdrumScotKeyItemKind {
        kTraditionalKind, kHumdrumScotKind };
        
    static string HumdrumScotKeyItemKindAsString (
      msrHumdrumScotKeyItemKind HumdrumScotKeyItemKind);

    enum msrHumdrumScotKeyItemModeKind {
        kMajorMode, kMinorMode,
        kIonianMode, kDorianMode, kPhrygianMode, kLydianMode,
        kMixolydianMode, kAeolianMode, kLocrianMode };

    static string HumdrumScotKeyItemModeKindAsString (
      msrHumdrumScotKeyItemModeKind HumdrumScotKeyItemModeKind);
      
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

    void                  setKeyItemOctave (int keyOctave);
                              
    int                   getKeyItemOctave () const
                              { return fKeyOctave; }

    // services
    // ------------------------------------------------------

    bool                  isEqualTo (
                            S_msrHumdrumScotKeyItem
                              otherHumdrumScotKeyItem) const;
                            
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

    msrDiatonicPitchKind  fKeyDiatonicPitchKind;
    msrAlterationKind     fKeyAlterationKind;
    int                   fKeyOctave;
};
typedef SMARTP<msrHumdrumScotKeyItem> S_msrHumdrumScotKeyItem;
EXP ostream& operator<< (ostream& os, const S_msrHumdrumScotKeyItem& elt);

//______________________________________________________________________________
class msrKey;
typedef SMARTP<msrKey> S_msrKey;

class msrKey : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrKeyKind {
        kTraditionalKind, kHumdrumScotKind };
        
    static string keyKindAsString (
      msrKeyKind keyKind);

    enum msrKeyModeKind {
        kMajorMode, kMinorMode,
        kIonianMode, kDorianMode, kPhrygianMode, kLydianMode,
        kMixolydianMode, kAeolianMode, kLocrianMode };

    static string keyModeKindAsString (
      msrKeyModeKind keyModeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrKey> createTraditional (
      int                      inputLineNumber,
      msrQuarterTonesPitchKind keyTonicPitchKind,
      msrKeyModeKind           keyModeKind,
      int                      keyCancel);
      
    static SMARTP<msrKey> createHumdrumScot (
      int                  inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrKey ( // for traditional keys
      int                      inputLineNumber,
      msrQuarterTonesPitchKind keyTonicPitchKind,
      msrKeyModeKind           keyModeKind,
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
                              
    msrKeyModeKind        getKeyModeKind () const
                              { return fKeyModeKind; }
                            
    int                   getKeyCancel () const
                              { return fKeyCancel; }

    // Humdrum/Scot keys

 //   void                  setKeyItemsOctavesAreSpecified ()
 //                           { fKeyItemsOctavesAreSpecified = true; }
                              
    bool                  getKeyItemsOctavesAreSpecified () const
                              { return fKeyItemsOctavesAreSpecified; }

    const vector<S_msrHumdrumScotKeyItem>&
                          getHumdrumScotKeyItemsVector ()
                              { return fHumdrumScotKeyItemsVector; }

    
    // services
    // ------------------------------------------------------

    bool                  isEqualTo (S_msrKey otherKey) const;
                            
    void                  appendHumdrumScotKeyItem (
                            S_msrHumdrumScotKeyItem item);                                

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

    msrKeyKind            fKeyKind;
    
    // traditional keys

    msrQuarterTonesPitchKind
                          fKeyTonicQuarterTonesPitchKind;
    msrKeyModeKind        fKeyModeKind;
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
