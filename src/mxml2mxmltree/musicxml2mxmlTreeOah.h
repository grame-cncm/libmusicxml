/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___musicxml2mxmlTreeOah___
#define ___musicxml2mxmlTreeOah___

#include "msrSegnosAndCodas.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class musicxml2mxmlTreeOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<musicxml2mxmlTreeOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMusicxml2mxmlTree ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    musicxml2mxmlTreeOahGroup ();

    virtual ~musicxml2mxmlTreeOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    virtual void          checkGroupOptionsConsistency () override;

  private:

    // private services
    // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
    void                  initializeMusicxml2mxmlTreeTraceOah ();
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

    void                  printMusicxml2mxmlTreeHelp ();

    void                  printMusicxml2mxmlTreeValues (int fieldWidth);

  public:

    // fields
    // ------------------------------------------------------
};
typedef SMARTP<musicxml2mxmlTreeOahGroup> S_musicxml2mxmlTreeOahGroup;
EXP ostream& operator<< (ostream& os, const S_musicxml2mxmlTreeOahGroup& elt);

EXP extern S_musicxml2mxmlTreeOahGroup gGlobalMusicxml2mxmlTreeOahGroup;

//______________________________________________________________________________
S_musicxml2mxmlTreeOahGroup createGlobalMusicxml2mxmlTreeOahGroup ();


}


#endif
