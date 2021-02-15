/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
#include <climits>      // INT_MIN, INT_MAX

#include <regex>

#include "waeMessagesHandling.h"

#include "msr2summaryVisitor.h"
*/

#include "waeExceptions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

// #include "msdrOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdrKeywords.h"
#include "msdrTokens.h"

#include "msdlLexicalFrenchWaeHandler.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_msdlLexicalFrenchWaeHandler msdlLexicalFrenchWaeHandler::create ()
{
  msdlLexicalFrenchWaeHandler* o =
    new msdlLexicalFrenchWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlLexicalFrenchWaeHandler::msdlLexicalFrenchWaeHandler ()
{}

msdlLexicalFrenchWaeHandler::~msdlLexicalFrenchWaeHandler ()
{}

void msdlLexicalFrenchWaeHandler::illegalCharacter (char theChar)
{
  cout <<
    "caractère illégal: " <<
    "$" << theChar << "$" <<
    endl;
}

void msdlLexicalFrenchWaeHandler::illegalCharacters (string theString)
{
  cout <<
    "caractères illégaux: " <<
    "$" << theString << "$" <<
    endl;
}

void msdlLexicalFrenchWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  cout <<
    "un chiffre est attendu après un point décimal, trouvé: " <<
    "$" << theChar << "$" <<
    endl;
}
void msdlLexicalFrenchWaeHandler::digitExpectedInExponent (char theChar)
{
  cout <<
    "un chiffre est attendu dans un exposant, trouvé: " <<
    "$" << theChar << "$" <<
    endl;
}

void msdlLexicalFrenchWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  cout <<
    "caractère d'échappement illégal après '\': " <<
    "$" << theChar << "$" <<
    endl;
}

string msdlLexicalFrenchWaeHandler::asString () const
{
  stringstream s;

  s <<
    "gestionnaire WAE pour MSDL en anglais";

  return s.str ();
}

void msdlLexicalFrenchWaeHandler::print (ostream& os) const
{
  os <<
    "msdlLexicalFrenchWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlLexicalFrenchWaeHandler& elt)
{
  elt->print (os);
  return os;
}


} // namespace
