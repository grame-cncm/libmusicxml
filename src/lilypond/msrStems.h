/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrStems___
#define ___msrStems___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrStem : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrStemKind {
      kStemNone,
      kStemUp, kStemDown, kStemDouble};

    static string stemKindAsString (
      msrStemKind stemKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStem> create (
      int         inputLineNumber,
      msrStemKind stemKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStem (
      int         inputLineNumber,
      msrStemKind stemKind);
      
    virtual ~msrStem ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrStemKind           getStemKind () const
                              { return fStemKind; }

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

    msrStemKind           fStemKind;
};
typedef SMARTP<msrStem> S_msrStem;
EXP ostream& operator<< (ostream& os, const S_msrStem& elt);


} // namespace MusicXML2


#endif
