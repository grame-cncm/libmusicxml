/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __musicxml2PartsInfo__
#define __musicxml2PartsInfo__


#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <map>

#include "smartpointer.h"

#include "libmusicxml.h"

#include "xmlPartsInfo.h"


namespace MusicXML2 
{

//#ifdef __cplusplus
//extern "C" {
//#endif

/*!
\addtogroup Extraction parts information from MusicXML data

The library includes a high level API to convert
  from the MusicXML format to the MSR
  (Music Score Representation) format.
@{
*/

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param file a file name 
  \param partsInfoOptions the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_xmlPartsInfo musicxmlFile2PartsInfo (
  const char*            file,
  S_xmlPartsInfoOptions& partsInfoOptions,
  std::ostream&          out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param fd a file descriptor 
  \param partsInfoOptions the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_xmlPartsInfo musicxmlFd2PartsInfo (
  FILE*                  fd,
  S_xmlPartsInfoOptions& partsInfoOptions,
  std::ostream&          out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param buff a string containing MusicXML code
  \param partsInfoOptions the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP S_xmlPartsInfo musicxmlString2PartsInfo (
  const char *           buff,
  S_xmlPartsInfoOptions& partsInfoOptions,
  std::ostream&          out);

//______________________________________________________________________________
S_xmlPartsInfo extractPartsInfoFromXMLElementTree (
  S_xmlPartsInfoOptions& partsInfoOptions,
  Sxmlelement            xmlTree);

//______________________________________________________________________________
void displayXmlPartsInfo (
  S_xmlPartsInfoOptions& partsInfoOptions,
  S_xmlPartsInfo         partsInfo,
  ostream&               os);

/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
