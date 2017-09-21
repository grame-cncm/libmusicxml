/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>

#include <climits>      /* INT_MIN, INT_MAX */
#include <iomanip>      // setw, set::precision, ...

#include "xml2lilypondOptionsHandling.h"

#include "musicXMLOptions.h"

#include "utilities.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

#define TRACE_OPTIONS 0

//______________________________________________________________________________
S_xml2lilypondOptionsHandler xml2lilypondOptionsHandler::create (
  string optionHandlerShortName,
  string optionHandlerLongName,
  string optionHandlerDescription)
{
  xml2lilypondOptionsHandler* o = new
    xml2lilypondOptionsHandler (
      optionHandlerShortName,
      optionHandlerLongName,
      optionHandlerDescription);
  assert(o!=0);
  return o;
}

xml2lilypondOptionsHandler::xml2lilypondOptionsHandler (
  string optionHandlerShortName,
  string optionHandlerLongName,
  string optionHandlerDescription)
  : msrOptionsHandler (
      optionHandlerShortName,
      optionHandlerLongName,
      optionHandlerDescription)
{
  initializeOptionsHandler ();
}

xml2lilypondOptionsHandler::~xml2lilypondOptionsHandler()
{}

void xml2lilypondOptionsHandler::initializeOptionsHandler ()
{
  // MusicXML options
  S_msrMusicXMLOptions
    musicXMLOptions =
      msrMusicXMLOptions::create ();

  appendOptionsGroup (musicXMLOptions);

  if (TRACE_OPTIONS) {
    // print the options handler initial state
    cerr << idtr <<
      "xml2lilypondOptionsHandler has been initialized as:" <<
      endl;
  
    idtr++;
  
    print (cerr);
    cerr <<
      endl <<
      endl;
    
    idtr--;
  }

  // register options handler in itself
  this->
    registerOptionsHandlerInSelf ();

  if (true || TRACE_OPTIONS) { // JMI
    cerr << idtr <<
      "xml2lilypondOptionsHandler help:" <<
      endl;

    this->
      printHelp (cerr);
  }
}

void xml2lilypondOptionsHandler::print (ostream& os) const
{
  const int fieldWidth = 27;
  
  os << idtr <<
    "xml2lilypondOptionsHandler:" <<
    endl;

  idtr++;

  os <<
    idtr << left <<
      setw(fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw(fieldWidth) <<
      "fOptionsElementDescription" << " : " << fOptionsElementDescription <<
      endl <<
    endl;

  os << idtr <<
    "Options groups (" <<
    singularOrPlural (
      fOptionsHandlerOptionsGroupsList.size (), "element",  "elements") <<
    "):" <<
    endl;

  if (fOptionsHandlerOptionsGroupsList.size ()) {
    os << endl;
    
    idtr++;

    list<S_msrOptionsGroup>::const_iterator
      iBegin = fOptionsHandlerOptionsGroupsList.begin(),
      iEnd   = fOptionsHandlerOptionsGroupsList.end(),
      i      = iBegin;
    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      cerr << endl;
    } // for

    idtr--;
  }

  idtr--;
  
  os << endl;
}

ostream& operator<< (ostream& os, const S_xml2lilypondOptionsHandler& elt)
{
  elt->print (os);
  return os;
}


}
