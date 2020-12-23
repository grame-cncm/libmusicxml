/*
  MusicXML Library
  Copyright (C) Grame 2006-2020

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __libxml2gmn__
#define __libxml2gmn__

// this file requires the use of C++

#include "libmusicxml.h" // for xmlErr
#include "liboptionsvector.h"

#include "exports.h"


namespace MusicXML2
{

//_______________________________________________________________________________
/*!
  \brief Gives the MusicXML to MusicXML converter version number.
  \return a version number as a float value
*/
EXP float       musicxml2guidoVersion();
/*!
  \brief Gives the MusicXML converter version as a string.
  \return a string
*/
EXP const char*   musicxml2guidoVersionStr();

/*!
  \brief Converts a MusicXML representation to the MusicXML format.
  \param file a file name
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlfile2guido  (const char *file, const optionsVector& options, std::ostream& out, std::ostream& err);

/*!
  \brief Converts a MusicXML representation to the MusicXML format.
  \param fd a file descriptor
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlfd2guido  (FILE* fd, const optionsVector& options, std::ostream& out, std::ostream& err);

/*!
  \brief Converts a MusicXML representation to the MusicXML format.
  \param buffer a string containing MusicXML code
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr      musicxmlstring2guido(const char *buffer, const optionsVector& options, std::ostream& out, std::ostream& err);


}


#endif
