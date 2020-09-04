/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2bmmlOah2ManPageGenerator___
#define ___xml2bmmlOah2ManPageGenerator___

#include "basicOah2ManPageGenerator.h"
#include "msrOah2ManPageGenerator.h"
#include "lpsrOah2ManPageGenerator.h"
#include "lilypondOah2ManPageGenerator.h"
#include "extraOah2ManPageGenerator.h"
#include "oahOah2ManPageGenerator.h"

#include "xml2bmmlOahTypes.h"


namespace MusicXML2
{

//________________________________________________________________________
class xml2bmmlOah2ManPageGenerator :

  // aggregate needed components' visitors
  virtual public basicOah2ManPageGenerator,
  virtual public msrOah2ManPageGenerator,
  virtual public lpsrOah2ManPageGenerator,
  virtual public lilypondOah2ManPageGenerator,
  virtual public extraOah2ManPageGenerator,
  virtual public oahOah2ManPageGenerator,

  // this man page generator's own visitors
  public visitor<S_xml2bmmlVersionOahAtom>,

  public visitor<S_xml2bmmlAboutOahAtom>,

  public visitor<S_xml2bmmlContactOahAtom>

{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2bmmlOah2ManPageGenerator> create (
      const S_oahHandler handler,
      ostream&           logOstream,
      ostream&           manPageOutputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlOah2ManPageGenerator (
      const S_oahHandler handler,
      ostream&           logOstream,
      ostream&           manPageOutputStream);

    virtual ~xml2bmmlOah2ManPageGenerator ();

  protected:

    // protected services
    // ------------------------------------------------------

    virtual void visitStart (S_xml2bmmlVersionOahAtom& elt);
    virtual void visitEnd   (S_xml2bmmlVersionOahAtom& elt);

    virtual void visitStart (S_xml2bmmlContactOahAtom& elt);
    virtual void visitEnd   (S_xml2bmmlContactOahAtom& elt);

    virtual void visitStart (S_xml2bmmlAboutOahAtom& elt);
    virtual void visitEnd   (S_xml2bmmlAboutOahAtom& elt);

  public:

    // public services
    // ------------------------------------------------------

    virtual void          visitTheHandler ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2bmmlOah2ManPageGenerator> S_xml2bmmlOah2ManPageGenerator;


}


#endif
