/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __musicXML2mxmlTreeInterface__
#define __musicXML2mxmlTreeInterface__

#include "typedefs.h"


namespace MusicXML2
{

/*!
\addtogroup mxmltree2Msr
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
EXP Sxmlelement musicXMLFile2mxmlTree (
  const char*       fileName,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream);

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param fd a file descriptor 
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/

EXP Sxmlelement musicXMLFd2mxmlTree (
  FILE*             fd,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream);

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to an MSR.
  \param buff a string containing MusicXML code
  \param msrOpts the MSR options to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP Sxmlelement musicXMLString2mxmlTree (
  const char*       buffer,
  S_musicXMLOptions mxmlOpts,
  indentedOstream&  logIOstream);


/*! @} */

}

#endif
