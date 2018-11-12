/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrKeys___
#define ___bsrKeys___

#include <list>

#include "bsrElements.h"

#include "bsrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrHumdrumScotKeyItem;
typedef SMARTP<bsrHumdrumScotKeyItem> S_bsrHumdrumScotKeyItem;

//______________________________________________________________________________
class bsrHumdrumScotKeyItem : public bsrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum bsrHumdrumScotKeyItemKind {
        kTraditionalKind, kHumdrumScotKind };
        
    static string HumdrumScotKeyItemKindAsString (
      bsrHumdrumScotKeyItemKind HumdrumScotKeyItemKind);

    enum bsrHumdrumScotKeyItemModeKind {
        kMajorMode, kMinorMode,
        kIonianMode, kDorianMode, kPhrygianMode, kLydianMode,
        kMixolydianMode, kAeolianMode, kLocrianMode };

    static string HumdrumScotKeyItemModeKindAsString (
      bsrHumdrumScotKeyItemModeKind HumdrumScotKeyItemModeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<bsrHumdrumScotKeyItem> create (
      int inputLineNumber);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrHumdrumScotKeyItem (
      int inputLineNumber);
            
    virtual ~bsrHumdrumScotKeyItem ();

  public:

    // set and get
    // ------------------------------------------------------
/* JMI
    void                  setKeyItemDiatonicPitchKind (
                            bsrDiatonicPitchKind diatonicPitchKind);
                              
    bsrDiatonicPitchKind  getKeyItemDiatonicPitchKind () const
                              { return fKeyDiatonicPitchKind; }

    void                  setKeyItemAlterationKind (
                            bsrAlterationKind alterationKind);
                              
    bsrAlterationKind     getKeyItemAlterationKind () const
                              { return fKeyAlterationKind; }

    void                  setKeyItemOctave (int keyOctave);
                              
    int                   getKeyItemOctave () const
                              { return fKeyOctave; }

    // services
    // ------------------------------------------------------

    bool                  isEqualTo (
                            S_bsrHumdrumScotKeyItem
                              otherHumdrumScotKeyItem) const;
                            */
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
/* JMI
    bsrDiatonicPitchKind  fKeyDiatonicPitchKind;
    bsrAlterationKind     fKeyAlterationKind;
    int                   fKeyOctave;
    */
};
typedef SMARTP<bsrHumdrumScotKeyItem> S_bsrHumdrumScotKeyItem;
EXP ostream& operator<< (ostream& os, const S_bsrHumdrumScotKeyItem& elt);

//______________________________________________________________________________
class bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class bsrKey : public bsrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum bsrKeyKind {
        kTraditionalKind, kHumdrumScotKind };
        
    static string keyKindAsString (
      bsrKeyKind keyKind);
      
    // creation from MusicXML
    // ------------------------------------------------------
/* JMI
    static SMARTP<bsrKey> createTraditional (
      int                      inputLineNumber,
      bsrQuarterTonesPitchKind keyTonicPitchKind,
      bsrKeyModeKind           keyModeKind,
      int                      keyCancel);
      
    static SMARTP<bsrKey> createHumdrumScot (
      int                  inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrKey ( // for traditional keys
      int                      inputLineNumber,
      bsrQuarterTonesPitchKind keyTonicPitchKind,
      bsrKeyModeKind           keyModeKind,
      int                      keyCancel);
      
    bsrKey ( // for Humdrum/Scot keys
      int                  inputLineNumber);
      */
    virtual ~bsrKey ();

  public:

    // set and get
    // ------------------------------------------------------

    bsrKeyKind            getKeyKind () const
                              { return fKeyKind; }

    // traditional keys
    /*
    bsrQuarterTonesPitchKind
                          getKeyTonicQuarterTonesPitchKind () const
                              { return fKeyTonicQuarterTonesPitchKind; }
                              
    bsrKeyModeKind        getKeyModeKind () const
                              { return fKeyModeKind; }
                            
    int                   getKeyCancel () const
                              { return fKeyCancel; }

    // Humdrum/Scot keys

 //   void                  setKeyItemsOctavesAreSpecified ()
 //                           { fKeyItemsOctavesAreSpecified = true; }
                              
    bool                  getKeyItemsOctavesAreSpecified () const
                              { return fKeyItemsOctavesAreSpecified; }

    const vector<S_bsrHumdrumScotKeyItem>&
                          getHumdrumScotKeyItemsVector ()
                              { return fHumdrumScotKeyItemsVector; }

    
    // services
    // ------------------------------------------------------

    bool                  isEqualTo (S_bsrKey otherKey) const;
                            
    void                  appendHumdrumScotKeyItem (
                            S_bsrHumdrumScotKeyItem item);                                
*/
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

    bsrKeyKind            fKeyKind;
    
    // traditional keys
/* JMI
    bsrQuarterTonesPitchKind
                          fKeyTonicQuarterTonesPitchKind;
    bsrKeyModeKind        fKeyModeKind;
    int                   fKeyCancel;

    // Humdrum/Scot keys
    vector<S_bsrHumdrumScotKeyItem>
                          fHumdrumScotKeyItemsVector;
    bool                  fKeyItemsOctavesAreSpecified;
    */
};
typedef SMARTP<bsrKey> S_bsrKey;
EXP ostream& operator<< (ostream& os, const S_bsrKey& elt);


} // namespace MusicXML2


#endif
