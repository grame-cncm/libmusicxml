/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___extraOah2ManPageGenerator___
#define ___extraOah2ManPageGenerator___

#include "extraOah.h"

#include "oah2ManPageGenerators.h"


namespace MusicXML2
{

//________________________________________________________________________
class extraOah2ManPageGenerator : virtual public oah2ManPageGenerator,

  public visitor<S_extraShowAllChordsStructuresAtom>,

  public visitor<S_extraShowAllChordsContentsAtom>,

  public visitor<S_extraShowChordDetailsAtom>,

  public visitor<S_extraShowChordAnalysisAtom>

{
  public:

    extraOah2ManPageGenerator (
      const S_oahHandler handler,
      ostream&           logOstream,
      ostream&           manPageOutputStream);

    virtual ~extraOah2ManPageGenerator ();

  protected:

    virtual void visitStart (S_extraShowAllChordsStructuresAtom& elt);
    virtual void visitEnd   (S_extraShowAllChordsStructuresAtom& elt);

    virtual void visitStart (S_extraShowAllChordsContentsAtom& elt);
    virtual void visitEnd   (S_extraShowAllChordsContentsAtom& elt);

    virtual void visitStart (S_extraShowChordDetailsAtom& elt);
    virtual void visitEnd   (S_extraShowChordDetailsAtom& elt);

    virtual void visitStart (S_extraShowChordAnalysisAtom& elt);
    virtual void visitEnd   (S_extraShowChordAnalysisAtom& elt);
};
typedef SMARTP<extraOah2ManPageGenerator> S_extraOah2ManPageGenerator;


}


#endif
