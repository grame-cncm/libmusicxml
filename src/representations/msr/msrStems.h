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
class EXP msrStem : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrStemKind {
      kStemNeutral,
      kStemUp, kStemDown, kStemDouble
    };

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

    virtual               ~msrStem ();

  public:

    // set and get
    // ------------------------------------------------------

    msrStemKind           getStemKind () const
                              { return fStemKind; }

  public:

    // public services
    // ------------------------------------------------------

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

    msrStemKind           fStemKind;
};
typedef SMARTP<msrStem> S_msrStem;
EXP ostream& operator<< (ostream& os, const S_msrStem& elt);


} // namespace MusicXML2


#endif
