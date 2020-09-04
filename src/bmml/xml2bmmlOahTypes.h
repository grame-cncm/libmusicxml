/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2bmmlOahTypes___
#define ___xml2bmmlOahTypes___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2bmmlVersionOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2bmmlVersionOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlVersionOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2bmmlVersionOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printVersion (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2bmmlVersionOahAtom> S_xml2bmmlVersionOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2bmmlVersionOahAtom& elt);

//______________________________________________________________________________
class xml2bmmlAboutOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2bmmlAboutOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlAboutOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2bmmlAboutOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printAbout (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2bmmlAboutOahAtom> S_xml2bmmlAboutOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2bmmlAboutOahAtom& elt);

//______________________________________________________________________________
class xml2bmmlContactOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2bmmlContactOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2bmmlContactOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2bmmlContactOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os) override;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const override;

    void                  printContact (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2bmmlContactOahAtom> S_xml2bmmlContactOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2bmmlContactOahAtom& elt);


}


#endif
