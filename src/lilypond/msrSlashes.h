/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrSlashes___
#define ___msrSlashes___

#include "msrElements.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
class msrSlash : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlash> create (
      int                  inputLineNumber,
      msrSlashTypeKind     slashTypeKind,
      msrSlashUseDotsKind  slashUseDotsKind,
      msrSlashUseStemsKind slashUseStemsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSlash (
      int                  inputLineNumber,
      msrSlashTypeKind     slashTypeKind,
      msrSlashUseDotsKind  slashUseDotsKind,
      msrSlashUseStemsKind slashUseStemsKind);
      
    virtual ~msrSlash ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrSlashTypeKind      getSlashTypeKind () const
                              { return fSlashTypeKind; }

    msrSlashUseDotsKind   getSlashUseDotsKind () const
                              { return fSlashUseDotsKind; }

    msrSlashUseStemsKind  getSlashUseStemsKind () const
                              { return fSlashUseStemsKind; }

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
    
    msrSlashTypeKind      fSlashTypeKind;
    msrSlashUseDotsKind   fSlashUseDotsKind;
    msrSlashUseStemsKind  fSlashUseStemsKind;
};
typedef SMARTP<msrSlash> S_msrSlash;
EXP ostream& operator<< (ostream& os, const S_msrSlash& elt);


} // namespace MusicXML2


#endif
