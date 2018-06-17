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

/*
#include <climits>      / * INT_MIN, INT_MAX * /

#include "utilities.h"

#include "messagesHandling.h"
#include "generalOptions.h"

#include "xml2lyOptionsHandling.h"
*/

#include "traceOptions.h"

#include "msrStreams.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
ostream& operator<< (ostream& os, const S_msrStreamPartGroup& elt)
{
 // elt->print (os);
  os << "msrStreamPartGroup" << endl;
  return os;
}

//______________________________________________________________________________
S_msrStreamScore msrStreamScore::create (
  string scoreTitle)
{
  msrStreamScore* o =
    new msrStreamScore (
      scoreTitle);
  assert(o!=0);
  return o;
}

msrStreamScore::msrStreamScore (
  string scoreTitle)
{
  fScoreTitle = scoreTitle;
  
  // create the msrScore
  fMsrScore = 
    msrScore::create (
      0); // inputLineNumber
}

msrStreamScore::~msrStreamScore ()
{}

void msrStreamScore::addStreamPartGroupToScore (
  S_msrStreamPartGroup streamPartGroup)
{
  if (fStreamPartGroupsSet.count (streamPartGroup)) {      
    stringstream s;
    
    s <<      
      "stream part group '" <<
 //     streamPartGroup->getPartGroupCombinedName () <<
      "' already exists in this score";

    msrStreamsError (
     33, // streamPartGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register it in this score
  fStreamPartGroupsSet.insert (streamPartGroup);
  fStreamPartGroupsList.push_back (streamPartGroup);
}

void msrStreamScore::appendCreditToScore (S_msrCredit credit)
{
  if (gTraceOptions->fTraceMsrStreams) {
    gLogIOstream <<
      "Appending credit '" <<
      credit->asString () <<
      "' to stream score" <<
      endl;
  }
  
  fMsrScore->appendCreditToScore (credit);
}

ostream& operator<< (ostream& os, const S_msrStreamScore& elt)
{
  elt->print (os);
  return os;
}

void msrStreamScore::print (ostream& os)
{
  os <<
    "MSR Stream Score" <<
    endl <<
    "title : " << fScoreTitle <<
    endl;

  gIndenter++;

  const int fieldWidth = 38;

  int partGroupsListSize =
    fStreamPartGroupsList.size ();
    
  os << left <<
    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      partGroupsListSize,
      "stream part group", "stream part groups") <<
    " : " <<
    partGroupsListSize <<
    endl <<
    endl;

  // print the part groups if any
  if (fStreamPartGroupsList.size ()) {
    list<S_msrStreamPartGroup>::const_iterator
      iBegin = fStreamPartGroupsList.begin (),
      iEnd   = fStreamPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  else {
    os <<
      "There are no stream part groups in the list" <<
      endl;
  }
  
  gIndenter--;
}

//______________________________________________________________________________
void initializeMSRStreams ()
{
}


}
