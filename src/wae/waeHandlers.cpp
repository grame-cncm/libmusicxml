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

// #include "msdlOah.h"
#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdlKeywords.h"
#include "msdlTokens.h"

#include "waeHandlers.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_waeHandler waeHandler::create ()
{
  waeHandler* o =
    new waeHandler ();
  assert (o != nullptr);
  return o;
}

waeHandler::waeHandler ()
{}

waeHandler::~waeHandler ()
{}

string waeHandler::asString () const
{
  stringstream s;

  s <<
    "WAE handler";

  return s.str ();
}

void waeHandler::print (ostream& os) const
{
  os <<
    "WaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_waeHandler& elt)
{
  elt->print (os);
  return os;
}


} // namespace
