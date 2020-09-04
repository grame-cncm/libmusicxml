/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2MeitreeOah___
#define ___msr2MeitreeOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msr2MeitreeOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2MeitreeOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<msr2MeitreeOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2MeitreeOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2MeitreeOah (
      S_oahHandler handlerUpLink);

    virtual ~msr2MeitreeOah ();

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
    void                  initializeMsr2MeiTreeTraceOah (
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

    void                  printMsr2MeitreeOahHelp ();

    void                  printMsr2MeitreeOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // code generation
    // --------------------------------------

    bool                  fMusicXMLComments;
};
typedef SMARTP<msr2MeitreeOah> S_msr2MeitreeOah;
EXP ostream& operator<< (ostream& os, const S_msr2MeitreeOah& elt);

EXP extern S_msr2MeitreeOah gMsr2MeitreeOah;
EXP extern S_msr2MeitreeOah gMsr2MeitreeOahUserChoices;
EXP extern S_msr2MeitreeOah gMsr2MeitreeOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMsr2MeitreeOahHandling (
  S_oahHandler handler);


}


#endif
