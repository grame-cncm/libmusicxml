/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mxmlTreeOah___
#define ___mxmlTreeOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


//#include "msrBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class mxmlTreeOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxmlTreeOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMxmlTreeOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mxmlTreeOahGroup ();

    virtual ~mxmlTreeOahGroup ();

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

#ifdef TRACING_IS_ENABLED
    // specific trace JMI move to traceOah
    // --------------------------------------

    // visitors
    bool                  getTraceMusicXMLTreeVisitors () const
                              { return fTraceMusicXMLTreeVisitors; }
#endif

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMxmlTreeTraceOah ();
#endif

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printMxmlTreeOahHelp ();

    void                  printMxmlTreeOahValues (int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------


#ifdef TRACING_IS_ENABLED
    // specific trace JMI move to traceOah
    // --------------------------------------

    // visitors
    bool                  fTraceMusicXMLTreeVisitors;
#endif
};
typedef SMARTP<mxmlTreeOahGroup> S_mxmlTreeOahGroup;
EXP ostream& operator<< (ostream& os, const S_mxmlTreeOahGroup& elt);

EXP extern S_mxmlTreeOahGroup gGlobalMxmlTreeOahGroup;

//______________________________________________________________________________
S_mxmlTreeOahGroup createGlobalMxmlTreeOahGroup ();


}


#endif
