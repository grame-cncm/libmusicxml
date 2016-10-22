/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __musicxml2msr__
#define __musicxml2msr__


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
see  http://www.msr.org
@{
*/

//______________________________________________________________________________
/*!
  \brief Gives the msr converter version number.
  \return a version number as an integer (e.g. version 1.0.0 is returned as 100)
*/
EXP int           musicxml2MsrVersionNbr();

/*!
  \brief Gives the msr converter version as a string.
  \return a string
*/
EXP const char*   musicxml2MsrVersionStr();

//______________________________________________________________________________
/*!
  \brief The LilyPond note names language.
*/
enum MsrNoteNamesLanguage {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kItaliano, 
  kFrancais, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams};

typedef std::map<std::string, MsrNoteNamesLanguage>
  MsrNoteNamesLanguageMap;
  
static MsrNoteNamesLanguageMap gMsrNoteNamesLanguageMap;

void                  initializeMsrNoteNamesLanguage ();
MsrNoteNamesLanguage getMsrNoteNamesLanguage (std::string lang);
  
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
    std::string                     fMsrNoteNamesLanguageAsString;
    MsrNoteNamesLanguage            fMsrNoteNamesLanguage;

    // MSR
    bool                            fGenerateStaffRelativeVoiceNumbers;
    
    // LilyPond code generation
    bool                            fGenerateAbsoluteCode;
    bool                            fGenerateNumericalTime;
    bool                            fGenerateComments;
    bool                            fGenerateStems;
    bool                            fGeneratePositions;

    // advanced settings
    bool                            fDelayRestsDynamics;

    // intermediate form
    bool                            fDisplayMSR;
    
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
EXP xmlErr musicxmlFile2Msr (
  const char *file, S_translationSettings& ts, std::ostream& out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param fd a file descriptor 
  \param ts the translation Settings to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr musicxmlFd2Msr (
  FILE* fd, S_translationSettings& ts, std::ostream& out);

/*!
  \brief Converts a MusicXML representation to the LilyPond format.
  \param buff a string containing MusicXML code
  \param sw the translation Settings to be used
  \param out the output stream
  \return an error code (\c kNoErr when success)
*/
EXP xmlErr musicxmlString2Msr (
  const char *buff, S_translationSettings& ts, std::ostream& out);


/*! @} */

//#ifdef __cplusplus
//}
//#endif

} // namespace MusicXML2


#endif
