/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdl2msdrOah___
#define ___msdl2msdrOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msdl2msdrOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdl2msdrOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdl2msdr ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdl2msdrOahGroup ();

    virtual               ~msdl2msdrOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    bool                  getTraceTokens () const
                              { return fTraceTokens; }
#endif

    bool                  getQuitOnLexicalErrors () const
                              { return fQuitOnLexicalErrors; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMsdl2msdrTraceOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsdl2msdrHelp ();

    void                  printMsdl2msdrValues (unsigned int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // trace
#ifdef TRACING_IS_ENABLED
    bool                  fTraceTokens;
#endif

    bool                  fQuitOnLexicalErrors;
};
typedef SMARTP<msdl2msdrOahGroup> S_msdl2msdrOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdl2msdrOahGroup& elt);

EXP extern S_msdl2msdrOahGroup gGlobalMsdl2msdrOahGroup;

//______________________________________________________________________________
EXP S_msdl2msdrOahGroup createGlobalMsdl2msdrOahGroup ();


}


#endif
