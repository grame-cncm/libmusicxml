/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrMutualDependencies.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_msrRepeatCoda msrRepeatCoda::create (
  int                 inputLineNumber,
  S_msrSegment        repeatCodaSegment,
  S_msrRepeat         repeatUplink)
{
  msrRepeatCoda* o =
    new msrRepeatCoda (
      inputLineNumber,
      repeatCodaSegment,
      repeatUplink);
  assert(o!=0);
  return o;
}

msrRepeatCoda::msrRepeatCoda (
  int                 inputLineNumber,
  S_msrSegment        repeatCodaSegment,
  S_msrRepeat         repeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    repeatCodaSegment != nullptr,
    "repeatCodaSegment is null");
    
  fRepeatCodaSegment = repeatCodaSegment;
  
  // sanity check
  msrAssert (
    repeatUplink != nullptr,
    "repeatUplink is null");
    
  fRepeatCodaRepeatUplink = repeatUplink;
}

msrRepeatCoda::~msrRepeatCoda ()
{}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaNewbornClone (
  S_msrRepeat containingRepeat)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingRepeat != nullptr,
    "containingRepeat is null");
    
  S_msrRepeatCoda
    newbornClone = 0; // JMI
    /*
      msrRepeatCoda::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCoda ()->
            getRepeatCodaSegment (), // JMI
        containingRepeat);
      */
      
  // segment

  // uplinks

  return newbornClone;
}

S_msrRepeatCoda msrRepeatCoda::createRepeatCodaDeepCopy (
  S_msrRepeat containingRepeat)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingRepeat != nullptr,
    "containingRepeat is null");
    
  S_msrRepeatCoda
    repeatCodaDeepCopy = 0; // JMI
    /* JMI
      msrRepeatCoda::create (
        fInputLineNumber,
        containingRepeat->
          getRepeatCoda ()->
            getRepeatCodaSegment (), // JMI
        containingRepeat);
    */
    
  // segment
  repeatCodaDeepCopy->fRepeatCodaSegment =
    fRepeatCodaSegment->
      createSegmentDeepCopy (
        fRepeatCodaRepeatUplink->
          getRepeatVoiceUplink ());
    
  // uplinks
  repeatCodaDeepCopy->fRepeatCodaRepeatUplink =
    containingRepeat;

  return repeatCodaDeepCopy;
}

/* JMI
void msrRepeatCoda::appendElementToRepeatCoda (
  S_msrElement elem) // JMI ???
{
  fRepeatCodaSegment->
    appendOtherElementToSegment (elem);
}
*/

void msrRepeatCoda::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatCoda::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCoda::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCoda::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRepeatCoda::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCoda>*
    p =
      dynamic_cast<visitor<S_msrRepeatCoda>*> (v)) {
        S_msrRepeatCoda elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRepeatCoda::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatCoda::browseData (basevisitor* v)
{
  // browse the segment
  msrBrowser<msrSegment> browser (v);
  browser.browse (*fRepeatCodaSegment);
}

string msrRepeatCoda::asString () const
{
  stringstream s;

  s <<
    "RepeatCoda" <<
    ", line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatCoda::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl;
 
  gIndenter++;

  os <<
    fRepeatCodaSegment;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrRepeatCoda& elt)
{
  elt->print (os);
  return os;
}


}
