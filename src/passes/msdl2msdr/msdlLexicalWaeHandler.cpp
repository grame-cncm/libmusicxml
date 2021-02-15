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

#include "msdlLexicalWaeHandler.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
/* this class is purely virtual
S_msdlLexicalWaeHandler msdlLexicalWaeHandler::create ()
{
  msdlLexicalWaeHandler* o =
    new msdlLexicalWaeHandler ();
  assert (o != nullptr);
  return o;
}
*/

msdlLexicalWaeHandler::msdlLexicalWaeHandler ()
{}

msdlLexicalWaeHandler::~msdlLexicalWaeHandler ()
{}

string msdlLexicalWaeHandler::asString () const
{
  stringstream s;

  s <<
    "MSDL WAE lexical handler";

  return s.str ();
}

void msdlLexicalWaeHandler::print (ostream& os) const
{
  os <<
    "msdlLexicalWaeHandler" <<
    endl;
}

ostream& operator<< (ostream& os, const S_msdlLexicalWaeHandler& elt)
{
  elt->print (os);
  return os;
}


} // namespace
