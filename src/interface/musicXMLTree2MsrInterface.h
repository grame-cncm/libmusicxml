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
  \brief Converts a MusicXML representation to an MSR.
  \param file a file name 
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_msrScore musicxmlFile2Msr (
  const char*      fileName,
  S_msrOptions&    msrOpts,
  indentedOstream& logIOstream);

/*!
  \brief Converts a MusicXML representation to an MSR.
  \param fd a file descriptor 
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_msrScore musicxmlFd2Msr (
  FILE*            fd,
  S_msrOptions&    msrOpts,
  indentedOstream& logIOstream);

/*!
  \brief Converts a MusicXML representation to an MSR.
  \param buff a string containing MusicXML code
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_msrScore musicxmlString2Msr (
  const char*      buffer,
  S_msrOptions&    msrOpts,
  indentedOstream& logIOstream);

//______________________________________________________________________________
S_msrScore buildMSRFromElementsTree (
  S_msrOptions& msrOpts,
  Sxmlelement   xmlTree);

//______________________________________________________________________________
void displayMSR (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);

//______________________________________________________________________________
void displayMSRSummary (
  S_msrOptions&    msrOpts,
  S_msrScore       mScore,
  indentedOstream& logIOstream);

/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
