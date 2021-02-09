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

#include "messagesHandling.h"

#include "msr2summaryVisitor.h"
*/

#include "exceptions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

// #include "msdrOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdrTokens.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
string msdrTokenKindAsString (msdrTokenKind tokenKind)
{
  string result;

  switch (tokenKind) {
    case k_NoToken:
      result = "*noToken*";
      break;

    case kTokenTitle:
      result = "kTokenTitle";
      break;
    case kTokenComposer:
      result = "kTokenComposer";
      break;
    case kTokenOpus:
      result = "kTokenOpus";
      break;

    case kTokenPitches:
      result = "kTokenPitches";
      break;

    case kTokenScore:
      result = "kTokenScore";
      break;
    case kTokenPartGroup:
      result = "kTokenPartGroup";
      break;
    case kTokenPart:
      result = "kTokenPart";
      break;
    case kTokenStaff:
      result = "kTokenStaff";
      break;
    case kTokenVoice:
      result = "kTokenVoice";
      break;
    case kTokenFragment:
      result = "kTokenFragment";
      break;

    case kTokenClef:
      result = "kTokenClef";
      break;

    case kTokenKey:
      result = "kTokenKey";
      break;
    case kTokenTreble:
      result = "kTokenTreble";
      break;
    case kTokenSoprano:
      result = "kTokenSoprano";
      break;
    case kTokenAlto:
      result = "kTokenAlto";
      break;
    case kTokenBaritone:
      result = "kTokenBaritone";
      break;
    case kTokenBass:
      result = "kTokenBass";
      break;

    case kTokenTime:
      result = "kTokenTime";
      break;

    case kTokenAnacrusis:
      result = "kTokenAnacrusis";
      break;

    case kTokenFinalBar:
      result = "kTokenFinalBar";
      break;
  } // switch

  return result;
}

string msdrTokenDescriptionKindAsString (msdrTokenDescriptionKind tokenDescriptionKind)
{
  string result;

  switch (tokenDescriptionKind) {
    case kTokenDescriptionInteger:
      result = "tokenDescriptionInteger";
      break;
    case kTokenDescriptionDouble:
      result = "tokenDescriptionDouble";
      break;
    case kTokenDescriptionString:
      result = "tokenDescriptionString";
      break;
  } // switch

  return result;
}

//_______________________________________________________________________________
S_msdrTokenDescription msdrTokenDescription::create ()
{
  msdrTokenDescription* o =
    new msdrTokenDescription ();
  assert (o != nullptr);
  return o;
}

msdrTokenDescription::msdrTokenDescription ()
{}

msdrTokenDescription::~msdrTokenDescription ()
{}

int msdrTokenDescription::getInt () const
{
  int result = 0;

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionInteger:
      result = fInteger;
      break;

    case kTokenDescriptionDouble:
      {
        string message =
          "MSDL token desr contains a double, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionString:
      {
        string message =
          "MSDL token desr contains a string, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;
  } // switch

  return result;
}

double msdrTokenDescription::getDouble () const
{
  double result = 0.0;

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionInteger:
      result = fInteger;
      break;

    case kTokenDescriptionDouble:
      {
        string message =
          "MSDL token desr contains a double, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionString:
      {
        string message =
          "MSDL token desr contains a string, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;
  } // switch


  return result;
}

string msdrTokenDescription::getString () const
{
  string result;

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionInteger:
      result = fInteger;
      break;

    case kTokenDescriptionDouble:
      {
        string message =
          "MSDL token desr contains a double, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescriptionString:
      {
        string message =
          "MSDL token desr contains a string, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;
  } // switch

  return result;
}

string msdrTokenDescription::asString () const
{
  stringstream s;

  s <<
    "MSDL token" <<
    ", tokenDescriptionKind = " <<
    msdrTokenDescriptionKindAsString (fTokenDescriptionKind) <<
    ": ";

  switch (fTokenDescriptionKind) {
    case kTokenDescriptionInteger:
      s << fInteger;
      break;

    case kTokenDescriptionDouble:
      s << fDouble;
      break;

    case kTokenDescriptionString:
      s << "\"" << fString << "\"";
      break;
  } // switch

  return s.str ();
}

void msdrTokenDescription::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const msdrTokenDescription& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
msdrToken::msdrToken (
  msdrTokenKind tokenKind)
    : fTokenKind (tokenKind)
{}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  int           value)
    : fTokenKind (tokenKind)
{
  fTokenDescription.setInt (value);
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  string        value)
    : fTokenKind (tokenKind)
{
  fTokenDescription.setString (value);
}

msdrToken::msdrToken (
  msdrTokenKind tokenKind,
  double        value)
    : fTokenKind (tokenKind)
{
  fTokenDescription.setDouble (value);
}

msdrToken::~msdrToken ()
{}

string msdrToken::asString () const
{
  stringstream s;

  s <<
    "MSDL token" <<
    ", tokenKind: " <<
    msdrTokenKindAsString (fTokenKind) <<
    ", fTokenDescription: " <<
    fTokenDescription.asString ();

  return s.str ();
}

void msdrToken::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const msdrToken& elt)
{
  elt.print (os);
  return os;
}

//_______________________________________________________________________________
S_msdrTokensList msdrTokensList::create ()
{
  msdrTokensList* o =
    new msdrTokensList ();
  assert (o != nullptr);
  return o;
}

msdrTokensList::msdrTokensList ()
{}

msdrTokensList::~msdrTokensList ()
{}

void msdrTokensList::appendTokenToTokensList (
  const msdrToken& token)
{
  fTokensList.push_back (token);
}

string msdrTokensList::asString () const
{
  stringstream s;

  s <<
    "MSDL tokens list" <<
    singularOrPluralWithoutNumber (
      fTokensList.size (), "element", "elements");

  return s.str ();
}

void msdrTokensList::print (ostream& os) const
{
  os <<
    "MsdrTokensList" <<
    endl;

  ++gIndenter;

  unsigned
    tokensListSize =
      fTokensList.size ();

  if (tokensListSize) {
    ++gIndenter;

    list<msdrToken>::const_iterator
      iBegin = fTokensList.begin (),
      iEnd   = fTokensList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << ((*i));
      if (++i == iEnd) break;
 // JMI      os << endl;
    } // for

    --gIndenter;
  }
  else {
    os << "empty" << endl;
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msdrTokensList& elt)
{
  elt->print (os);
  return os;
}


} // namespace
