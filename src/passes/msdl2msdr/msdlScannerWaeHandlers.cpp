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

#include "msdlKeywords.h"
#include "msdlTokens.h"

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
  gLogStream <<
    "### illegal character: " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerEnglishWaeHandler::illegalCharacters (string theString)
{
  gLogStream <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    endl;
}

void msdlScannerEnglishWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  gLogStream <<
    "### a digit is expected after decimal dot, found: " <<
    "'" << theChar << "' ###" <<
    endl;
}
void msdlScannerEnglishWaeHandler::digitExpectedInExponent (char theChar)
{
  gLogStream <<
    "### a digit is expected in exponent, found: " <<
    "'" << theChar << "' " <<
    endl;
}

void msdlScannerEnglishWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  gLogStream <<
    "### illegal escaped character after '\': " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerEnglishWaeHandler::inputIsLexicallyCorrect (int numberOfTokens)
{
  gLogStream <<
    "The MSDL input is lexically correct. It contains " <<
    numberOfTokens <<
    " tokens" <<
    endl;
}

void msdlScannerEnglishWaeHandler::inputIsLexicallyIncorrect (int numberOfTokens)
{
  gLogStream <<
    "### The MSDL input is lexically incorrect. It contains " <<
    numberOfTokens <<
    " tokens ###" <<
    endl;
}

void msdlScannerEnglishWaeHandler::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsTranslationLanguage)
{
  gLogStream <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsTranslationLanguage) <<
    "\", there is no need to translate it ###" <<
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
  gLogStream <<
    "### caractère illégal: " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerFrenchWaeHandler::illegalCharacters (string theString)
{
  gLogStream <<
    "### caractères illégaux: " <<
    "\"" << theString << "\" ###" <<
    endl;
}

void msdlScannerFrenchWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  gLogStream <<
    "### un chiffre est attendu après un point décimal, trouvé: " <<
    "'" << theChar << "' ###" <<
    endl;
}
void msdlScannerFrenchWaeHandler::digitExpectedInExponent (char theChar)
{
  gLogStream <<
    "### un chiffre est attendu dans un exposant, trouvé: " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerFrenchWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  gLogStream <<
    "### caractère d'échappement illégal après '\': " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerFrenchWaeHandler::inputIsLexicallyCorrect (int numberOfTokens)
{
  gLogStream <<
    "Le texte MSDL est lexicalement correct. Il contient " <<
    numberOfTokens <<
    " symboles terminaux" <<
    endl;
}

void msdlScannerFrenchWaeHandler::inputIsLexicallyIncorrect (int numberOfTokens)
{
  gLogStream <<
    "### Le texte MSDL est lexicalement incorrect. Il contient " <<
    numberOfTokens <<
    " symboles terminaux ###" <<
    endl;
}

void msdlScannerFrenchWaeHandler::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsTranslationLanguage)
{
  gLogStream <<
    "### Le texte MSDL est déjà dans la langue \"" <<
    msdlKeywordsLanguageKindAsString (keywordsTranslationLanguage) <<
    "\" pour les mots clés, il n'a a pas besoin de le traduire ###" <<
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

//_______________________________________________________________________________
S_msdlScannerItalianWaeHandler msdlScannerItalianWaeHandler::create ()
{
  msdlScannerItalianWaeHandler* o =
    new msdlScannerItalianWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlScannerItalianWaeHandler::msdlScannerItalianWaeHandler ()
{}

msdlScannerItalianWaeHandler::~msdlScannerItalianWaeHandler ()
{}

void msdlScannerItalianWaeHandler::illegalCharacter (char theChar)
{
  gLogStream <<
    "### illegal character: " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerItalianWaeHandler::illegalCharacters (string theString)
{
  gLogStream <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    endl;
}

void msdlScannerItalianWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  gLogStream <<
    "### a digit is expected after decimal dot, found: " <<
    "'" << theChar << "' ###" <<
    endl;
}
void msdlScannerItalianWaeHandler::digitExpectedInExponent (char theChar)
{
  gLogStream <<
    "### a digit is expected in exponent, found: " <<
    "'" << theChar << "' " <<
    endl;
}

void msdlScannerItalianWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  gLogStream <<
    "### illegal escaped character after '\': " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerItalianWaeHandler::inputIsLexicallyCorrect (int numberOfTokens)
{
  gLogStream <<
    "The MSDL input is lexically correct. It contains " <<
    numberOfTokens <<
    " tokens" <<
    endl;
}

void msdlScannerItalianWaeHandler::inputIsLexicallyIncorrect (int numberOfTokens)
{
  gLogStream <<
    "### The MSDL input is lexically incorrect. It contains " <<
    numberOfTokens <<
    " tokens ###" <<
    endl;
}

void msdlScannerItalianWaeHandler::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsTranslationLanguage)
{
  gLogStream <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsTranslationLanguage) <<
    "\", there is no need to translate it ###" <<
    endl;
}

string msdlScannerItalianWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlScannerItalianWaeHandler::print (ostream& os) const
{
  os <<
    "msdlScannerItalianWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlScannerItalianWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlScannerGermanWaeHandler msdlScannerGermanWaeHandler::create ()
{
  msdlScannerGermanWaeHandler* o =
    new msdlScannerGermanWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlScannerGermanWaeHandler::msdlScannerGermanWaeHandler ()
{}

msdlScannerGermanWaeHandler::~msdlScannerGermanWaeHandler ()
{}

void msdlScannerGermanWaeHandler::illegalCharacter (char theChar)
{
  gLogStream <<
    "### illegal character: " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerGermanWaeHandler::illegalCharacters (string theString)
{
  gLogStream <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    endl;
}

void msdlScannerGermanWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  gLogStream <<
    "### a digit is expected after decimal dot, found: " <<
    "'" << theChar << "' ###" <<
    endl;
}
void msdlScannerGermanWaeHandler::digitExpectedInExponent (char theChar)
{
  gLogStream <<
    "### a digit is expected in exponent, found: " <<
    "'" << theChar << "' " <<
    endl;
}

void msdlScannerGermanWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  gLogStream <<
    "### illegal escaped character after '\': " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerGermanWaeHandler::inputIsLexicallyCorrect (int numberOfTokens)
{
  gLogStream <<
    "The MSDL input is lexically correct. It contains " <<
    numberOfTokens <<
    " tokens" <<
    endl;
}

void msdlScannerGermanWaeHandler::inputIsLexicallyIncorrect (int numberOfTokens)
{
  gLogStream <<
    "### The MSDL input is lexically incorrect. It contains " <<
    numberOfTokens <<
    " tokens ###" <<
    endl;
}

void msdlScannerGermanWaeHandler::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsTranslationLanguage)
{
  gLogStream <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsTranslationLanguage) <<
    "\", there is no need to translate it ###" <<
    endl;
}

string msdlScannerGermanWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlScannerGermanWaeHandler::print (ostream& os) const
{
  os <<
    "msdlScannerGermanWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlScannerGermanWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlScannerSpanishWaeHandler msdlScannerSpanishWaeHandler::create ()
{
  msdlScannerSpanishWaeHandler* o =
    new msdlScannerSpanishWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlScannerSpanishWaeHandler::msdlScannerSpanishWaeHandler ()
{}

msdlScannerSpanishWaeHandler::~msdlScannerSpanishWaeHandler ()
{}

void msdlScannerSpanishWaeHandler::illegalCharacter (char theChar)
{
  gLogStream <<
    "### illegal character: " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerSpanishWaeHandler::illegalCharacters (string theString)
{
  gLogStream <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    endl;
}

void msdlScannerSpanishWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  gLogStream <<
    "### a digit is expected after decimal dot, found: " <<
    "'" << theChar << "' ###" <<
    endl;
}
void msdlScannerSpanishWaeHandler::digitExpectedInExponent (char theChar)
{
  gLogStream <<
    "### a digit is expected in exponent, found: " <<
    "'" << theChar << "' " <<
    endl;
}

void msdlScannerSpanishWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  gLogStream <<
    "### illegal escaped character after '\': " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerSpanishWaeHandler::inputIsLexicallyCorrect (int numberOfTokens)
{
  gLogStream <<
    "The MSDL input is lexically correct. It contains " <<
    numberOfTokens <<
    " tokens" <<
    endl;
}

void msdlScannerSpanishWaeHandler::inputIsLexicallyIncorrect (int numberOfTokens)
{
  gLogStream <<
    "### The MSDL input is lexically incorrect. It contains " <<
    numberOfTokens <<
    " tokens ###" <<
    endl;
}

void msdlScannerSpanishWaeHandler::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsTranslationLanguage)
{
  gLogStream <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsTranslationLanguage) <<
    "\", there is no need to translate it ###" <<
    endl;
}

string msdlScannerSpanishWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlScannerSpanishWaeHandler::print (ostream& os) const
{
  os <<
    "msdlScannerSpanishWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlScannerSpanishWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlScannerDutchWaeHandler msdlScannerDutchWaeHandler::create ()
{
  msdlScannerDutchWaeHandler* o =
    new msdlScannerDutchWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlScannerDutchWaeHandler::msdlScannerDutchWaeHandler ()
{}

msdlScannerDutchWaeHandler::~msdlScannerDutchWaeHandler ()
{}

void msdlScannerDutchWaeHandler::illegalCharacter (char theChar)
{
  gLogStream <<
    "### illegal character: " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerDutchWaeHandler::illegalCharacters (string theString)
{
  gLogStream <<
    "### illegal characters: " <<
    "\"" << theString << "\" ###" <<
    endl;
}

void msdlScannerDutchWaeHandler::digitExpectedAfterDecimalDot (char theChar)
{
  gLogStream <<
    "### a digit is expected after decimal dot, found: " <<
    "'" << theChar << "' ###" <<
    endl;
}
void msdlScannerDutchWaeHandler::digitExpectedInExponent (char theChar)
{
  gLogStream <<
    "### a digit is expected in exponent, found: " <<
    "'" << theChar << "' " <<
    endl;
}

void msdlScannerDutchWaeHandler::illegalEscapedCharacterInString (char theChar)
{
  gLogStream <<
    "### illegal escaped character after '\': " <<
    "'" << theChar << "' ###" <<
    endl;
}

void msdlScannerDutchWaeHandler::inputIsLexicallyCorrect (int numberOfTokens)
{
  gLogStream <<
    "The MSDL input is lexically correct. It contains " <<
    numberOfTokens <<
    " tokens" <<
    endl;
}

void msdlScannerDutchWaeHandler::inputIsLexicallyIncorrect (int numberOfTokens)
{
  gLogStream <<
    "### The MSDL input is lexically incorrect. It contains " <<
    numberOfTokens <<
    " tokens ###" <<
    endl;
}

void msdlScannerDutchWaeHandler::inputIsAlreadyInMsdlKeywordsLanguage (
  msdlKeywordsLanguageKind
    keywordsTranslationLanguage)
{
  gLogStream <<
    "### The MSDL input is already in keywords language \"" <<
    msdlKeywordsLanguageKindAsString (keywordsTranslationLanguage) <<
    "\", there is no need to translate it ###" <<
    endl;
}

string msdlScannerDutchWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlScannerDutchWaeHandler::print (ostream& os) const
{
  os <<
    "msdlScannerDutchWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlScannerDutchWaeHandler& elt)
{
  elt->print (os);
  return os;
}



} // namespace
