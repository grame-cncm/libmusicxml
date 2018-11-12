/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrTimes___
#define ___bsrTimes___

#include "bsrElements.h"

#include "bsrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class bsrTimeItem;
typedef SMARTP<bsrTimeItem> S_bsrTimeItem;

class bsrTime;
typedef SMARTP<bsrTime> S_bsrTime;

//______________________________________________________________________________
class bsrTimeItem : public bsrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<bsrTimeItem> create (
      int inputLineNumber);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTimeItem (
      int inputLineNumber);
            
    virtual ~bsrTimeItem ();

  public:

    // set and get
    // ------------------------------------------------------

    const vector<int>&    getTimeBeatsNumbersVector ()
                              { return fTimeBeatsNumbersVector; }

    void                  setTimeBeatValue (int timeBeatValue)
                              { fTimeBeatValue = timeBeatValue; }
                              
    int                   getTimeBeatValue () const
                              { return fTimeBeatValue; }

    // services
    // ------------------------------------------------------

    bool                  isEqualTo (S_bsrTimeItem otherTimeItem) const;
                            
    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeBeatsNumber () const;
                              
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

    vector<int>           fTimeBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeBeatValue;
};
typedef SMARTP<bsrTimeItem> S_bsrTimeItem;
EXP ostream& operator<< (ostream& os, const S_bsrTimeItem& elt);

//______________________________________________________________________________
class bsrTime : public bsrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum bsrTimeSymbolKind {
        kTimeSymbolNone,
        kTimeSymbolCommon,
        kTimeSymbolCut,
        kTimeSymbolNote,
        kTimeSymbolDottedNote,
        kTimeSymbolSingleNumber,
        kTimeSymbolSenzaMisura };

    static string timeSymbolKindAsString (
      bsrTimeSymbolKind timeSymbolKind);
      
    enum bsrTimeSeparatorKind {
        kTimeSeparatorNone,
        kTimeSeparatorHorizontal,
        kTimeSeparatorDiagonal,
        kTimeSeparatorVertical,
        kTimeSeparatorAdjacent };

    static string timeSeparatorKindAsString (
      bsrTimeSeparatorKind timeSeparatorKind);
      
    enum bsrTimeRelationKind {
        kTimeRelationNone,
        kTimeRelationParentheses,
        kTimeRelationBracket,
        kTimeRelationEquals,
        kTimeRelationSlash,
        kTimeRelationSpace,
        kTimeRelationHyphen };

    static string timeRelationKindAsString (
      bsrTimeRelationKind timeRelationKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<bsrTime> create (
      int               inputLineNumber,
      bsrTimeSymbolKind timeSymbolKind);

    static SMARTP<bsrTime> createFourQuartersTime (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    bsrTime (
      int               inputLineNumber,
      bsrTimeSymbolKind timeSymbolKind);
      
    virtual ~bsrTime ();
  
  public:

    // set and get
    // ------------------------------------------------------

    bsrTimeSymbolKind     getTimeSymbolKind () const
                              { return fTimeSymbolKind; }
                  
    bool                  getTimeIsCompound () const
                              { return fTimeIsCompound; }
                  
    const vector<S_bsrTimeItem>&
                          getTimeItemsVector ()
                              { return fTimeItemsVector; }

    // services
    // ------------------------------------------------------
                  
    bool                  isEqualTo (S_bsrTime otherTime) const;
                            
    void                  appendTimeItem (
                            S_bsrTimeItem timeItem);

    rational              wholeNotesPerMeasure () const;

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

    string                asShortString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    bsrTimeSymbolKind     fTimeSymbolKind;

    vector<S_bsrTimeItem> fTimeItemsVector;

    // a time is compound if it contains several items
    // or if the only one has several beats numbers
    // i.e. 3/4 is not, (3+4)/8 is, and 2/4+3/4 is too
    bool                  fTimeIsCompound;
};
typedef SMARTP<bsrTime> S_bsrTime;
EXP ostream& operator<< (ostream& os, const S_bsrTime& elt);


} // namespace MusicXML2


#endif
