/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2BsrOah___
#define ___msr2BsrOah___

#include "bsrOah.h"

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msr2BsrOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2BsrOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<msr2BsrOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2BsrOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2BsrOah (
      S_oahHandler handlerUpLink);

    virtual ~msr2BsrOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    void                  setAllMsr2BsrTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeMsr2BsrTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeMsr2BsrMiscellaneousOptions (
                            bool boolOptionsInitialValue);

    void                  initializeMsr2BsrExitAfterSomePassesOptions (
                            bool boolOptionsInitialValue);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2BsrOahHelp ();

    void                  printMsr2BsrOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // miscellaneous
    // --------------------------------------

    bool                  fNoBrailleMusicHeadings;

    bool                  fNoTempos;

    bool                  fNoPageNumbers;
    bool                  fNoLineNumbers;
    bool                  fNoMeasureNumbers;

    bool                  fNoBrailleLyrics;

    bool                  fBrailleCompileDate;

    bsrFacSimileKind      fFacSimileKind;

    bool                  fIncludeClefs;

    // exit after some passes

    bool                  fExit3a;
    bool                  fExit3b;
};
typedef SMARTP<msr2BsrOah> S_msr2BsrOah;
EXP ostream& operator<< (ostream& os, const S_msr2BsrOah& elt);

EXP extern S_msr2BsrOah gGlobalMsr2BsrOah;
EXP extern S_msr2BsrOah gGlobalMsr2BsrOahUserChoices;
EXP extern S_msr2BsrOah gGlobalMsr2BsrOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMsr2BsrOahHandling (
  S_oahHandler handler);


}


#endif