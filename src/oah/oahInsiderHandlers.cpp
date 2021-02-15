/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "enableExtraOahIfDesired.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "waeMessagesHandling.h"

#include "oahInsiderHandlers.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
/* this class is purely virtual
S_oahInsiderHandler oahInsiderHandler::create (
  const string& executableName,
  const string& handlerHeader,
  const string& handlerDescription,
  const string& handlerUsage)
{
  // create the insider handler
  oahInsiderHandler* o = new
    oahInsiderHandler (
      executableName,
      handlerHeader,
      handlerDescription,
      handlerUsage);
  assert (o != nullptr);

  return o;
}
*/

oahInsiderHandler::oahInsiderHandler (
  const string& executableName,
  const string& handlerHeader,
  const string& handlerDescription,
  const string& handlerUsage)
  : oahHandler (
      executableName,
      handlerHeader,
      handlerDescription,
      handlerUsage)
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "Initializing \"" <<
      fHandlerHeader <<
      "\" insider handler" <<
      endl;
  }
#endif
}

oahInsiderHandler::~oahInsiderHandler ()
{}

void oahInsiderHandler::handlerOahError (
  const string& errorMessage)
{
  oahError (errorMessage);
}

void oahInsiderHandler::print (ostream& os) const
{
  const unsigned int fieldWidth = 27;

  os <<
    "oahInsiderHandler:" <<
    endl;

  ++gIndenter;

  printHandlerEssentials (
    os, fieldWidth);
  os << endl;

  os <<
    "Options groups (" <<
    singularOrPlural (
      fHandlerGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fHandlerGroupsList.size ()) {
    os << endl;

    ++gIndenter;

    list<S_oahGroup>::const_iterator
      iBegin = fHandlerGroupsList.begin (),
      iEnd   = fHandlerGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    --gIndenter;
  }

  --gIndenter;

  os << endl;
}

ostream& operator<< (ostream& os, const S_oahInsiderHandler& elt)
{
  elt->print (os);
  return os;
}


}
