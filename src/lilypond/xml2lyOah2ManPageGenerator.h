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

#include "xml2lyOahTypes.h"


namespace MusicXML2
{

//________________________________________________________________________
class xml2lyOah2ManPageGenerator : public oahVisitor, basicOah2ManPageGenerator,

  public visitor<S_xml2lyVersionOahAtom>,

  public visitor<S_xml2lyAboutOahAtom>,

  public visitor<S_xml2lyContactOahAtom>

{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyOah2ManPageGenerator> create (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyOah2ManPageGenerator (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

    virtual ~xml2lyOah2ManPageGenerator ();

  protected:

    // protected services
    // ------------------------------------------------------

    virtual void visitStart (S_xml2lyVersionOahAtom& elt);

    virtual void visitStart (S_xml2lyContactOahAtom& elt);

    virtual void visitStart (S_xml2lyAboutOahAtom& elt);

  public:

    // public services
    // ------------------------------------------------------

    virtual void          visitTheHandler ();
};
typedef SMARTP<xml2lyOah2ManPageGenerator> S_xml2lyOah2ManPageGenerator;


}


#endif
