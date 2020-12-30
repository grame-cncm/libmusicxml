/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___generalOahGroup___
#define ___generalOahGroup___

#include <set>

#include "exports.h"

#include "oahBasicTypes.h"


namespace MusicXML2
{

//______________________________________________________________________________
class EXP generalOahGroup : public oahGroup
{
  public:

    static SMARTP<generalOahGroup> create ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeGeneralOah ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    generalOahGroup ();

    virtual ~generalOahGroup ();

  public:

    // set and get
    // ------------------------------------------------------

    // translation date
    // --------------------------------------

    string                getTranslationDateFull () const
                              { return fTranslationDateFull;  }
    string                getTranslationDateYYYYMMDD () const
                              { return fTranslationDateYYYYMMDD;  }

    // warning and error handling
    // --------------------------------------

    bool                  getQuiet () const
                              { return fQuiet;  }
    bool                  getDontShowErrors () const
                              { return fDontShowErrors;  }
    bool                  getDontQuitOnErrors () const
                              { return fDontQuitOnErrors;  }
    bool                  getDisplaySourceCodePosition () const
                              { return fDisplaySourceCodePosition;  }

    // CPU usage
    // --------------------------------------

    bool                  getDisplayCPUusage () const
                              { return fDisplayCPUusage;  }

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

    void                  initializeGeneralWarningAndErrorsOptions ();

    void                  initializeGeneralCPUUsageOptions ();

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v) override;
    virtual void          acceptOut (basevisitor* v) override;

    virtual void          browseData (basevisitor* v) override;

  public:

    // print
    // ------------------------------------------------------

    void                  printGeneralOahHelp ();

    void                  printGeneralOahValues (unsigned int fieldWidth);

  private:

    // translation date
    // --------------------------------------

    string                fTranslationDateFull;
    string                fTranslationDateYYYYMMDD;

    // warning and error handling
    // --------------------------------------

    bool                  fQuiet;

    bool                  fDontShowErrors;
    bool                  fDontQuitOnErrors;

    bool                  fDisplaySourceCodePosition;

    // CPU usage
    // --------------------------------------

    bool                  fDisplayCPUusage;
};
typedef SMARTP<generalOahGroup> S_generalOahGroup;
EXP ostream& operator<< (ostream& os, const S_generalOahGroup& elt);

EXP extern S_generalOahGroup gGlobalGeneralOahGroup;

//______________________________________________________________________________
EXP S_generalOahGroup createGlobalGeneralOahGroup ();


}


#endif
