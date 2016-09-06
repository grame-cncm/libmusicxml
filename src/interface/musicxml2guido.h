/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>

#include "libmusicxml.h"


namespace MusicXML2 
{

//#ifdef __cplusplus
//extern "C" {
//#endif

/*!
\addtogroup Converting MusicXML to Guido Music Notation format

The library includes a high level API to convert from the MusicXML format to the
Guido Music Notation (GMN) format. For more information about this format, 
see  http://guidolib.sourceforge.net
@{
*/

/*!
  \brief Gives the guido converter version number.
  \return a version number as an integer (e.g. version 1.0.0 is returned as 100)
*/
EXP int       musicxml2guidoVersion();
/*!
  \brief Gives the guido converter version as a string.
  \return a string
*/
EXP const char*   musicxml2guidoVersionStr();

/*!
  \brief Converts a MusicXML representation to the Guido format.
  \param file a file name 
  \param generateBars a boolean to force barlines generation
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlfile2guido  (const char *file, bool generateBars, std::ostream& out);

/*!
  \brief Converts a MusicXML representation to the Guido format.
  \param fd a file descriptor 
  \param generateBars a boolean to force barlines generation
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlfd2guido  (FILE* fd, bool generateBars, std::ostream& out);

/*!
  \brief Converts a MusicXML representation to the Guido format.
  \param buff a string containing MusicXML code
  \param generateBars a boolean to force barlines generation
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlstring2guido(const char *buff, bool generateBars, std::ostream& out);

/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2
