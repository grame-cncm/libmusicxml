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
S_msrMeasuresRepeatPattern msrMeasuresRepeatPattern::create (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
{
  msrMeasuresRepeatPattern* o =
    new msrMeasuresRepeatPattern (
      inputLineNumber,
      measuresRepeatUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeatPattern::msrMeasuresRepeatPattern (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatUplink != nullptr,
    "measuresRepeatUplink is null");
    
  fMeasuresRepeatUplink = measuresRepeatUplink;
}

msrMeasuresRepeatPattern::~msrMeasuresRepeatPattern ()
{}

void msrMeasuresRepeatPattern::setMeasuresRepeatPatternSegment (
    S_msrSegment measuresRepeatPatternSegment)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat pattern segment containing " <<
      singularOrPlural (
          measuresRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    measuresRepeatPatternSegment != nullptr,
    "measuresRepeatPatternSegment is null");

  fMeasuresRepeatPatternSegment =
    measuresRepeatPatternSegment;
}

int msrMeasuresRepeatPattern::measuresRepeatPatternMeasuresNumber () const
{
  int result;

  if (fMeasuresRepeatPatternSegment) {
    result =
      fMeasuresRepeatPatternSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrMeasuresRepeatPattern::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatPattern::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatPattern::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasuresRepeatPatternSegment);
  }
}

string msrMeasuresRepeatPattern::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")"; 

  return s.str ();
}

void msrMeasuresRepeatPattern::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl <<
    endl;
  
  gIndenter++;
  
  // print the pattern segment
  os <<
    "Pattern segment:";

  if (! fMeasuresRepeatPatternSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    gIndenter++;
    
    os <<
      fMeasuresRepeatPatternSegment;

    gIndenter--;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeatPattern& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrMeasuresRepeatReplicas msrMeasuresRepeatReplicas::create (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
{
  msrMeasuresRepeatReplicas* o =
    new msrMeasuresRepeatReplicas (
      inputLineNumber,
      measuresRepeatUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeatReplicas::msrMeasuresRepeatReplicas (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatUplink != nullptr,
    "measuresRepeatUplink is null");
    
  fMeasuresRepeatUplink = measuresRepeatUplink;
}

msrMeasuresRepeatReplicas::~msrMeasuresRepeatReplicas ()
{}

void msrMeasuresRepeatReplicas::setMeasuresRepeatReplicasSegment (
    S_msrSegment measuresRepeatReplicasSegment)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat replicas segment containing " <<
      singularOrPlural (
          measuresRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    measuresRepeatReplicasSegment != nullptr,
    "measuresRepeatReplicasSegment is null");

  fMeasuresRepeatReplicasSegment =
    measuresRepeatReplicasSegment;
}

int msrMeasuresRepeatReplicas::measuresRepeatReplicasMeasuresNumber () const
{
  int result;

  if (fMeasuresRepeatReplicasSegment) {
    result =
      fMeasuresRepeatReplicasSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrMeasuresRepeatReplicas::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatReplicas::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasuresRepeatReplicasSegment);
  }
}

string msrMeasuresRepeatReplicas::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")"; 

  return s.str ();
}

void msrMeasuresRepeatReplicas::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl <<
    endl;
  
  gIndenter++;
  
  // print the replicas segment
  os <<
    "Replicas segment:";

  if (! fMeasuresRepeatReplicasSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;
      
    gIndenter++;
    
    os <<
      fMeasuresRepeatReplicasSegment;

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeatReplicas& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrMeasuresRepeat msrMeasuresRepeat::create (
  int        inputLineNumber,
  int        measuresRepeatMeasuresNumber,
  int        measuresRepeatSlashesNumber,
  S_msrVoice voiceUplink)
{
  msrMeasuresRepeat* o =
    new msrMeasuresRepeat (
      inputLineNumber,
      measuresRepeatMeasuresNumber,
      measuresRepeatSlashesNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeat::msrMeasuresRepeat (
  int        inputLineNumber,
  int        measuresRepeatMeasuresNumber,
  int        measuresRepeatSlashesNumber,
  S_msrVoice voiceUplink)
    : msrVoiceElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatMeasuresNumber > 0,
    "measuresRepeatMeasuresNumber is not positive");

  fMeasuresRepeatMeasuresNumber = measuresRepeatMeasuresNumber;

  // sanity check
  msrAssert (
    measuresRepeatSlashesNumber > 0,
    "measuresRepeatSlashesNumber is not positive");

  fMeasuresRepeatSlashesNumber  = measuresRepeatSlashesNumber;

  fMeasuresRepeatVoiceUplink = voiceUplink;
}

msrMeasuresRepeat::~msrMeasuresRepeat ()
{}

void msrMeasuresRepeat::setMeasuresRepeatPattern (
  S_msrMeasuresRepeatPattern measuresRepeatPattern)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat pattern containing " <<
      singularOrPlural (
        measuresRepeatPattern->
          measuresRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    measuresRepeatPattern != nullptr,
    "measuresRepeatPattern is null");

  fMeasuresRepeatPattern = measuresRepeatPattern;
}

void msrMeasuresRepeat::setMeasuresRepeatReplicas (
  S_msrMeasuresRepeatReplicas measuresRepeatReplicas)
{
#ifdef TRACE_OPTIONS
  if (gGeneralOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measure repeat replicas containing " <<
      singularOrPlural (
        measuresRepeatReplicas->
          measuresRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif
      
  // sanity check
  msrAssert (
    measuresRepeatReplicas != nullptr,
    "measuresRepeatReplicas is null");

  fMeasuresRepeatReplicas = measuresRepeatReplicas;
}

int msrMeasuresRepeat::measuresRepeatReplicasNumber () const
{  
  int patternMeasuresNumber =
    measuresRepeatPatternMeasuresNumber ();
    
  // sanity check
  msrAssert (
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");

  return
    measuresRepeatReplicasMeasuresNumber ()
      /
    patternMeasuresNumber;    
}

void msrMeasuresRepeat::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeat::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeat::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrMeasuresRepeatPattern> browser (v);
    browser.browse (*fMeasuresRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fMeasuresRepeatVoiceUplink->
        fetchVoicePartUplink ()->
          getPartPartGroupUplink ()->
            getPartGroupScoreUplink ();
              
  bool inhibitMeasuresRepeatReplicasBrowsing =
    score->getInhibitMeasuresRepeatReplicasBrowsing ();

  if (inhibitMeasuresRepeatReplicasBrowsing) {
    if (
      gMsrOptions->fTraceMsrVisitors
#ifdef TRACE_OPTIONS
        ||
      gGeneralOptions->fTraceRepeats
#endif
    ) {
      gLogIOstream <<
        "% ==> visiting measure repeat replicas is inhibited" <<
        endl;
    }
  }

  if (fMeasuresRepeatReplicas) {
    if (! inhibitMeasuresRepeatReplicasBrowsing) {
      // browse the measure repeat replicas
      msrBrowser<msrMeasuresRepeatReplicas> browser (v);
      browser.browse (*fMeasuresRepeatReplicas);
    }
  }
}

string msrMeasuresRepeat::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      measuresRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    measuresRepeatReplicasNumber () << " replicas" <<
    ")"; 

  return s.str ();
}

void msrMeasuresRepeat::print (ostream& os)
{
  os <<
    endl <<
    "MeasuresRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatPatternMeasuresNumber (),
      "pattern measure",
      "pattern measures") <<
    ", " <<
    /* JMI ???
    singularOrPlural (
      measuresRepeatReplicasMeasuresNumber (),
      "replica measure",
      "replicas measures") <<
      */
    ")" <<
    endl;
  
  gIndenter++;
  
  // print the measure repeat pattern
  if (! fMeasuresRepeatPattern) {
    os <<
      "Measure repeat pattern: none" <<
      endl;
  }

  else {
    os <<
      fMeasuresRepeatPattern <<
      endl;
  }
  
  // print the measure repeat replicas
  if (! fMeasuresRepeatReplicas) {
    os <<
      "Measure repeat replicas: none" <<
      endl;
  }

  else {
    os <<
      fMeasuresRepeatReplicas;
  }
      
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeat& elt)
{
  elt->print (os);
  return os;
}


}
