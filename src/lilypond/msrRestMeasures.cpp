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
S_msrRestMeasuresContents msrRestMeasuresContents::create (
  int               inputLineNumber,
  S_msrRestMeasures restMeasures)
{
  msrRestMeasuresContents* o =
    new msrRestMeasuresContents (
      inputLineNumber,
      restMeasures);
  assert(o!=0);
  return o;
}

msrRestMeasuresContents::msrRestMeasuresContents (
  int              inputLineNumber,
  S_msrRestMeasures restMeasures)
    : msrElement (inputLineNumber)
{
  fRestMeasuresContentsRestMeasuresUplink =
    restMeasures;
}

msrRestMeasuresContents::~msrRestMeasuresContents ()
{}

S_msrRestMeasuresContents msrRestMeasuresContents::createRestMeasuresContentsNewbornClone (
  S_msrRestMeasures restMeasures)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRestMeasures) {
    gLogIOstream <<
      "Creating a newborn clone of a rest measures contents" <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    restMeasures != nullptr,
    "restMeasures is null");
    
  S_msrRestMeasuresContents
    newbornClone =
      msrRestMeasuresContents::create (
        fInputLineNumber,
        restMeasures);

  return newbornClone;
}

void msrRestMeasuresContents::setRestMeasuresContentsSegment (
  S_msrSegment restMeasuresContentsSegment)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRestMeasures) {
    gLogIOstream <<
      "Setting rest measures contents segment containing " <<
      singularOrPlural (
        restMeasuresContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    restMeasuresContentsSegment != nullptr,
    "restMeasuresContentsSegment is null");

  fRestMeasuresContentsSegment =
    restMeasuresContentsSegment;
}

int msrRestMeasuresContents::restMeasuresContentsMeasuresNumber () const
{
  int result;

  if (fRestMeasuresContentsSegment) {
    result =
      fRestMeasuresContentsSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrRestMeasuresContents::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRestMeasuresContents::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrRestMeasuresContents>*
    p =
      dynamic_cast<visitor<S_msrRestMeasuresContents>*> (v)) {
        S_msrRestMeasuresContents elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRestMeasuresContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRestMeasuresContents::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRestMeasuresContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRestMeasuresContents>*
    p =
      dynamic_cast<visitor<S_msrRestMeasuresContents>*> (v)) {
        S_msrRestMeasuresContents elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRestMeasuresContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRestMeasuresContents::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRestMeasuresContents::browseData ()" <<
      endl;
  }

  if (fRestMeasuresContentsSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fRestMeasuresContentsSegment);
  }
}

string msrRestMeasuresContents::asString () const
{
  stringstream s;

  s <<

    "RestMeasuresContents" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      restMeasuresContentsMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str ();
}

void msrRestMeasuresContents::print (ostream& os)
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

  if (! fRestMeasuresContentsSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    gIndenter++;
    
    os <<
      fRestMeasuresContentsSegment;

    gIndenter--;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrRestMeasuresContents& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrRestMeasures msrRestMeasures::create (
  int        inputLineNumber,
  rational   restMeasuresMeasureSoundingNotes,
  int        restMeasuresNumber,
  S_msrVoice voiceUplink)
{
  msrRestMeasures* o =
    new msrRestMeasures (
      inputLineNumber,
      restMeasuresMeasureSoundingNotes,
      restMeasuresNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrRestMeasures::msrRestMeasures (
  int        inputLineNumber,
  rational   restMeasuresMeasureSoundingNotes,
  int        restMeasuresNumber,
  S_msrVoice voiceUplink)
    : msrVoiceElement (inputLineNumber)
{
  fRestMeasuresMeasureSoundingNotes = restMeasuresMeasureSoundingNotes;
  fRestMeasuresNumber       = restMeasuresNumber;

  fRestMeasuresVoiceUplink = voiceUplink;
}

msrRestMeasures::~msrRestMeasures ()
{}

S_msrRestMeasures msrRestMeasures::createRestMeasuresNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRestMeasures) {
    gLogIOstream <<
      "Creating a newborn clone of rest measures " <<
      asString () <<
      endl;
  }
#endif
  
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
    
  S_msrRestMeasures
    newbornClone =
      msrRestMeasures::create (
        fInputLineNumber,
        fRestMeasuresMeasureSoundingNotes,
        fRestMeasuresNumber,
        containingVoice);

  newbornClone->fRestMeasuresNextMeasureNumber =
    fRestMeasuresNextMeasureNumber;
    
  return newbornClone;
}

void msrRestMeasures::setRestMeasuresContents (
  S_msrRestMeasuresContents restMeasuresContents)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRestMeasures) {
    gLogIOstream <<
      "Setting rest measures contents containing " <<
      singularOrPlural (
        restMeasuresContents->
          restMeasuresContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    restMeasuresContents != nullptr,
    "restMeasuresContents is null");

  fRestMeasuresContents = restMeasuresContents;
}

void msrRestMeasures::setRestMeasuresNextMeasureNumber (
  string measureNumber)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRestMeasures) {
    gLogIOstream <<
      "Setting rest measures next measure number to'" <<
      "' " <<
      measureNumber <<
      endl;
  }
#endif

  fRestMeasuresNextMeasureNumber =
    measureNumber;
}

void msrRestMeasures::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRestMeasures::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrRestMeasures>*
    p =
      dynamic_cast<visitor<S_msrRestMeasures>*> (v)) {
        S_msrRestMeasures elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRestMeasures::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRestMeasures::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRestMeasures::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRestMeasures>*
    p =
      dynamic_cast<visitor<S_msrRestMeasures>*> (v)) {
        S_msrRestMeasures elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrRestMeasures::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRestMeasures::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrRestMeasures::browseData ()" <<
      endl;
  }

/* JMI
  // get the part uplink
  S_msrPart
    partUplink =
      fRestMeasuresVoiceUplink->
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

  bool inhibitRestMeasuresBrowsing =
    scoreUplink->
      getInhibitRestMeasuresBrowsing ();

  if (inhibitRestMeasuresBrowsing) {
    if (gMsrOptions->fTraceMsrVisitors || gGeneralOptions->fTraceRestMeasures) {
      gLogIOstream <<
        "% ==> visiting rest measures is inhibited" <<
        endl;
    }
  }
*/

  if (fRestMeasuresContents) {
 // JMI   if (! inhibitRestMeasuresBrowsing) {
      // browse the rest measures contents
      msrBrowser<msrRestMeasuresContents> browser (v);
      browser.browse (*fRestMeasuresContents);
//    }
  }
}

string msrRestMeasures::asString () const
{
  stringstream s;

  s <<
    "RestMeasures" <<
    ", line " << fInputLineNumber <<
    ", restMeasuresMeasureSoundingNotes: " <<
    fRestMeasuresMeasureSoundingNotes <<
    ", " <<
    singularOrPlural (
      fRestMeasuresNumber,
        "rest measure",
        "rest measures") <<
    ", next measure number: '" <<
    fRestMeasuresNextMeasureNumber <<
    "'";
    
  return s.str ();
}

void msrRestMeasures::displayRestMeasures (
  int    inputLineNumber,
  string context)
{
  gLogIOstream <<
    endl <<
    "*********>> Rest measures " << context << " \"" <<
 // JMI   getVoiceName () <<
    "\"" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  gIndenter++;
  print (gLogIOstream);
  gIndenter--;

  gLogIOstream <<
    "<<*********" <<
    endl <<
    endl;
}

void msrRestMeasures::print (ostream& os)
{
  os <<
    "RestMeasures" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os << left <<
    setw (fieldWidth) <<
    "restMeasuresMeasureSoundingNotes" << " : " <<
    fRestMeasuresMeasureSoundingNotes <<
    endl <<
    setw (fieldWidth) <<
    "restMeasuresNumber" << " : " <<
    fRestMeasuresNumber <<
    endl <<
    setw (fieldWidth) <<
    "restMeasuresNextMeasureNumber" << " : '" <<
    fRestMeasuresNextMeasureNumber <<
    "'" <<
    endl;
  
  // print the voice uplink
  os << left <<
    setw (fieldWidth) <<
    "restMeasuresVoiceUplink" << " : " <<
    "\"" <<
    fRestMeasuresVoiceUplink->getVoiceName () <<
    "\"" <<
    endl;
    
  // print the rests contents
  if (! fRestMeasuresContents) {
    os << left <<
      setw (fieldWidth) <<
      "restMeasuresContents" << " : " << "none" <<
      endl;
  }

  else {
    os <<
      fRestMeasuresContents;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrRestMeasures& elt)
{
  elt->print (os);
  return os;
}


}
