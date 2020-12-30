/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___lilypondOah___
#define ___lilypondOah___

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP lilypondOahGroup : public oahGroup
{
  public:

    // creation
    // ------------------------------------------------------

    static SMARTP<lilypondOahGroup> create ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeLilypondOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    lilypondOahGroup ();

    virtual ~lilypondOahGroup ();

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
// JMI    void                  initializelilypondOahTraceOah ();
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

    void                  printLilypondOahHelp ();

    void                  printLilypondOahValues (unsigned int fieldWidth);

  private:

    // private fields
    // ------------------------------------------------------
};
typedef SMARTP<lilypondOahGroup> S_lilypondOahGroup;
EXP ostream& operator<< (ostream& os, const S_lilypondOahGroup& elt);

EXP extern S_lilypondOahGroup gGlobalLilypondOahGroup;

//______________________________________________________________________________
EXP S_lilypondOahGroup createGlobalLilypondOahGroup ();


}


#endif
