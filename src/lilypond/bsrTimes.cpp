/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>

#include "bsrTimes.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "bsrOptions.h"
#include "xml2brlOptionsHandling.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrTime bsrTime::create (
  int inputLineNumber)
{
  bsrTime* o =
    new bsrTime (
      inputLineNumber);
  assert (o!=0);
  return o;
}

bsrTime::bsrTime (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{}

bsrTime::~bsrTime ()
{}

void bsrTime::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTime::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrTime>*
    p =
      dynamic_cast<visitor<S_bsrTime>*> (v)) {
        S_bsrTime elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTime::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrTime::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTime::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrTime>*
    p =
      dynamic_cast<visitor<S_bsrTime>*> (v)) {
        S_bsrTime elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTime::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrTime::browseData (basevisitor* v)
{}

string bsrTime::asString () const
{
  stringstream s;

  s <<
    "Time" << 
    ", line " << fInputLineNumber;
 
  return s.str ();
}

void bsrTime::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrTime& elt)
{
  elt->print (os);
  return os;
}


}
