/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrTies___
#define ___msrTies___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrTie : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrTieKind {
      kTieNone,
      kTieStart, kTieContinue, kTieStop};        
    
    static string tieKindAsString (
      msrTieKind tieKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTie> create (
      int        inputLineNumber,
      msrTieKind tieKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTie (
      int        inputLineNumber,
      msrTieKind tieKind);
      
    virtual ~msrTie ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTieKind            getTieKind () const
                              { return fTieKind; }

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

    string                tieKindAsString () const
                              { return tieKindAsString (fTieKind); }

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrTieKind            fTieKind;
};
typedef SMARTP<msrTie> S_msrTie;
EXP ostream& operator<< (ostream& os, const S_msrTie& elt);


} // namespace MusicXML2


#endif
