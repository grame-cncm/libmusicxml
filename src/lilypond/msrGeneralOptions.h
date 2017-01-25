/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrGeneralOptions__
#define __msrGeneralOptions__

#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <map>

#include "smartpointer.h"
/*
#include "exports.h"
#include "typedefs.h"

#include "tree_browser.h"

#include "msrUtilities.h"

//JMI #include "musicxml2msr.h"
*/

namespace MusicXML2 
{

//______________________________________________________________________________
/*!
  \brief The general options.

  A class is used to avoid passing arguments one by one
  to the various methods that need them.
*/
class EXP msrGeneralOptions : public smartable
{
  public:

    static SMARTP<msrGeneralOptions> create ();
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    msrGeneralOptions();
  
    virtual ~msrGeneralOptions();
 
  public:

    // synthetic view
    string    fCommandLineOptions;
    string    fInputSourceName;
    string    fTranslationDate;

    // interactive mode
    bool      fInteractive;
    
    // trace
    bool      fTrace;
    
    // debug
    bool      fDebug;
    bool      fDebugDebug;

    // forcing debug information at specific places in the code
    bool      fForceDebug;
    
    // measure number-selective debug
    set<int>  fDebugMeasureNumbersSet;
    bool      fSaveDebug;
    bool      fSaveDebugDebug;
};
typedef SMARTP<msrGeneralOptions> S_msrGeneralOptions;

extern S_msrGeneralOptions gGeneralOptions;


/*! @} */

}


#endif
