/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2mxmlTreeOah___
#define ___msr2mxmlTreeOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP msr2mxmlTreeOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<msr2mxmlTreeOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsr2mxmlTreeOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msr2mxmlTreeOahGroup ();

    virtual ~msr2mxmlTreeOahGroup ();

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

    bool                  getMusicXMLComments () const
                              { return fMusicXMLComments; }

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
// JMI    void                  initializeMsr2mxmlTreeTraceOah ();
#endif

    void                  initializeCodeGenerationOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMsr2mxmlTreeOahHelp ();

    void                  printMsr2mxmlTreeOahValues (int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // code generation
    // --------------------------------------

    bool                  fXml2xmlInfos;

    bool                  fMusicXMLComments;
};
typedef SMARTP<msr2mxmlTreeOahGroup> S_msr2mxmlTreeOahGroup;
EXP ostream& operator<< (ostream& os, const S_msr2mxmlTreeOahGroup& elt);

EXP extern S_msr2mxmlTreeOahGroup gGlobalMsr2mxmlTreeOahGroup;

//______________________________________________________________________________
EXP S_msr2mxmlTreeOahGroup createGlobalMsr2mxmlTreeOahGroup ();


}


#endif
