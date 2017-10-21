/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __messagesHandling__
#define __messagesHandling__


using namespace std;

namespace MusicXML2 
{

/*!
\addtogroup msr
@{
*/

//______________________________________________________________________________
/*!
\internal
\brief A macro to exit when a condition if false
*/
// a macro to abort with code source line information,
// as assert() itself

#define msrAssert( condition, messageIfFalse ) \
{ \
  if (! condition) { \
    gLogIOstream << \
      endl << \
      "#### " << messageIfFalse << \
      endl << \
      "Aborting." << \
      endl << \
      flush; \
      \
    assert(condition); \
  } \
}

/*!
\internal
\brief A macro to emit warning messages regarding MusicXML data
*/
//______________________________________________________________________________
/*
#define msrWarning( context, inputLineNumber, message ) \
{ \
  gLogIOstream << \
    endl << \
    endl << \
    "\"" << context << " WARNING" << "\"" << \
    endl << \
    gGeneralOptions->fInputSourceName << \
    ", input line " << inputLineNumber << ":" << \
    endl;
\
//  gIndenter++; \
\
  gLogIOstream << \
    message << \
    endl << \
    endl; \
\
//  gIndenter++; \
}
*/

/*!
\internal
\brief A macro to emit error messages regarding MusicXML data and exit
*/
//______________________________________________________________________________
/*
#define msrError( context, inputLineNumber, message ) \
{ \
  gLogIOstream << \
    endl << \
    endl << \
    "\"" << context << " ERROR" << "\"" << \
    endl << \
    gGeneralOptions->fInputSourceName << \
    ", input line " << inputLineNumber << ":" << \
    endl;
\
//  gIndenter++; \
\
  gLogIOstream << \
    message << \
    endl << \
    endl; \
\
//  gIndenter++; \
}
 */


/*! @} */

}


#endif
