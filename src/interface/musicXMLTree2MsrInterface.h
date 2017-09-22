/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __musicxml2MsrInterface__
#define __musicxml2MsrInterface__


#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <map>

#include "smartpointer.h"
#include "rational.h"

#include "msrOptions.h"

#include "msr.h"


namespace MusicXML2 
{

//#ifdef __cplusplus
//extern "C" {
//#endif

/*!
\addtogroup Converting MusicXML to MSR format

The library includes a high level API to convert
  from the MusicXML format to the MSR
  (Music Score Representation) format.
@{
*/

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param file a file name 
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_msrScore musicxmlFile2Msr (
  const char *file, S_msrOptions& msrOpts, ostream& out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param fd a file descriptor 
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_msrScore musicxmlFd2Msr (
  FILE* fd, S_msrOptions& msrOpts, ostream& out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param buff a string containing MusicXML code
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_msrScore musicxmlString2Msr (
  const char *buff, S_msrOptions& msrOpts, ostream& out);

//______________________________________________________________________________
S_msrScore buildMSRFromElementsTree (
  S_msrOptions& msrOpts,
  Sxmlelement   xmlTree);

//______________________________________________________________________________
void displayMSR (
  S_msrOptions& msrOpts,
  S_msrScore    mScore,
  ostream&      os);

//______________________________________________________________________________
void displayMSRSummary (
  S_msrOptions& msrOpts,
  S_msrScore    mScore,
  ostream&      os);

/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
