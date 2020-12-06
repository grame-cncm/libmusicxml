/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2bsrOah___
#define ___msr2bsrOah___

#include "bsrOah.h"

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msr2bsrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2bsrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2bsrOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2bsrOahGroup ();

    virtual ~msr2bsrOahGroup ();

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

    // miscellaneous
    bool                  getNoBrailleMusicHeadings () const
                              { return fNoBrailleMusicHeadings; }

    bool                  getNoTempos () const
                              { return fNoTempos; }

    bool                  getNoPageNumbers () const
                              { return fNoPageNumbers; }
    bool                  getNoLineNumbers () const
                              { return fNoLineNumbers; }
    bool                  getNoMeasureNumbers () const
                              { return fNoMeasureNumbers; }

    bool                  getNoBrailleLyrics () const
                              { return fNoBrailleLyrics; }

    bool                  getBrailleCompileDate () const
                              { return fBrailleCompileDate; }

    bsrFacSimileKind      getFacSimileKind () const
                              { return fFacSimileKind; }

    bool                  getIncludeClefs () const
                              { return fIncludeClefs; }

    // quit after some passes
    bool                  getQuit3a () const
                              { return fQuitAfterPass3a; }
    bool                  getQuit3b () const
                              { return fQuitAfterPass3b; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeMsr2bsrTraceOahGroup ();
#endif

    void                  initializeMsr2bsrMiscellaneousOptions ();

    void                  initializeMsr2bsrQuitAfterSomePassesOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2bsrOahHelp ();

    void                  printMsr2bsrOahValues (int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // miscellaneous
    bool                  fNoBrailleMusicHeadings;

    bool                  fNoTempos;

    bool                  fNoPageNumbers;
    bool                  fNoLineNumbers;
    bool                  fNoMeasureNumbers;

    bool                  fNoBrailleLyrics;

    bool                  fBrailleCompileDate;

    bsrFacSimileKind      fFacSimileKind;

    bool                  fIncludeClefs;

    // quit after some passes
    bool                  fQuitAfterPass3a;
    bool                  fQuitAfterPass3b;
};
typedef SMARTP<msr2bsrOahGroup> S_msr2bsrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2bsrOahGroup& elt);

EXP extern S_msr2bsrOahGroup gGlobalMsr2bsrOahGroup;

//______________________________________________________________________________
S_msr2bsrOahGroup createGlobalMsr2bsrOahGroup ();


}


#endif
