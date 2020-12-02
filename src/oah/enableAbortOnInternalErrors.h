/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___enableAbortOnInternalErrorsIfDesired___
#define ___enableAbortOnInternalErrorsIfDesired___


// comment the following definition if abort on internal errors is desired
// CAUTION: DON'T USE THIS IN PRODUCTION CODE,
// since that could kill a session on a web server, for example

#ifndef ABORT_ON_INTENAL_ERRORS
//  #define ABORT_ON_INTENAL_ERRORS
#endif

#endif
