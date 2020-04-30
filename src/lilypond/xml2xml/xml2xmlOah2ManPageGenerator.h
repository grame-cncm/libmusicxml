/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlOah2ManPageGenerator___
#define ___xml2xmlOah2ManPageGenerator___

#include "basicOah2ManPageGenerator.h"
#include "msrOah2ManPageGenerator.h"
#include "lpsrOah2ManPageGenerator.h"
#include "lilypondOah2ManPageGenerator.h"
#include "extraOah2ManPageGenerator.h"
#include "oahOah2ManPageGenerator.h"

#include "xml2xmlOahTypes.h"


namespace MusicXML2
{

//________________________________________________________________________
class xml2xmlOah2ManPageGenerator :

  // aggregate needed components' visitors
  virtual public basicOah2ManPageGenerator,
  virtual public msrOah2ManPageGenerator,
  virtual public lpsrOah2ManPageGenerator,
  virtual public lilypondOah2ManPageGenerator,
  virtual public extraOah2ManPageGenerator,
  virtual public oahOah2ManPageGenerator,

  // this man page generator's own visitors
  public visitor<S_xml2xmlVersionOahAtom>,

  public visitor<S_xml2xmlAboutOahAtom>,

  public visitor<S_xml2xmlContactOahAtom>

{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlOah2ManPageGenerator> create (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlOah2ManPageGenerator (
      const S_oahHandler handler,
      indentedOstream&   logOstream,
      ostream&           manPageOutputStream);

    virtual ~xml2xmlOah2ManPageGenerator ();

  protected:

    // protected services
    // ------------------------------------------------------

    virtual void visitStart (S_xml2xmlVersionOahAtom& elt);
    virtual void visitEnd   (S_xml2xmlVersionOahAtom& elt);

    virtual void visitStart (S_xml2xmlContactOahAtom& elt);
    virtual void visitEnd   (S_xml2xmlContactOahAtom& elt);

    virtual void visitStart (S_xml2xmlAboutOahAtom& elt);
    virtual void visitEnd   (S_xml2xmlAboutOahAtom& elt);

  public:

    // public services
    // ------------------------------------------------------

    virtual void          visitTheHandler ();

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2xmlOah2ManPageGenerator> S_xml2xmlOah2ManPageGenerator;


}


#endif
