/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___xml2lyWithOptionsVectorOahTypes___
#define ___xml2lyWithOptionsVectorOahTypes___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class xml2lyWithOptionsVectorVersionOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyWithOptionsVectorVersionOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyWithOptionsVectorVersionOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyWithOptionsVectorVersionOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printVersion (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyWithOptionsVectorVersionOahAtom> S_xml2lyWithOptionsVectorVersionOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorVersionOahAtom& elt);

//______________________________________________________________________________
class xml2lyWithOptionsVectorAboutOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyWithOptionsVectorAboutOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyWithOptionsVectorAboutOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyWithOptionsVectorAboutOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAbout (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyWithOptionsVectorAboutOahAtom> S_xml2lyWithOptionsVectorAboutOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorAboutOahAtom& elt);

//______________________________________________________________________________
class xml2lyWithOptionsVectorContactOahAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<xml2lyWithOptionsVectorContactOahAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xml2lyWithOptionsVectorContactOahAtom (
      string shortName,
      string longName,
      string description);

    virtual ~xml2lyWithOptionsVectorContactOahAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // services
    // ------------------------------------------------------

    S_oahValuedAtom       handleOptionUnderName (
                            string   optionName,
                            ostream& os);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printContact (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<xml2lyWithOptionsVectorContactOahAtom> S_xml2lyWithOptionsVectorContactOahAtom;
EXP ostream& operator<< (ostream& os, const S_xml2lyWithOptionsVectorContactOahAtom& elt);


}


#endif
