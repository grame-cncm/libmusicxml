/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "waeExceptions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdrKeywords.h"
#include "msdrTokens.h"

#include "msdlScannerWaeHandlers.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
/* this class is purely virtual
S_msdlScannerWaeHandler msdlScannerWaeHandler::create ()
{
  msdlScannerWaeHandler* o =
    new msdlScannerWaeHandler ();
  assert (o != nullptr);
  return o;
}
*/

msdlScannerWaeHandler::msdlScannerWaeHandler ()
{}

msdlScannerWaeHandler::~msdlScannerWaeHandler ()
{}

string msdlScannerWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL WAE lexical handler";

  return s.str ();
}

void msdlScannerWaeHandler::print (ostream& os) const
{
  os <<
    "msdlScannerWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlScannerWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlScannerEnglishWaeHandler msdlScannerEnglishWaeHandler::create ()
{
  msdlScannerEnglishWaeHandler* o =
    new msdlScannerEnglishWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlScannerEnglishWaeHandler::msdlScannerEnglishWaeHandler ()
{}

msdlScannerEnglishWaeHandler::~msdlScannerEnglishWaeHandler ()
{}

void msdlScannerEnglishWaeHandler::illegalCharacter (char theChar)
{
  cout <<
    "illegal character: " <<
    "'" << theChar << "'" <<
    endl;
}

void msdlScannerEnglishWaeHandler::illegalCharacters (string theString)
{
  cout <<
    "illegal characters: " <<
    "\"" << theString << "\"" <<
    endl;
}

void msdlScannerEnglishWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  cout <<
    "a digit is expected after decimal dot, found: " <<
    "'" << theChar << "'" <<
    endl;
}
void msdlScannerEnglishWaeHandler::digitExpectedInExponent (char theChar)
{
  cout <<
    "a digit is expected in exponent, found: " <<
    "'" << theChar << "'" <<
    endl;
}

void msdlScannerEnglishWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  cout <<
    "illegal escaped character after '\': " <<
    "'" << theChar << "'" <<
    endl;
}

string msdlScannerEnglishWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlScannerEnglishWaeHandler::print (ostream& os) const
{
  os <<
    "msdlScannerEnglishWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlScannerEnglishWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlScannerFrenchWaeHandler msdlScannerFrenchWaeHandler::create ()
{
  msdlScannerFrenchWaeHandler* o =
    new msdlScannerFrenchWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlScannerFrenchWaeHandler::msdlScannerFrenchWaeHandler ()
{}

msdlScannerFrenchWaeHandler::~msdlScannerFrenchWaeHandler ()
{}

void msdlScannerFrenchWaeHandler::illegalCharacter (char theChar)
{
  cout <<
    "caractère illégal: " <<
    "'" << theChar << "'" <<
    endl;
}

void msdlScannerFrenchWaeHandler::illegalCharacters (string theString)
{
  cout <<
    "caractères illégaux: " <<
    "\"" << theString << "\"" <<
    endl;
}

void msdlScannerFrenchWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  cout <<
    "un chiffre est attendu après un point décimal, trouvé: " <<
    "'" << theChar << "'" <<
    endl;
}
void msdlScannerFrenchWaeHandler::digitExpectedInExponent (char theChar)
{
  cout <<
    "un chiffre est attendu dans un exposant, trouvé: " <<
    "'" << theChar << "'" <<
    endl;
}

void msdlScannerFrenchWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  cout <<
    "caractère d'échappement illégal après '\': " <<
    "'" << theChar << "'" <<
    endl;
}

string msdlScannerFrenchWaeHandler::asString () const
{
  stringstream s;

  s <<
    "gestionnaire WAE pour MSDL en français";

  return s.str ();
}

void msdlScannerFrenchWaeHandler::print (ostream& os) const
{
  os <<
    "msdlScannerFrenchWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlScannerFrenchWaeHandler& elt)
{
  elt->print (os);
  return os;
}


} // namespace
