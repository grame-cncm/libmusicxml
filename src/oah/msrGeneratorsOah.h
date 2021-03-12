/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrGeneratorsOah___
#define ___msrGeneratorsOah___

#include "msr.h"

#include "msrGeneratorsBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msrGeneratorsOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msrGeneratorsOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrGeneratorsOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msrGeneratorsOahGroup ();

    virtual               ~msrGeneratorsOahGroup ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceGroupQuietness () override;

  public:

    // consistency check
    // ------------------------------------------------------

    void                  checkGroupOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    // fields
    // ------------------------------------------------------

    // generaton API kind

    void                  setGenerationAPIKind (msrGenerationAPIKind value)
                              { fGenerationAPIKind = value; }

    msrGenerationAPIKind  getGenerationAPIKind () const
                              { return fGenerationAPIKind;  }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeMsrGeneratorsTraceOah ();
#endif

    void                  initializeGenerationAPIOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsrGeneratorsOahHelp ();

    void                  printMsrGeneratorsOahValues (
                            unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // generation API kind

    msrGenerationAPIKind  fGenerationAPIKind;
    S_msrGenerationAPIKindAtom
                          fGenerationAPIKindAtom;
};
typedef SMARTP<msrGeneratorsOahGroup> S_msrGeneratorsOahGroup;
EXP ostream& operator<< (ostream& os, const S_msrGeneratorsOahGroup& elt);

EXP extern S_msrGeneratorsOahGroup gGlobalMsrGeneratorsOahGroup;

//______________________________________________________________________________
EXP S_msrGeneratorsOahGroup createGlobalMsrGeneratorsOahGroup ();


}


#endif
