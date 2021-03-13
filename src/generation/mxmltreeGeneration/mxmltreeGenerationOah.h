/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mxmltreeGenerationOah___
#define ___mxmltreeGenerationOah___


#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP mxmltreeGenerationOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxmltreeGenerationOahGroup> create ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          mxmltreeGenerationOahGroup ();

    virtual               ~mxmltreeGenerationOahGroup ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMxmltreeGenerationOahGroup ();

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

    bool                  getXml2xmlInfos () const
                              { return fXml2xmlInfos; }

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

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMxmltreeGenerationOahHelp ();

    void                  printMxmltreeGenerationOahValues (unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------

    // code generation
    // --------------------------------------

    bool                  fXml2xmlInfos;

    bool                  fMusicXMLComments;
};
typedef SMARTP<mxmltreeGenerationOahGroup> S_mxmltreeGenerationOahGroup;
EXP ostream& operator<< (ostream& os, const S_mxmltreeGenerationOahGroup& elt);

EXP extern S_mxmltreeGenerationOahGroup gGlobalMxmltreeGenerationOahGroup;

//______________________________________________________________________________
EXP S_mxmltreeGenerationOahGroup createGlobalMxmltreeGenerationOahGroup ();


}


#endif
