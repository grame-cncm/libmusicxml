/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrClefs___
#define ___msrClefs___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrClef;
typedef SMARTP<msrClef> S_msrClef;

//______________________________________________________________________________
class msrClef : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrClefKind {
      k_NoClef,
      kTrebleClef,
      kSopranoClef, kMezzoSopranoClef, kAltoClef, kTenorClef, kBaritoneClef,
      kBassClef,
      kTrebleLine1Clef,
      kTrebleMinus15Clef, kTrebleMinus8Clef,
      kTreblePlus8Clef, kTreblePlus15Clef, 
      kBassMinus15Clef, kBassMinus8Clef,
      kBassPlus8Clef, kBassPlus15Clef,
      kVarbaritoneClef,
      kTablature4Clef, kTablature5Clef, kTablature6Clef, kTablature7Clef,
      kPercussionClef,
      kJianpuClef};
      
    static string clefKindAsString (
      msrClefKind clefKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrClef> create (
      int         inputLineNumber,
      msrClefKind clefKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrClef (
      int         inputLineNumber,
      msrClefKind clefKind);
      
    virtual ~msrClef ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrClefKind           getClefKind () const
                              { return fClefKind; }
                
    // services
    // ------------------------------------------------------

    bool                  isEqualTo (S_msrClef otherClef) const;

    bool                  clefIsATablatureClef () const;
    
    bool                  clefIsAPercussionClef () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    static msrClefKind    clefKindFromString (
                            string theString);
                            
    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrClefKind           fClefKind;
};
typedef SMARTP<msrClef> S_msrClef;
EXP ostream& operator<< (ostream& os, const S_msrClef& elt);


} // namespace MusicXML2


#endif
