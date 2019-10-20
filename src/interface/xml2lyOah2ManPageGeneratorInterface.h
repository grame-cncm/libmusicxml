/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2lyOah2ManPageGeneratorInterface__
#define __xml2lyOah2ManPageGeneratorInterface__


#include "bsr.h"
#include "exports.h"

#include "xml2lyOah2ManPageGenerator.h"


namespace MusicXML2
{

/*!
\addtogroup Converting MSR to BSR format

The library includes a high level API to convert
  from the MusicXML format to the BSR
  (Braille Score Representation) format.
@{
*/

//______________________________________________________________________________
EXP void generateXml2lyManPageFromOahHandler (
  const S_oahHandler handler,
  S_manPageOah       manPageOpts,
  indentedOstream&   logOstream,
  ostream&           manPageOutputStream);

/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
