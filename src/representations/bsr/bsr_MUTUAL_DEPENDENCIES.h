/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
  This file declares classes that cannot be placed in 'individual' headers
  due to mutual dependencies, which C++ requires to be in one and the same file.

  It includes the corresponding bsr*.h files,
  which are meant to be included only in this one.
*/

#ifndef ___bsrMutualDependencies___
#define ___bsrMutualDependencies___

#include "smartpointer.h"

#include "bsrElements.h"

#include "bsrCellsLists.h"

//______________________________________________________________________________

#include "bsrLineContentsElements.h"

#include "bsrPageElements.h"

#include "bsrLines.h"

#include "bsrSpaces.h"

#include "bsrPaginations.h"

#include "bsrClefs.h"

#include "bsrKeys.h"

#include "bsrTimes.h"

#include "bsrBarlines.h"

#include "bsrNumbers.h"

#include "bsrWords.h"

#include "bsrNotes.h"

#include "bsrDynamics.h"

#include "bsrMeasures.h"

#include "bsrTempos.h"

#include "bsrPageHeadings.h"

#include "bsrMusicHeadings.h"

#include "bsrFootNotes.h"

#include "bsrPages.h"


#endif
