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

#include "generalOptions.h"
#include "musicXMLOptions.h"
#include "msrOptions.h"
#include "lpsrOptions.h"
#include "lilypondOptions.h"

#include "utilities.h"


using namespace std;

namespace MusicXML2 
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

#define TRACE_OPTIONS 0

//______________________________________________________________________________
S_xml2lilypondOptionsHandler xml2lilypondOptionsHandler::create ()
{
  xml2lilypondOptionsHandler* o = new
    xml2lilypondOptionsHandler ();
  assert(o!=0);
  return o;
}

xml2lilypondOptionsHandler::xml2lilypondOptionsHandler ()
  : msrOptionsHandler (
    "Available options",
    "Options values",
    "h", "help",
R"(
                Welcome to xml2lilypond v0.1.0, 
              the MusicXML to LilyPond translator
          delivered as part of the libmusicxml2 library.
      https://github.com/grame-cncm/libmusicxml/tree/lilypond

Usage:

    xml2lilypond [options] [MusicXMLFile|-]

What it does:

    This multi-pass translator basically performs 4 passes:
        Pass 1: reads the contents of MusicXMLFile or stdin ('-')
                and converts it to a MusicXML tree;
        Pass 2: converts that tree to a Music Score Representation (MSR);
        Pass 3: converts the MSR to a LilyPond Score Representation (LPSR);
        Pass 4: converts the LPSR to LilyPond source code 
                and writes it to standard output.

    Other passes are performed according to the options, such as
    printing views of the internal data or printing a summary of the score.

    A number of options exist to fine tune the generated LilyPond code
    and limit the need for manually editing the latter.
    They are organized in a group-subgroup-item hierarchy,
    and help can be obtained for groups or subgroups at will.
    Both '-' and '--' can be used to introduce an option in the command line.
    We use '-' to describe the options in the help,
    but you can use '--' if you so prefer.
    
    The activity log and warning/error messages go to standard error.
)"
      )
{
  initializeOptionsHandler ();
}

xml2lilypondOptionsHandler::~xml2lilypondOptionsHandler()
{}

void xml2lilypondOptionsHandler::initializeOptionsHandler ()
{
  // append the various options groups
  appendOptionsGroup (
    gGeneralOptions);
    
  appendOptionsGroup (
    gMusicXMLOptions);
    
  appendOptionsGroup (
    gMsrOptions);
    
  appendOptionsGroup (
    gLpsrOptions);
    
  appendOptionsGroup (
    gLilypondOptions);

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

  // register options handler in itself,
  // so that the 'global' help options can be handled
  this->
    registerOptionsHandlerInSelf ();

  if (TRACE_OPTIONS) {
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
      setw (fieldWidth) <<
      "fOptionsElementShortName" << " : " << fOptionsElementShortName <<
      endl <<
    idtr <<
      setw (fieldWidth) <<
      "fOptionsElementLongName" << " : " << fOptionsElementLongName <<
      endl <<
    idtr <<
      setw (fieldWidth) <<
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
