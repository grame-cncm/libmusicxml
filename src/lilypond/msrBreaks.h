/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrBreaks___
#define ___msrBreaks___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrLineBreak : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLineBreak> create (
      int    inputLineNumber,
      string nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrLineBreak (
      int    inputLineNumber,
      string nextBarNumber);
      
    virtual ~msrLineBreak ();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getNextBarNumber () const
                              { return fNextBarNumber; }

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

    string                fNextBarNumber;
};
typedef SMARTP<msrLineBreak> S_msrLineBreak;
EXP ostream& operator<< (ostream& os, const S_msrLineBreak& elt);

//______________________________________________________________________________
class msrPageBreak : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageBreak> create (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPageBreak (
      int inputLineNumber);
      
    virtual ~msrPageBreak ();
  
  public:

    // set and get
    // ------------------------------------------------------

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
};
typedef SMARTP<msrPageBreak> S_msrPageBreak;
EXP ostream& operator<< (ostream& os, const S_msrPageBreak& elt);


} // namespace MusicXML2


#endif
