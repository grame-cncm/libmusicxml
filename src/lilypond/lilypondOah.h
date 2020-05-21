/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lilypondOah___
#define ___lilypondOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{
//______________________________________________________________________________
class lilypondOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<lilypondOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilypondOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondOah (
      S_oahHandler handlerUpLink);

    virtual ~lilypondOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setAlllilypondOahTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializelilypondOahTraceOah (
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

    void                  printLilypondOahHelp ();

    void                  printLilypondOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<lilypondOah> S_lilypondOah;
EXP ostream& operator<< (ostream& os, const S_lilypondOah& elt);

EXP extern S_lilypondOah gLilypondOah;
EXP extern S_lilypondOah gLilypondOahUserChoices;
EXP extern S_lilypondOah gLilypondOahWithDetailedTrace;

//______________________________________________________________________________
void initializeLilypondOahHandling (
  S_oahHandler handler);


}


#endif
