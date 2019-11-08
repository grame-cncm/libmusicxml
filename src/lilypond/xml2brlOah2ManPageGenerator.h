/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2brlOah2ManPageGenerator___
#define ___xml2brlOah2ManPageGenerator___

#include "basicOah2ManPageGenerator.h"
#include "msrOah2ManPageGenerator.h"
#include "bsrOah2ManPageGenerator.h"
#include "brailleOah2ManPageGenerator.h"
#include "extraOah2ManPageGenerator.h"
#include "oahOah2ManPageGenerator.h"

#include "xml2brlOahTypes.h"


namespace MusicXML2
{

//________________________________________________________________________
class xml2brlOah2ManPageGenerator :

  // aggregate needed components' visitors
  public basicOah2ManPageGenerator,
  public msrOah2ManPageGenerator,
  public bsrOah2ManPageGenerator,
  public brailleOah2ManPageGenerator,
  public extraOah2ManPageGenerator,
  public oahOah2ManPageGenerator,

  // this man page generator's own visitors
  public visitor<S_xml2brlVersionOahAtom>,

  public visitor<S_xml2brlAboutOahAtom>,

  public visitor<S_xml2brlContactOahAtom>

{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlOah2ManPageGenerator> create (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlOah2ManPageGenerator (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

    virtual ~xml2brlOah2ManPageGenerator ();

  protected:

    // protected services
    // ------------------------------------------------------

    virtual void visitStart (S_xml2brlVersionOahAtom& elt);
    virtual void visitEnd   (S_xml2brlVersionOahAtom& elt);

    virtual void visitStart (S_xml2brlContactOahAtom& elt);
    virtual void visitEnd   (S_xml2brlContactOahAtom& elt);

    virtual void visitStart (S_xml2brlAboutOahAtom& elt);
    virtual void visitEnd   (S_xml2brlAboutOahAtom& elt);

  public:

    // public services
    // ------------------------------------------------------

    virtual void          visitTheHandler ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2brlOah2ManPageGenerator> S_xml2brlOah2ManPageGenerator;


}


#endif
