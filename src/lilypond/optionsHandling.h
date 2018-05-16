/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __optionsHandling__
#define __optionsHandling__

#include "list"
#include "vector"
#include "map"
#include "set"

#include "smartpointer.h"
#include "rational.h"

#include "utilities.h"

#include "msrBasicTypes.h"
#include "lpsrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
// PRE-declarations for class dependencies

class optionsElement;
typedef SMARTP<optionsElement> S_optionsElement;

class optionsSubGroup;
typedef SMARTP<optionsSubGroup> S_optionsSubGroup;

class optionsGroup;
typedef SMARTP<optionsGroup> S_optionsGroup;

class EXP optionsHandler;
typedef SMARTP<optionsHandler> S_optionsHandler;


}


#endif
