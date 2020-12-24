/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrMutualDependencies___
#define ___bsrMutualDependencies___

#include "bsrElements.h"

#include "bsrCellsLists.h"

#include "msrTempos.h"
#include "msrDynamics.h"


namespace MusicXML2
{

/*
  This file declares classes that cannot be placed in 'individual' headers
  due to mutual dependencies, which C++ requires to be in one and the same file.

  It includes the corresponding bsr*_MUT_DEP.h files,
  which are meant to be included only in this one.
*/

//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class bsrLine;
typedef SMARTP<bsrLine> S_bsrLine;

class bsrSpaces;
typedef SMARTP<bsrSpaces> S_bsrSpaces;

class bsrPage;
typedef SMARTP<bsrPage> S_bsrPage;

class bsrMeasure;
typedef SMARTP<bsrMeasure> S_bsrMeasure;

class bsrKey;
typedef SMARTP<bsrKey> S_bsrKey;

class bsrTime;
typedef SMARTP<bsrTime> S_bsrTime;

class bsrTempo;
typedef SMARTP<bsrTempo> S_bsrTempo;

class bsrNote;
typedef SMARTP<bsrNote> S_bsrNote;


//______________________________________________________________________________

#include "bsrLineElements_MUT_DEP.h"

#include "bsrPageElements_MUT_DEP.h"

#include "bsrLines_MUT_DEP.h"

#include "bsrSpaces_MUT_DEP.h"

#include "bsrPaginations_MUT_DEP.h"

#include "bsrClefs_MUT_DEP.h"

#include "bsrKeys_MUT_DEP.h"

#include "bsrTimes_MUT_DEP.h"

#include "bsrBarlines_MUT_DEP.h"

#include "bsrNumbers_MUT_DEP.h"

#include "bsrWords_MUT_DEP.h"

#include "bsrNotes_MUT_DEP.h"

#include "bsrDynamics_MUT_DEP.h"

#include "bsrMeasures_MUT_DEP.h"

#include "bsrTempos_MUT_DEP.h"

#include "bsrPageHeadings_MUT_DEP.h"

#include "bsrMusicHeadings_MUT_DEP.h"

#include "bsrFootNotes_MUT_DEP.h"

#include "bsrPages_MUT_DEP.h"

}

#endif
