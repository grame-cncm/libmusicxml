/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyOah2ManPageGenerator___
#define ___xml2lyOah2ManPageGenerator___

#include "basicOah2ManPageGenerator.h"

#include "msrOah.h"


namespace MusicXML2
{

//________________________________________________________________________
class xml2lyOah2ManPageGenerator : public basicOah2ManPageGenerator,

  public visitor<S_msrRenamePartAtom>,

  public visitor<S_msrTransposePartAtom>,

  public visitor<S_msrOmitPartAtom>,

  public visitor<S_msrKeepPartAtom>,

  public visitor<S_msrPitchesLanguageAtom>

{
  public:

    xml2lyOah2ManPageGenerator (
      const S_oahHandler handler,
      S_manPageOah&      manPageOpts,
      indentedOstream&   logOstream,
      ostream&            manPageOutputStream);

    virtual ~xml2lyOah2ManPageGenerator ();

    void generateManPageFromOahHandler ();

  protected:

    virtual void visitStart (S_msrRenamePartAtom& elt);

    virtual void visitStart (S_msrTransposePartAtom& elt);

    virtual void visitStart (S_msrOmitPartAtom& elt);

    virtual void visitStart (S_msrKeepPartAtom& elt);

    virtual void visitStart (S_msrPitchesLanguageAtom& elt);


  private:

};


}


#endif
