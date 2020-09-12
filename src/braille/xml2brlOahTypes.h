/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2brlOahTypes___
#define ___xml2brlOahTypes___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2brlVersionOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlVersionOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlVersionOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2brlVersionOahAtom ();

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

  public:

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
typedef SMARTP<xml2brlVersionOahAtom> S_xml2brlVersionOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2brlVersionOahAtom& elt);

//______________________________________________________________________________
class xml2brlAboutOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlAboutOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlAboutOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2brlAboutOahAtom ();

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

  public:

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
typedef SMARTP<xml2brlAboutOahAtom> S_xml2brlAboutOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2brlAboutOahAtom& elt);

//______________________________________________________________________________
class xml2brlContactOahAtom : public oahHelpAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2brlContactOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2brlContactOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2brlContactOahAtom ();

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

  public:

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
typedef SMARTP<xml2brlContactOahAtom> S_xml2brlContactOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2brlContactOahAtom& elt);


}


#endif
