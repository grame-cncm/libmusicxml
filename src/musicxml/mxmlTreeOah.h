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
class mxmlTreeOah : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<mxmlTreeOah> create (
      S_oahHandler handlerUpLink);

    SMARTP<mxmlTreeOah>   createCloneWithDetailedTrace ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMxmlTreeOah (
                            bool boolOptionsInitialValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    mxmlTreeOah (
      S_oahHandler handlerUpLink);

    virtual ~mxmlTreeOah ();

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

    void                  setAllMxmlTreeTraceOah (
                            bool boolOptionsInitialValue);

  public:

    // public services
    // ------------------------------------------------------

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACE_OAH
    void                  initializeMxmlTreeTraceOah (
                            bool boolOptionsInitialValue);
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

  public:

    // fields
    // ------------------------------------------------------


#ifdef TRACE_OAH
    // specific trace JMI move to traceOah
    // --------------------------------------

    // visitors
    bool                  fTraceMusicXMLTreeVisitors;
#endif
};
typedef SMARTP<mxmlTreeOah> S_mxmlTreeOah;
EXP ostream& operator<< (ostream& os, const S_mxmlTreeOah& elt);

EXP extern S_mxmlTreeOah globalMxmlTreeOah;
EXP extern S_mxmlTreeOah globalMxmlTreeOahUserChoices;
EXP extern S_mxmlTreeOah globalMxmlTreeOahWithDetailedTrace;

//______________________________________________________________________________
void initializeMxmlTreeOahHandling (
  S_oahHandler handler);


}


#endif
