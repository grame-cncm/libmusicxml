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

#include "msdrTokensList.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
string msdrTokenDescrKindAsString (msdrTokenDescrKind tokenDescrKind)
{
  string result;

  switch (tokenDescrKind) {
    case kTokenDescrInteger:
      result = "tokenDescrInteger";
      break;
    case kTokenDescrDouble:
      result = "tokenDescrDouble";
      break;
    case kTokenDescrString:
      result = "tokenDescrString";
      break;
  } // switch

  return result;
}

//_______________________________________________________________________________
int msdrTokenDescr::getInt () const
{
  int result;

  switch (fTokenDescrKind) {
    case kTokenDescrInteger:
      result = fInteger;
      break;

    case kTokenDescrDouble:
      {
        string message =
          "MSDL token desr contains a double, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescrString:
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

double msdrTokenDescr::getDouble () const
{
  double result;

  switch (fTokenDescrKind) {
    case kTokenDescrInteger:
      result = fInteger;
      break;

    case kTokenDescrDouble:
      {
        string message =
          "MSDL token desr contains a double, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescrString:
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

string msdrTokenDescr::getString () const
{
  string result;

  switch (fTokenDescrKind) {
    case kTokenDescrInteger:
      result = fInteger;
      break;

    case kTokenDescrDouble:
      {
        string message =
          "MSDL token desr contains a double, not an integer";

        gLogStream <<
          message <<
          endl;

        throw msgMsdlException (message);
      }
      break;

    case kTokenDescrString:
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

string msdrTokenDescr::asString () const
{
  stringstream s;

  s <<
    "MSDL token" <<
    ", tokenDescrKind = " <<
    msdrTokenDescrKindAsString (fTokenDescrKind) <<
    ": ";

  switch (fTokenDescrKind) {
    case kTokenDescrInteger:
      s << fInteger;
      break;

    case kTokenDescrDouble:
      s << fDouble;
      break;

    case kTokenDescrString:
      s << "\"" << fString << "\"";
      break;
  } // switch

  return s.str ();
}

void msdrTokenDescr::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const msdrTokenDescr& elt)
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

string msdrTokensList::asString () const
{
  stringstream s;

  s <<
    "MSDL tokens list" <<
    singularOrPluralWithoutNumber (
      fMsdrTokenDescrList.size (), "element", "elements");

  return s.str ();
}

void msdrTokensList::print (ostream& os) const
{
  os <<
    "MsdrTokensList" <<
    endl;

  ++gIndenter;

  unsigned
    msdrTokenDescrListSize =
      fMsdrTokenDescrList.size ();

  if (msdrTokenDescrListSize) {
    ++gIndenter;

    list<msdrTokenDescr>::const_iterator
      iBegin = fMsdrTokenDescrList.begin (),
      iEnd   = fMsdrTokenDescrList.end (),
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
