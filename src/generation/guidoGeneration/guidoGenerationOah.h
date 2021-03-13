/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___guidoGenerationOah___
#define ___guidoGenerationOah___

#include "msrBasicTypes.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP guidoGenerationOahGroup : public oahGroup
{
  public:

    static SMARTP<guidoGenerationOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeGuidoGenerationOahGroup ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

                          guidoGenerationOahGroup ();

    virtual               ~guidoGenerationOahGroup ();

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

    void                  printGuidoGenerationOahGroupHelp ();

    void                  printGuidoGenerationOahGroupValues (
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
typedef SMARTP<guidoGenerationOahGroup> S_guidoGenerationOahGroup;
EXP ostream& operator<< (ostream& os, const S_guidoGenerationOahGroup& elt);

EXP extern S_guidoGenerationOahGroup gGlobalGuidoGenerationOahGroup;

//______________________________________________________________________________
S_guidoGenerationOahGroup createGlobalGuidoGenerationOahGroup ();


}


#endif
