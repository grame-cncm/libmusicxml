/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___guidoOutputOah___
#define ___guidoOutputOah___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP guidoOutputOahGroup : public oahGroup
{
  public:

    static SMARTP<guidoOutputOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeGuidoOutputOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          guidoOutputOahGroup ();

    virtual               ~guidoOutputOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setGenerateGuidoComments ()
                              { fGenerateGuidoComments = true; }
    bool                  getGenerateGuidoComments () const
                              { return fGenerateGuidoComments; }

    void                  setGenerateGuidoStem ()
                              { fGenerateGuidoStem = true; }
    bool                  getGenerateGuidoStem () const
                              { return fGenerateGuidoStem; }

    void                  setGenerateGuidoBars ()
                              { fGenerateGuidoBars = true; }
    bool                  getGenerateGuidoBars () const
                              { return fGenerateGuidoBars; }

  public:

    // public services
    // ------------------------------------------------------

    // quiet mode
    void                  enforceGroupQuietness () override;

    // consistency check
    void                  checkGroupOptionsConsistency () override;

  public:

    // visitors
    // ------------------------------------------------------

    void                  acceptIn  (basevisitor* v) override;
    void                  acceptOut (basevisitor* v) override;

    void                  browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printGuidoOutputOahGroupHelp ();

    void                  printGuidoOutputOahGroupValues (
                            unsigned int fieldWidth);

  private:

    // private services
    // ------------------------------------------------------

    void                  createGuidoSubGroup ();

    void                  createLilypondSubGroup ();

  private:

    // private fields
    // ------------------------------------------------------

    bool                  fGenerateGuidoComments;
    bool                  fGenerateGuidoStem;
    bool                  fGenerateGuidoBars;
};
typedef SMARTP<guidoOutputOahGroup> S_guidoOutputOahGroup;
EXP ostream& operator<< (ostream& os, const S_guidoOutputOahGroup& elt);

EXP extern S_guidoOutputOahGroup gGlobalGuidoOutputOahGroup;

//______________________________________________________________________________
S_guidoOutputOahGroup createGlobalGuidoOutputOahGroup ();


}


#endif
