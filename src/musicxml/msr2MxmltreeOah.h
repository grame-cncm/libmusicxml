/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2MxmltreeOah___
#define ___msr2MxmltreeOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class msr2MxmltreeOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2MxmltreeOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<msr2MxmltreeOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2MxmltreeOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2MxmltreeOah (
      S_oahHandler handlerUpLink);

    virtual ~msr2MxmltreeOah ();

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
    void                  initializeMsr2MxmlTreeTraceOah (
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

    void                  printMsr2MxmltreeOahHelp ();

    void                  printMsr2MxmltreeOahValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------

    // code generation
    // --------------------------------------

    bool                  fMusicXMLComments;
};
typedef SMARTP<msr2MxmltreeOah> S_msr2MxmltreeOah;
EXP ostream& operator<< (ostream& os, const S_msr2MxmltreeOah& elt);

EXP extern S_msr2MxmltreeOah gGlobalMsr2MxmltreeOah;
EXP extern S_msr2MxmltreeOah gGlobalMsr2MxmltreeOahUserChoices;
EXP extern S_msr2MxmltreeOah gGlobalMsr2MxmltreeOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMsr2MxmltreeOahHandler (
  S_oahHandler handler);


}


#endif
