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
S_msrMultipleRestMeasuresContents msrMultipleRestMeasuresContents::create (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRestMeasuresContents* o =
    new msrMultipleRestMeasuresContents (
      inputLineNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRestMeasuresContents::msrMultipleRestMeasuresContents (
  int        inputLineNumber,
  S_msrVoice voiceUplink)
    : msrElement (inputLineNumber)
{
  fMultipleRestMeasuresContentsVoiceUplink = voiceUplink;
}

msrMultipleRestMeasuresContents::~msrMultipleRestMeasuresContents ()
{}

S_msrMultipleRestMeasuresContents msrMultipleRestMeasuresContents::createMultipleRestMeasuresContentsNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMultipleRestMeasures) {
    gLogIOstream <<
      "Creating a newborn clone of a multiple rest contents" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrMultipleRestMeasuresContents
    newbornClone =
      msrMultipleRestMeasuresContents::create (
        fInputLineNumber,
        containingVoice);

  return newbornClone;
}

void msrMultipleRestMeasuresContents::setMultipleRestMeasuresContentsSegment (
    S_msrSegment multipleRestMeasuresContentsSegment)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMultipleRestMeasures) {
    gLogIOstream <<
      "Setting multiple rest contents segment containing " <<
      singularOrPlural (
          multipleRestMeasuresContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    multipleRestMeasuresContentsSegment != nullptr,
    "multipleRestMeasuresContentsSegment is null");

  fMultipleRestMeasuresContentsSegment =
    multipleRestMeasuresContentsSegment;
}

int msrMultipleRestMeasuresContents::multipleRestMeasuresContentsMeasuresNumber () const
{
  int result;

  if (fMultipleRestMeasuresContentsSegment) {
    result =
      fMultipleRestMeasuresContentsSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrMultipleRestMeasuresContents::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestMeasuresContents::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMultipleRestMeasuresContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestMeasuresContents>*> (v)) {
        S_msrMultipleRestMeasuresContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestMeasuresContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleRestMeasuresContents::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestMeasuresContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMultipleRestMeasuresContents>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestMeasuresContents>*> (v)) {
        S_msrMultipleRestMeasuresContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestMeasuresContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleRestMeasuresContents::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestMeasuresContents::browseData ()" <<
      endl;
  }

  if (fMultipleRestMeasuresContentsSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMultipleRestMeasuresContentsSegment);
  }
}

string msrMultipleRestMeasuresContents::asString () const
{
  stringstream s;

  s <<

    "MultipleRestMeasuresContents" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      multipleRestMeasuresContentsMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str ();
}

void msrMultipleRestMeasuresContents::print (ostream& os)
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

  if (! fMultipleRestMeasuresContentsSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    gIndenter++;
    
    os <<
      fMultipleRestMeasuresContentsSegment;

    gIndenter--;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMultipleRestMeasuresContents& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrMultipleRestMeasures msrMultipleRestMeasures::create (
  int        inputLineNumber,
  rational   multipleRestMeasuresMeasureSoundingNotes,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
{
  msrMultipleRestMeasures* o =
    new msrMultipleRestMeasures (
      inputLineNumber,
      multipleRestMeasuresMeasureSoundingNotes,
      multipleRestMeasuresNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMultipleRestMeasures::msrMultipleRestMeasures (
  int        inputLineNumber,
  rational   multipleRestMeasuresMeasureSoundingNotes,
  int        multipleRestMeasuresNumber,
  S_msrVoice voiceUplink)
    : msrMeasureElement (inputLineNumber)
{
  fMultipleRestMeasuresMeasureSoundingNotes = multipleRestMeasuresMeasureSoundingNotes;
  fMultipleRestMeasuresNumber       = multipleRestMeasuresNumber;

  fMultipleRestMeasuresVoiceUplink = voiceUplink;
}

msrMultipleRestMeasures::~msrMultipleRestMeasures ()
{}

S_msrMultipleRestMeasures msrMultipleRestMeasures::createMultipleRestMeasuresNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMultipleRestMeasures) {
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
    
  S_msrMultipleRestMeasures
    newbornClone =
      msrMultipleRestMeasures::create (
        fInputLineNumber,
        fMultipleRestMeasuresMeasureSoundingNotes,
        fMultipleRestMeasuresNumber,
        containingVoice);

  newbornClone->fMultipleRestMeasuresNextMeasureNumber =
    fMultipleRestMeasuresNextMeasureNumber;
    
  return newbornClone;
}

void msrMultipleRestMeasures::setMultipleRestMeasuresContents (
  S_msrMultipleRestMeasuresContents multipleRestMeasuresContents)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMultipleRestMeasures) {
    gLogIOstream <<
      "Setting multiple rest contents containing " <<
      singularOrPlural (
        multipleRestMeasuresContents->
          multipleRestMeasuresContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    multipleRestMeasuresContents != nullptr,
    "multipleRestMeasuresContents is null");

  fMultipleRestMeasuresContents = multipleRestMeasuresContents;
}

void msrMultipleRestMeasures::setMultipleRestMeasuresNextMeasureNumber (
  string measureNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceMultipleRestMeasures) {
    gLogIOstream <<
      "Setting multiple rest next measure number to'" <<
      "' " <<
      measureNumber <<
      endl;
  }
#endif

  fMultipleRestMeasuresNextMeasureNumber =
    measureNumber;
}

void msrMultipleRestMeasures::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestMeasures::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMultipleRestMeasures>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestMeasures>*> (v)) {
        S_msrMultipleRestMeasures elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestMeasures::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMultipleRestMeasures::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestMeasures::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMultipleRestMeasures>*
    p =
      dynamic_cast<visitor<S_msrMultipleRestMeasures>*> (v)) {
        S_msrMultipleRestMeasures elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMultipleRestMeasures::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMultipleRestMeasures::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMultipleRestMeasures::browseData ()" <<
      endl;
  }

/* JMI
  // get the part uplink
  S_msrPart
    partUplink =
      fMultipleRestMeasuresVoiceUplink->
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
    if (gMsrOptions->fTraceMsrVisitors || gGeneralOptions->fTraceMultipleRestMeasures) {
      gLogIOstream <<
        "% ==> visiting multiple rest measures is inhibited" <<
        endl;
    }
  }
*/

  if (fMultipleRestMeasuresContents) {
 // JMI   if (! inhibitMultipleRestMeasuresBrowsing) {
      // browse the multiple rest contents
      msrBrowser<msrMultipleRestMeasuresContents> browser (v);
      browser.browse (*fMultipleRestMeasuresContents);
//    }
  }
}

string msrMultipleRestMeasures::asString () const
{
  stringstream s;

  s <<
    "MultipleRestMeasures" <<
    ", line " << fInputLineNumber <<
    ", multipleRestMeasuresMeasureSoundingNotes: " <<
    fMultipleRestMeasuresMeasureSoundingNotes <<
    ", " <<
    singularOrPlural (
      fMultipleRestMeasuresNumber,
        "rest measure",
        "rest measures") <<
    ", next measure number: '" <<
    fMultipleRestMeasuresNextMeasureNumber <<
    "'";
    
  return s.str ();
}

void msrMultipleRestMeasures::print (ostream& os)
{
  os <<
    "MultipleRestMeasures" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os << left <<
    setw (fieldWidth) <<
    "multipleRestMeasuresMeasureSoundingNotes" << " : " <<
    fMultipleRestMeasuresMeasureSoundingNotes <<
    endl <<
    setw (fieldWidth) <<
    "multipleRestMeasuresNumber" << " : " <<
    fMultipleRestMeasuresNumber <<
    endl <<
    setw (fieldWidth) <<
    "multipleRestMeasuresNextMeasureNumber" << " : '" <<
    fMultipleRestMeasuresNextMeasureNumber <<
    "'" <<
    endl;
  
  // print the voice uplink
  os << left <<
    setw (fieldWidth) <<
    "multipleRestMeasuresVoiceUplink" << " : " <<
    "\"" <<
    fMultipleRestMeasuresVoiceUplink->getVoiceName () <<
    "\"" <<
    endl;
    
  // print the rests contents
  if (! fMultipleRestMeasuresContents) {
    os << left <<
      setw (fieldWidth) <<
      "multipleRestMeasuresContents" << " : " << "none" <<
      endl;
  }

  else {
    os <<
      fMultipleRestMeasuresContents;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMultipleRestMeasures& elt)
{
  elt->print (os);
  return os;
}


}
