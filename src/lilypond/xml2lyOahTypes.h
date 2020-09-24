/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyOahTypes___
#define ___xml2lyOahTypes___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2lyVersionOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyVersionOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyVersionOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyVersionOahAtom ();

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
typedef SMARTP<xml2lyVersionOahAtom> S_xml2lyVersionOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyVersionOahAtom& elt);

//______________________________________________________________________________
class xml2lyInsiderOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyInsiderOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyInsiderOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyInsiderOahAtom ();

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
typedef SMARTP<xml2lyInsiderOahAtom> S_xml2lyInsiderOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyInsiderOahAtom& elt);

//______________________________________________________________________________
class xml2lyRegularOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyRegularOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyRegularOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyRegularOahAtom ();

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
typedef SMARTP<xml2lyRegularOahAtom> S_xml2lyRegularOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyRegularOahAtom& elt);

//______________________________________________________________________________
class xml2lyAboutOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyAboutOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyAboutOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyAboutOahAtom ();

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
typedef SMARTP<xml2lyAboutOahAtom> S_xml2lyAboutOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyAboutOahAtom& elt);

//______________________________________________________________________________
class xml2lyContactOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyContactOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyContactOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyContactOahAtom ();

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
typedef SMARTP<xml2lyContactOahAtom> S_xml2lyContactOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyContactOahAtom& elt);


}


#endif
