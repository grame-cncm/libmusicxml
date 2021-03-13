/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msdlCompilerOah___
#define ___msdlCompilerOah___

#include "msr.h"

#include "msrGenerationBasicTypes.h"

#include "brailleGenerationOah.h"
#include "bsr2brailleCodeGenerators.h"


namespace MusicXML2
{

//______________________________________________________________________________
#define MSDR_STANDARD_INPUT_NAME ""
//#define MSDR_STANDARD_INPUT_NAME "-"

//______________________________________________________________________________
class EXP msdlCompilerOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msdlCompilerOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsdlCompilerOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          msdlCompilerOahGroup ();

    virtual               ~msdlCompilerOahGroup ();

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

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeMsdlCompilerTraceOah ();
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

    void                  printMsdlCompilerOahHelp ();

    void                  printMsdlCompilerOahValues (
                            unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<msdlCompilerOahGroup> S_msdlCompilerOahGroup;
EXP ostream& operator<< (ostream& os, const S_msdlCompilerOahGroup& elt);

EXP extern S_msdlCompilerOahGroup gGlobalMsdlCompilerOahGroup;

//______________________________________________________________________________
EXP S_msdlCompilerOahGroup createGlobalMsdlCompilerOahGroup ();


}


#endif
