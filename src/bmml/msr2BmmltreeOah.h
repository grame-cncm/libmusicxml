/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2BmmltreeOah___
#define ___msr2BmmltreeOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msr2BmmltreeOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2BmmltreeOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<msr2BmmltreeOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2BmmltreeOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2BmmltreeOah (
      S_oahHandler handlerUpLink);

    virtual ~msr2BmmltreeOah ();

  public:

    // quiet mode
    // ------------------------------------------------------

    void                  enforceQuietness ();

  public:

    // consistency check
    // ------------------------------------------------------

    virtual void          checkOptionsConsistency () override;

  public:

    // set and get
    // ------------------------------------------------------

    void                  setAllMusicXMLTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeMsr2BmmlTreeTraceOah (
                            bool boolOptionsInitialValue);
#endif

    void                  initializeCodeGenerationOptions (
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

    void                  printMsr2BmmltreeOahHelp ();

    void                  printMsr2BmmltreeOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // code generation
    // --------------------------------------

    bool                  fMusicXMLComments;
};
typedef SMARTP<msr2BmmltreeOah> S_msr2BmmltreeOah;
EXP ostream& operator<< (ostream& os, const S_msr2BmmltreeOah& elt);

EXP extern S_msr2BmmltreeOah gMsr2BmmltreeOah;
EXP extern S_msr2BmmltreeOah gMsr2BmmltreeOahUserChoices;
EXP extern S_msr2BmmltreeOah gMsr2BmmltreeOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMsr2BmmltreeOahHandling (
  S_oahHandler handler);


}


#endif
