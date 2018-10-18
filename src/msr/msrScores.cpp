/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrScores.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"

#include "xml2lyOptionsHandling.h"


using namespace std;


namespace MusicXML2 
{

//______________________________________________________________________________
S_msrScore msrScore::create (
  int inputLineNumber)
{
  msrScore* o =
    new msrScore (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrScore::msrScore (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  // create the identification
  fIdentification =
    msrIdentification::create (
      inputLineNumber);

  // create the page geometry
  fPageGeometry =
    msrPageGeometry::create (
      inputLineNumber);

  // number of measures
  fScoreNumberOfMeasures = -1;

  // part group names max length
  fScorePartGroupNamesMaxLength = -1;

  // part names max length
  fScorePartNamesMaxLength = -1;
  
  // set instrument names max lengthes
  fScoreInstrumentNamesMaxLength      = -1;
  fScoreInstrumentAbbreviationsMaxLength = -1;
  
  // measure repeats replicas should be browsed by default
  fInhibitMeasuresRepeatReplicasBrowsing = false;

  // multiple rest measures should be browsed by default
  fInhibitMultipleRestMeasuresBrowsing = false;
}

msrScore::~msrScore ()
{}

S_msrScore msrScore::createScoreNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceScore) {
    gLogIOstream <<
      "Creating a newborn clone of a score" <<
      endl;
  }
#endif

  S_msrScore
    newbornClone =
      msrScore::create (
        fInputLineNumber);

  // number of measures
  newbornClone->fScoreNumberOfMeasures =
    fScoreNumberOfMeasures;

  // part group names max length

  newbornClone->fScorePartGroupNamesMaxLength =
    fScorePartGroupNamesMaxLength;

  // part names max length

  newbornClone->fScorePartNamesMaxLength =
    fScorePartNamesMaxLength;

  // instrument names max lengthes
  
  newbornClone->fScoreInstrumentNamesMaxLength =
    fScoreInstrumentNamesMaxLength;
    
  newbornClone->fScoreInstrumentAbbreviationsMaxLength =
    fScoreInstrumentAbbreviationsMaxLength;
    
  // inhibiting browsing

  newbornClone->fInhibitMeasuresRepeatReplicasBrowsing =
    fInhibitMeasuresRepeatReplicasBrowsing;
    
  newbornClone->fInhibitMultipleRestMeasuresBrowsing =
    fInhibitMultipleRestMeasuresBrowsing;
    
  return newbornClone;
}

void msrScore::addPartGroupToScore (S_msrPartGroup partGroup)
{
  if (fScorePartGroupsSet.count (partGroup)) {      
    stringstream s;
    
    s <<      
      "part group '" <<
      partGroup->getPartGroupCombinedName () <<
      "' already exists in this score";

    msrInternalError (
      gXml2lyOptions->fInputSourceName,
      partGroup->getInputLineNumber (),
      __FILE__, __LINE__,
      s.str ());
  }

  // register it in this score
  fScorePartGroupsSet.insert (partGroup);
  fPartGroupsList.push_back (partGroup);
}

void msrScore::appendCreditToScore (S_msrCredit credit)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceCredits) {
    gLogIOstream <<
      "Appending credit '" <<
      credit->asString () <<
      "' to score" <<
      endl;
  }
#endif
  
  fCreditsList.push_back (credit);
}

S_msrPart msrScore::fetchPartFromScoreByItsPartID (
  int    inputLineNumber,
  string partID)
{
  S_msrPart result;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePartGroupsDetails) {
    gLogIOstream <<
      "fetchPartFromScoreByItsPartID(" << partID << "), fPartGroupsList contains:" <<
      endl;
      
    gIndenter++;

    for (
      list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
        i != fPartGroupsList.end ();
        i++) {
      gLogIOstream <<
        (*i)->getPartGroupCombinedName () <<
        ", " <<
        (*i)->getPartGroupName () <<
        endl;
    } // for
  
    gIndenter--;

    gLogIOstream <<
      "<=- fetchPartFromScoreByItsPartID(" << partID << ")" <<
      endl <<
      endl;
  }
#endif

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++) {
    S_msrPart
      part =
        (*i)->
          fetchPartFromPartGroupByItsPartID (
            inputLineNumber,
            partID);

    if (part) {
      result = part;
      break;
    }
  } // for

  return result;
}

void msrScore::collectScorePartsList (
  int              inputLineNumber,
  list<S_msrPart>& partsList)
{
  S_msrPart result;

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++) {
    S_msrPartGroup
      partGroup = (*i);
      partGroup->
        collectPartGroupPartsList (
          inputLineNumber,
          partsList);
  } // for
}

/*
S_msrPartGroup msrScore::fetchScorePartGroup (
  int partGroupNumber)
{
  S_msrPartGroup result;
  
  if (fScorePartGroupsMap.count (partGroupNumber)) {
    result = fScorePartGroupsMap [partGroupNumber];
  }

  return result;
}
*/

void msrScore::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScore::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrScore::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrScore::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScore::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrScore>*
    p =
      dynamic_cast<visitor<S_msrScore>*> (v)) {
        S_msrScore elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrScore::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrScore::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrScore::browseData ()" <<
      endl;
  }
    
  if (fIdentification) {
    // browse identification
    msrBrowser<msrIdentification> browser (v);
    browser.browse (*fIdentification);
  }

  if (fPageGeometry) {
    // browse page geometry
    msrBrowser<msrPageGeometry> browser (v);
    browser.browse (*fPageGeometry);
  }

  for (
    list<S_msrCredit>::const_iterator i = fCreditsList.begin ();
    i != fCreditsList.end ();
    i++) {
    // browse the part credit
    msrBrowser<msrCredit> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrPartGroup>::const_iterator i = fPartGroupsList.begin ();
    i != fPartGroupsList.end ();
    i++) {
    // browse the part group
    msrBrowser<msrPartGroup> browser (v);
    browser.browse (*(*i));
  } // for

  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% <== msrScore::browseData ()" <<
      endl;
  }
}

void msrScore::print (ostream& os)
{
  os <<
    "MSR Score" <<
    endl;

  gIndenter++;

  const int fieldWidth = 38;

  int partGroupsListSize =
    fPartGroupsList.size ();
    
  os << left <<
    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      partGroupsListSize,
      "part group", "part groups") <<
    " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      fScoreNumberOfMeasures,
      "measure", "measures") <<
    " : " <<
    fScoreNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "scorePartGroupNamesMaxLength" <<  " : " <<
    fScorePartGroupNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scorePartNamesMaxLength" <<  " : " <<
    fScorePartNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fScoreInstrumentNamesMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitMultipleRestMeasuresBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitMultipleRestMeasuresBrowsing) <<
    endl<<
    endl;

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }
  
  // print the page geometry if any
  if (fPageGeometry) {
    os <<
      fPageGeometry;
  }

  // print the credits if any
  int creditsListSize = fCreditsList.size ();

  os <<
    setw (fieldWidth) <<
    "CreditsList";
  if (creditsListSize) {
    os <<
      endl;
    gIndenter++;

    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  else {
    os <<
      " : " << "none" <<
      endl;
  }
  os <<
    endl;

  // print the part groups if any
  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      // no endl here
    } // for
  }
  else {
    os <<
      "There are no part groups in the list" <<
      endl;
  }
  
  gIndenter--;
}

void msrScore::printSummary (ostream& os)
{
  os <<
    "MSR component" <<
    endl;

  gIndenter++;

  const int fieldWidth = 38;

  int partGroupsListSize =
    fPartGroupsList.size ();

  // print global information
  os << left <<
    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      partGroupsListSize,
      "part group", "part groups") <<
    " : " <<
    partGroupsListSize <<
    endl <<

    setw (fieldWidth) <<
    singularOrPluralWithoutNumber (
      fScoreNumberOfMeasures,
      "measure", "measures") <<
    " : " <<
    fScoreNumberOfMeasures <<
    endl <<

    setw (fieldWidth) <<
    "scoreInstrumentNamesMaxLength" <<  " : " <<
    fScoreInstrumentNamesMaxLength <<
    endl <<

    setw (fieldWidth) <<
    "scoreInstrumentAbbreviationsMaxLength" <<  " : " <<
    fScoreInstrumentAbbreviationsMaxLength <<
    endl<<

    setw (fieldWidth) <<
    "inhibitMultipleRestMeasuresBrowsing" <<  " : " <<
    booleanAsString (
      fInhibitMultipleRestMeasuresBrowsing) <<
    endl<<
    endl;

  // print the implicit part group contents
  // it is the only element in fPartGroupsList JMI single variable
  if (partGroupsListSize) {
    os <<
      "Parts and part groups structure:" <<
      endl;
  
    gIndenter++;
    
    fPartGroupsList.front () ->
      printPartGroupParts (
        fInputLineNumber,
        os);
  
    gIndenter--;

    os <<
      endl;
  }

  // print the identification if any
  if (fIdentification) {
    os <<
      fIdentification;
  }
  
  // print the page geometry if any
  if (fPageGeometry) {
    os <<
      fPageGeometry;
  }
  
  // print the credits if any
  if (fCreditsList.size ()) {
    list<S_msrCredit>::const_iterator
      iBegin = fCreditsList.begin (),
      iEnd   = fCreditsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os <<
      endl;
  }
  
  if (partGroupsListSize) {
    list<S_msrPartGroup>::const_iterator
      iBegin = fPartGroupsList.begin (),
      iEnd   = fPartGroupsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      (*i)->
        printSummary (os);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrScore& elt)
{
  elt->print (os);
  return os;
}


}
