/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2meiOahTypes___
#define ___xml2meiOahTypes___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2meiVersionOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2meiVersionOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2meiVersionOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2meiVersionOahAtom ();

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
typedef SMARTP<xml2meiVersionOahAtom> S_xml2meiVersionOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2meiVersionOahAtom& elt);

//______________________________________________________________________________
class xml2meiAboutOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2meiAboutOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2meiAboutOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2meiAboutOahAtom ();

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
typedef SMARTP<xml2meiAboutOahAtom> S_xml2meiAboutOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2meiAboutOahAtom& elt);

//______________________________________________________________________________
class xml2meiContactOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2meiContactOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2meiContactOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2meiContactOahAtom ();

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
typedef SMARTP<xml2meiContactOahAtom> S_xml2meiContactOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2meiContactOahAtom& elt);


}


#endif
