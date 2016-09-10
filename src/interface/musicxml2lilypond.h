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
\addtogroup Converting MusicXML to LilyPond format

The library includes a high level API to convert from the MusicXML format to the
LilyPond (.ly) format. For more information about this format, 
see  http://www.lilypond.org
@{
*/

/*!
  \brief Gives the lilypond converter version number.
  \return a version number as an integer (e.g. version 1.0.0 is returned as 100)
*/
EXP int           musicxml2lilypondVersion();
/*!
  \brief Gives the lilypond converter version as a string.
  \return a string
*/
EXP const char*   musicxml2lilypondVersionStr();

/*!
  \brief The lilypond code generation switches.

  A structure is used to avoid passing arguments one by one
  to the various methods that need them.
*/
typedef struct {
  bool fTrace;
  bool fGenerateAbsoluteCode;
  bool fGenerateComments;
  bool fGenerateBars;
  bool fGenerateStems;
  bool fGeneratePositions;
  bool fDebug;
} translationSwitches;

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param file a file name 
  \param sw the translation switches to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlfile2lilypond  (const char *file, translationSwitches sw, std::ostream& out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param fd a file descriptor 
  \param sw the translation switches to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlfd2lilypond  (FILE* fd, translationSwitches sw, std::ostream& out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param buff a string containing MusicXML code
  \param sw the translation switches to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlstring2lilypond(const char *buff, translationSwitches sw, std::ostream& out);


/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2
