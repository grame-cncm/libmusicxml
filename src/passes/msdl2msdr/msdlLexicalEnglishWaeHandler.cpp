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

#include "msdlLexicalEnglishWaeHandler.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_msdlLexicalEnglishWaeHandler msdlLexicalEnglishWaeHandler::create ()
{
  msdlLexicalEnglishWaeHandler* o =
    new msdlLexicalEnglishWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlLexicalEnglishWaeHandler::msdlLexicalEnglishWaeHandler ()
{}

msdlLexicalEnglishWaeHandler::~msdlLexicalEnglishWaeHandler ()
{}

void msdlLexicalEnglishWaeHandler::illegalCharacter (char theChar)
{
  cout <<
    "illegal character: " <<
    "$" << theChar << "$" <<
    endl;
}

void msdlLexicalEnglishWaeHandler::illegalCharacters (string theString)
{
  cout <<
    "illegal characters: " <<
    "$" << theString << "$" <<
    endl;
}

void msdlLexicalEnglishWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  cout <<
    "a digit is expected after decimal dot, found: " <<
    "$" << theChar << "$" <<
    endl;
}
void msdlLexicalEnglishWaeHandler::digitExpectedInExponent (char theChar)
{
  cout <<
    "a digit is expected in exponent, found: " <<
    "$" << theChar << "$" <<
    endl;
}

void msdlLexicalEnglishWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  cout <<
    "illegal escaped character after '\': " <<
    "$" << theChar << "$" <<
    endl;
}

string msdlLexicalEnglishWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlLexicalEnglishWaeHandler::print (ostream& os) const
{
  os <<
    "msdlLexicalEnglishWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlLexicalEnglishWaeHandler& elt)
{
  elt->print (os);
  return os;
}


} // namespace
