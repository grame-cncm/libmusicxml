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

#include "msdlParserWaeHandlers.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
/* this class is purely virtual
S_msdlParserWaeHandler msdlParserWaeHandler::create ()
{
  msdlParserWaeHandler* o =
    new msdlParserWaeHandler ();
  assert (o != nullptr);
  return o;
}
*/

msdlParserWaeHandler::msdlParserWaeHandler ()
{}

msdlParserWaeHandler::~msdlParserWaeHandler ()
{}

string msdlParserWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL WAE lexical handler";

  return s.str ();
}

void msdlParserWaeHandler::print (ostream& os) const
{
  os <<
    "msdlParserWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlParserWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlParserEnglishWaeHandler msdlParserEnglishWaeHandler::create ()
{
  msdlParserEnglishWaeHandler* o =
    new msdlParserEnglishWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserEnglishWaeHandler::msdlParserEnglishWaeHandler ()
{}

msdlParserEnglishWaeHandler::~msdlParserEnglishWaeHandler ()
{}

void msdlParserEnglishWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    endl;
}

void msdlParserEnglishWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    endl;
}

void msdlParserEnglishWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    endl;
}

void msdlParserEnglishWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    endl;
}

void msdlParserEnglishWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserEnglishWaeHandler::ignoringToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserEnglishWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    endl;
}

void msdlParserEnglishWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    endl;
}

void msdlParserEnglishWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    endl;
}

void msdlParserEnglishWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    endl;
}

string msdlParserEnglishWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserEnglishWaeHandler::print (ostream& os) const
{
  os <<
    "msdlParserEnglishWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlParserEnglishWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlParserFrenchWaeHandler msdlParserFrenchWaeHandler::create ()
{
  msdlParserFrenchWaeHandler* o =
    new msdlParserFrenchWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserFrenchWaeHandler::msdlParserFrenchWaeHandler ()
{}

msdlParserFrenchWaeHandler::~msdlParserFrenchWaeHandler ()
{}

void msdlParserFrenchWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### une seule section d'identification est possible, au début de la spécification ###" <<
    endl;
}

void msdlParserFrenchWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### une chaîne est attendue comme titre ###" <<
    endl;
}

void msdlParserFrenchWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### une chaîne est attendue comme compositeur-trice ###" <<
    endl;
}

void msdlParserFrenchWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### une chaîne est attendue comme opus ###" <<
    endl;
}

void msdlParserFrenchWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  string           context) const
{

  gLogStream <<
    "### " << context << ": " <<
    "terminal inattendu: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserFrenchWaeHandler::ignoringToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "on ignore le terminal: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserFrenchWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "Le texte MSDL est syntaxiquement correct" <<
    endl;
}

void msdlParserFrenchWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### Le texte MSDL est syntaxiquement incorrect ###" <<
    endl;
}

void msdlParserFrenchWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### On ne peut pas mélanger les virgules et apostrophes dans les indications d'octave ###" <<
    endl;
}

void msdlParserFrenchWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Durée de note mal formée ###" <<
    endl;
}

string msdlParserFrenchWaeHandler::asString () const
{
  stringstream s;

  s <<
    "gestionnaire WAE pour MSDL en français";

  return s.str ();
}

void msdlParserFrenchWaeHandler::print (ostream& os) const
{
  os <<
    "msdlParserFrenchWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlParserFrenchWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlParserItalianWaeHandler msdlParserItalianWaeHandler::create ()
{
  msdlParserItalianWaeHandler* o =
    new msdlParserItalianWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserItalianWaeHandler::msdlParserItalianWaeHandler ()
{}

msdlParserItalianWaeHandler::~msdlParserItalianWaeHandler ()
{}

void msdlParserItalianWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    endl;
}

void msdlParserItalianWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    endl;
}

void msdlParserItalianWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    endl;
}

void msdlParserItalianWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    endl;
}

void msdlParserItalianWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserItalianWaeHandler::ignoringToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserItalianWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    endl;
}

void msdlParserItalianWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    endl;
}

void msdlParserItalianWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    endl;
}

void msdlParserItalianWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    endl;
}

string msdlParserItalianWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserItalianWaeHandler::print (ostream& os) const
{
  os <<
    "msdlParserItalianWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlParserItalianWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlParserGermanWaeHandler msdlParserGermanWaeHandler::create ()
{
  msdlParserGermanWaeHandler* o =
    new msdlParserGermanWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserGermanWaeHandler::msdlParserGermanWaeHandler ()
{}

msdlParserGermanWaeHandler::~msdlParserGermanWaeHandler ()
{}

void msdlParserGermanWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    endl;
}

void msdlParserGermanWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    endl;
}

void msdlParserGermanWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    endl;
}

void msdlParserGermanWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    endl;
}

void msdlParserGermanWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserGermanWaeHandler::ignoringToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserGermanWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    endl;
}

void msdlParserGermanWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    endl;
}

void msdlParserGermanWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    endl;
}

void msdlParserGermanWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    endl;
}

string msdlParserGermanWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserGermanWaeHandler::print (ostream& os) const
{
  os <<
    "msdlParserGermanWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlParserGermanWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlParserSpanishWaeHandler msdlParserSpanishWaeHandler::create ()
{
  msdlParserSpanishWaeHandler* o =
    new msdlParserSpanishWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserSpanishWaeHandler::msdlParserSpanishWaeHandler ()
{}

msdlParserSpanishWaeHandler::~msdlParserSpanishWaeHandler ()
{}

void msdlParserSpanishWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    endl;
}

void msdlParserSpanishWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    endl;
}

void msdlParserSpanishWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    endl;
}

void msdlParserSpanishWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    endl;
}

void msdlParserSpanishWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserSpanishWaeHandler::ignoringToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserSpanishWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    endl;
}

void msdlParserSpanishWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    endl;
}

void msdlParserSpanishWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    endl;
}

void msdlParserSpanishWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    endl;
}

string msdlParserSpanishWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserSpanishWaeHandler::print (ostream& os) const
{
  os <<
    "msdlParserSpanishWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlParserSpanishWaeHandler& elt)
{
  elt->print (os);
  return os;
}

//_______________________________________________________________________________
S_msdlParserDutchWaeHandler msdlParserDutchWaeHandler::create ()
{
  msdlParserDutchWaeHandler* o =
    new msdlParserDutchWaeHandler ();
  assert (o != nullptr);
  return o;
}

msdlParserDutchWaeHandler::msdlParserDutchWaeHandler ()
{}

msdlParserDutchWaeHandler::~msdlParserDutchWaeHandler ()
{}

void msdlParserDutchWaeHandler::multipleIdentifications () const
{
  gLogStream <<
    "### only one identification section is possible, ahead of the specfication ###" <<
    endl;
}

void msdlParserDutchWaeHandler::stringExpectedAsTitle () const
{
  gLogStream <<
    "### a string is expected as title ###" <<
    endl;
}

void msdlParserDutchWaeHandler::stringExpectedAsComposer () const
{
  gLogStream <<
    "### a string is expected as composer ###" <<
    endl;
}

void msdlParserDutchWaeHandler::stringExpectedAsOpus () const
{
  gLogStream <<
    "### a string is expected as opus ###" <<
    endl;
}

void msdlParserDutchWaeHandler::unexpectedToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "unexpected token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserDutchWaeHandler::ignoringToken (
  const msdlToken& theToken,
  string           context) const
{
  gLogStream <<
    "### " << context << ": " <<
    "ignoring token: " <<
    "'" << theToken.asString () <<  "'" <<
    " ###" <<
    endl;
}

void msdlParserDutchWaeHandler::inputIsSyntacticallyCorrect () const
{
  gLogStream <<
    "The MSDL input is syntactically correct" <<
    " tokens" <<
    endl;
}

void msdlParserDutchWaeHandler::inputIsSyntacticallyIncorrect () const
{
  gLogStream <<
    "### The MSDL input is syntactically incorrect ###" <<
    endl;
}

void msdlParserDutchWaeHandler::mixedCommasAndQuotesInOctaveIndication () const
{
  gLogStream <<
    "### Commas and quotes cannot be mixed in octave inditations ###" <<
    endl;
}

void msdlParserDutchWaeHandler::malformedNoteDuration () const
{
  gLogStream <<
    "### Malformed note duration ###" <<
    endl;
}

string msdlParserDutchWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL lexical english WAE handler";

  return s.str ();
}

void msdlParserDutchWaeHandler::print (ostream& os) const
{
  os <<
    "msdlParserDutchWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlParserDutchWaeHandler& elt)
{
  elt->print (os);
  return os;
}



} // namespace
