/*
  MusicXML Library
  Copyright (C) Grame 2006-2020

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __libxml2ly__
#define __libxml2ly__

// this file requires the use of C++

#include "libmusicxml.h" // for xmlErr
#include "liboptionsvector.h"

#include "exports.h"


namespace MusicXML2
{

//_______________________________________________________________________________
/*!
  \brief Gives the MusicXML to LilyPond converter version number.
  \return a version number as a float value
*/
EXP float       msdlVersion();
/*!
  \brief Gives the LilyPond converter version as a string.
  \return a string
*/
EXP const char*   msdlVersionStr();

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param file a file name
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      msdlfile2lilypond  (const char *file, const optionsVector& options, std::ostream& out, std::ostream& err);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param fd a file descriptor
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      msdlfd2lilypond  (FILE* fd, const optionsVector& options, std::ostream& out, std::ostream& err);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param buffer a string containing MusicXML code
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      msdlstring2lilypond(const char *buffer, const optionsVector& options, std::ostream& out, std::ostream& err);


}


#endif
