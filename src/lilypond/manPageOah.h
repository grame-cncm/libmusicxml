/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___manPageOah___
#define ___manPageOah___

#include "exports.h"

#include "oahBasicTypes.h"

namespace MusicXML2
{

//______________________________________________________________________________
class manPageGenerateAtom : public oahAtom
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<manPageGenerateAtom> create (
      string shortName,
      string longName,
      string description);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    manPageGenerateAtom (
      string shortName,
      string longName,
      string description);

    virtual ~manPageGenerateAtom ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
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

  public:

    // print
    // ------------------------------------------------------

    void                  print (ostream& os) const;

    void                  printAtomOptionsValues (
                            ostream& os,
                            int      valueFieldWidth) const;

  private:

    // private services
    // ------------------------------------------------------

    void                  generateManPageData (ostream& os) const;

  private:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<manPageGenerateAtom> S_manPageGenerateAtom;
EXP ostream& operator<< (ostream& os, const S_manPageGenerateAtom& elt);

//______________________________________________________________________________
class manPageOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<manPageOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<manPageOah>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeManPageOah (
                            bool boolOptionsInitialValue);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    manPageOah (
      S_oahHandler handlerUpLink);

    virtual ~manPageOah ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

    void                  initializeManPageDisplayOptions (
                            bool boolOptionsInitialValue);

#ifdef TRACE_OAH
    void                  initializeManPageTraceOah (
                            bool boolOptionsInitialValue);
#endif

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    void                  printManPageOahHelp ();

    void                  printManPageOahValues (int fieldWidth);

  public:

/* JMI
    // display
    // --------------------------------------

    bool                  fDisplayOah;

    // trace
    // --------------------------------------

*/
};
typedef SMARTP<manPageOah> S_manPageOah;
EXP ostream& operator<< (ostream& os, const S_manPageOah& elt);

EXP extern S_manPageOah gManPageOah;
EXP extern S_manPageOah gManPageOahUserChoices;
EXP extern S_manPageOah gManPageOahWithDetailedTrace;

//______________________________________________________________________________
EXP  void initializeManPageOahHandling (
  S_oahHandler handler);


}


#endif
