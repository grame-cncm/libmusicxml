/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __musicxml2lpsr__
#define __musicxml2lpsr__


#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <map>

#include "smartpointer.h"

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
see  http://www.lpsr.org
@{
*/

//______________________________________________________________________________
/*!
  \brief Gives the lpsr converter version number.
  \return a version number as an integer (e.g. version 1.0.0 is returned as 100)
*/
EXP int           musicxml2LpsrVersionNbr();

/*!
  \brief Gives the lpsr converter version as a string.
  \return a string
*/
EXP const char*   musicxml2LpsrVersionStr();

//______________________________________________________________________________
/*!
  \brief The LilyPond note names language.
*/
enum LpsrNoteNamesLanguage {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kItaliano, 
  kFrancais, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams};

typedef std::map<std::string, LpsrNoteNamesLanguage>
  LpsrNoteNamesLanguageMap;
  
static LpsrNoteNamesLanguageMap gLpsrNoteNamesLanguageMap;

void                  initializeLpsrNoteNamesLanguage ();
LpsrNoteNamesLanguage getLpsrNoteNamesLanguage (std::string lang);
  
//______________________________________________________________________________
/*!
  \brief The LPSR code generation Settings.

  A class is used to avoid passing arguments one by one
  to the various methods that need them.
*/
class EXP translationSettings : public smartable {
  public:

    static SMARTP<translationSettings> create();
    
  public:
  
    translationSettings();
    virtual ~translationSettings();
 
  public:

    // languages
    std::string                     fLpsrNoteNamesLanguageAsString;
    LpsrNoteNamesLanguage           fLpsrNoteNamesLanguage;
    
    // LilyPond code generation
    bool                            fGenerateAbsoluteCode;
    bool                            fGenerateNumericalTime;
    bool                            fGenerateComments;
    bool                            fGenerateBars;
    bool                            fGenerateStems;
    bool                            fGeneratePositions;
    
    // debug and trace
    bool                            fDebug;
    bool                            fTrace;
    
    // to be displayed in the LilyPond code as a comment
    std::string                     fSelectedOptions;
};
typedef SMARTP<translationSettings> S_translationSettings;

//______________________________________________________________________________
/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param file a file name 
  \param ts the translation Settings to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr musicxmlFile2Lpsr (
  const char *file, S_translationSettings& ts, std::ostream& out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param fd a file descriptor 
  \param ts the translation Settings to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr musicxmlFd2Lpsr (
  FILE* fd, S_translationSettings& ts, std::ostream& out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param buff a string containing MusicXML code
  \param sw the translation Settings to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr musicxmlString2Lpsr (
  const char *buff, S_translationSettings& ts, std::ostream& out);


/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
