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
S_msrMultipleRestContents msrMultipleRestContents::create (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRestContents* o =
    new msrMultipleRestContents (
      inputLineNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRestContents::msrMultipleRestContents (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMultipleRestContentsVoiceUplink = voiceUplink;
}

msrMultipleRestContents::~msrMultipleRestContents ()
{}

S_msrMultipleRestContents msrMultipleRestContents::createMultipleRestContentsNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of a multiple rest contents" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMultipleRestContents
    newbornClone =
      msrMultipleRestContents::create (
        fInputLineNumber,
        containingVoice);

  return newbornClone;
}

void msrMultipleRestContents::setMultipleRestContentsSegment (
    S_msrSegment multipleRestContentsSegment)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting multiple rest contents segment containing " <<
      singularOrPlural (
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    multipleRestContentsSegment != nullptr,
    "multipleRestContentsSegment is null");

  fMultipleRestContentsSegment =
    multipleRestContentsSegment;
}

int msrMultipleRestContents::multipleRestContentsMeasuresNumber () const
{
  int result;

  if (fMultipleRestContentsSegment) {
    result =
      fMultipleRestContentsSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrMultipleRestContents::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestContents::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleRestContents::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMultipleRestContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestContents>*> (v)) {
        S_msrMultipleRestContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleRestContents::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestContents::browseData ()" <<
      endl;
  }

  if (fMultipleRestContentsSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMultipleRestContentsSegment);
  }
}

string msrMultipleRestContents::asString () const
{
  stringstream s;

  s <<

    "MultipleRestContents" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      multipleRestContentsMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str ();
}

void msrMultipleRestContents::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl <<
    endl;
  
  gIndenter++;
  
  // print the pattern segment
  os <<
    "Contents segment:";

  if (! fMultipleRestContentsSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    gIndenter++;
    
    os <<
      fMultipleRestContentsSegment;

    gIndenter--;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMultipleRestContents& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrMultipleRest msrMultipleRest::create (
  int        inputLineNumber,
  rational   multipleRestMeasureSoundingNotes,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRest* o =
    new msrMultipleRest (
      inputLineNumber,
      multipleRestMeasureSoundingNotes,
      multipleRestMeasuresNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRest::msrMultipleRest (
  int        inputLineNumber,
  rational   multipleRestMeasureSoundingNotes,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
    : msrMeasureElement (inputLineNumber)
{
  fMultipleRestMeasureSoundingNotes = multipleRestMeasureSoundingNotes;
  fMultipleRestMeasuresNumber       = multipleRestMeasuresNumber;

  fMultipleRestVoiceUplink = voiceUplink;
}

msrMultipleRest::~msrMultipleRest ()
{}

S_msrMultipleRest msrMultipleRest::createMultipleRestNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of multiple rest " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMultipleRest
    newbornClone =
      msrMultipleRest::create (
        fInputLineNumber,
        fMultipleRestMeasureSoundingNotes,
        fMultipleRestMeasuresNumber,
        containingVoice);

  newbornClone->fMultipleRestNextMeasureNumber =
    fMultipleRestNextMeasureNumber;
    
  return newbornClone;
}

void msrMultipleRest::setMultipleRestContents (
  S_msrMultipleRestContents multipleRestContents)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting multiple rest contents containing " <<
      singularOrPlural (
        multipleRestContents->
          multipleRestContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    multipleRestContents != nullptr,
    "multipleRestContents is null");

  fMultipleRestContents = multipleRestContents;
}

void msrMultipleRest::setMultipleRestNextMeasureNumber (
  string measureNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting multiple rest next measure number to'" <<
      "' " <<
      measureNumber <<
      endl;
  }
#endif

  fMultipleRestNextMeasureNumber =
    measureNumber;
}

void msrMultipleRest::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRest::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleRest::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMultipleRest>*
    p =
      dynamic_cast<visitor<S_msrMultipleRest>*> (v)) {
        S_msrMultipleRest elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRest::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleRest::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRest::browseData ()" <<
      endl;
  }

/* JMI
  // get the part uplink
  S_msrPart
    partUplink =
      fMultipleRestVoiceUplink->
        fetchVoicePartUplink ();

  // get the part group uplink
  S_msrPartGroup
    partGroupUplink =
      partUplink->
        getPartPartGroupUplink ();

  // get the score uplink
  S_msrScore
    scoreUplink =
      partGroupUplink->
        getPartGroupScoreUplink ();

  bool inhibitMultipleRestMeasuresBrowsing =
    scoreUplink->
      getInhibitMultipleRestMeasuresBrowsing ();

  if (inhibitMultipleRestMeasuresBrowsing) {
    if (gMsrOptions->fTraceMsrVisitors || gGeneralOptions->fTraceRepeats) {
      gLogIOstream <<
        "% ==> visiting multiple rest measures is inhibited" <<
        endl;
    }
  }
*/

  if (fMultipleRestContents) {
 // JMI   if (! inhibitMultipleRestMeasuresBrowsing) {
      // browse the multiple rest contents
      msrBrowser<msrMultipleRestContents> browser (v);
      browser.browse (*fMultipleRestContents);
//    }
  }
}

string msrMultipleRest::asString () const
{
  stringstream s;

  s <<
    "MultipleRest" <<
    ", line " << fInputLineNumber <<
    ", multipleRestMeasureSoundingNotes: " <<
    fMultipleRestMeasureSoundingNotes <<
    ", " <<
    singularOrPlural (
      fMultipleRestMeasuresNumber,
        "rest measure",
        "rest measures") <<
    ", next measure number: '" <<
    fMultipleRestNextMeasureNumber <<
    "'";
    
  return s.str ();
}

void msrMultipleRest::print (ostream& os)
{
  os <<
    "MultipleRest" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os << left <<
    setw (fieldWidth) <<
    "multipleRestMeasureSoundingNotes" << " : " <<
    fMultipleRestMeasureSoundingNotes <<
    endl <<
    setw (fieldWidth) <<
    "multipleRestMeasuresNumber" << " : " <<
    fMultipleRestMeasuresNumber <<
    endl <<
    setw (fieldWidth) <<
    "multipleRestNextMeasureNumber" << " : '" <<
    fMultipleRestNextMeasureNumber <<
    "'" <<
    endl;
  
  // print the voice uplink
  os << left <<
    setw (fieldWidth) <<
    "multipleRestVoiceUplink" << " : " <<
    "\"" <<
    fMultipleRestVoiceUplink->getVoiceName () <<
    "\"" <<
    endl;
    
  // print the rests contents
  if (! fMultipleRestContents) {
    os << left <<
      setw (fieldWidth) <<
      "multipleRestContents" << " : " << "none" <<
      endl;
  }

  else {
    os <<
      fMultipleRestContents;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMultipleRest& elt)
{
  elt->print (os);
  return os;
}


}
