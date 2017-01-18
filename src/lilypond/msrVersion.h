/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrVersion__
#define __msrVersion__

#include <string>

namespace MusicXML2 
{

/*!
\addtogroup version
@{
*/

//______________________________________________________________________________
//______________________________________________________________________________
/*!
  \brief Gives the MSR version number.
  \return a version number as an integer (e.g. version 1.0.0 is returned as 100)
*/
int           MSRVersionNumber ();

/*!
  \brief Gives the MSR version number as a string.
  \return a string
*/
const char*   MSRVersionNumberAsString ();


/*! @} */

}


#endif
