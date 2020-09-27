/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2xmlOahTypes___
#define ___xml2xmlOahTypes___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2xmlVersionOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlVersionOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlVersionOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2xmlVersionOahAtom ();

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

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2xmlVersionOahAtom> S_xml2xmlVersionOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2xmlVersionOahAtom& elt);

//______________________________________________________________________________
class xml2xmlInsiderOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlInsiderOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlInsiderOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2xmlInsiderOahAtom ();

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

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2xmlInsiderOahAtom> S_xml2xmlInsiderOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2xmlInsiderOahAtom& elt);

//______________________________________________________________________________
class xml2xmlRegularOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlRegularOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlRegularOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2xmlRegularOahAtom ();

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

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2xmlRegularOahAtom> S_xml2xmlRegularOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2xmlRegularOahAtom& elt);

//______________________________________________________________________________
class xml2xmlAboutOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlAboutOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlAboutOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2xmlAboutOahAtom ();

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

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2xmlAboutOahAtom> S_xml2xmlAboutOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2xmlAboutOahAtom& elt);

//______________________________________________________________________________
class xml2xmlContactOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2xmlContactOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2xmlContactOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2xmlContactOahAtom ();

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

    void                  printValuedAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const override;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2xmlContactOahAtom> S_xml2xmlContactOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2xmlContactOahAtom& elt);


}


#endif
