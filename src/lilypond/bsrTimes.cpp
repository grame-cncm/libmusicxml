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
  int         inputLineNumber,
  bsrTimeKind timeKind)
{
  bsrTime* o =
    new bsrTime (
      inputLineNumber,
      timeKind);
  assert (o!=0);
  return o;
}

bsrTime::bsrTime (
  int         inputLineNumber,
  bsrTimeKind timeKind)
    : bsrElement (inputLineNumber)
{
  fTimeKind = timeKind;
  
  fTimeCellsList =
    asCellsList ();

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes) {
    gLogIOstream <<
      "Creating bsrTimes '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrTime::~bsrTime ()
{}

S_bsrCellsList bsrTime::asCellsList () const
{
  S_bsrCellsList
    result; // =
   //   bsrCellsList::create (fInputLineNumber);

  switch (fTimeKind) {
    case bsrTime::kTimeNone:
      break;

    case bsrTime::kTimeCommon:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots46, kDots14);
      break;
      
    case bsrTime::kTimeCut:
      result =
        bsrCellsList::create (
          fInputLineNumber,
          kDots456, kDots14);
      break;
      
    case bsrTime::kTimeNumerical:
      break;
  /* 
    case bsrTime::kTimeSymbolDottedNote:
      break;
    case bsrTime::kTimeSymbolSingleNumber:
      break;
    case bsrTime::kTimeSymbolSenzaMisura:
      break;
      */
  } // switch


/*
  // append note octave if needed
  switch (fNoteOctaveIsNeeded) {
    case bsrNote::kNoteOctaveIsNeededYes:
      result->appendCellsListToCellsList (
        noteOctaveKindAsCellsList ());
      break;
    case bsrNote::kNoteOctaveIsNeededNo:
      break;
  } // switch

  // append note value
  result->appendCellsListToCellsList (
    noteValueKindAsCellsList ());

  // append dots if any
  for (int i = 0; i < fNoteDotsNumber; i++) {
    result->appendCellKindToCellsList (
      kCellAugmentationDot);
  } // for
*/

  return result;
}

int bsrTime::fetchCellsNumber() const
{
//  return asCellsList ().fetchCellsNumber();
  return fTimeCellsList->fetchCellsNumber();
}

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
    ", timeCellsList: " << fTimeCellsList->asString () <<
    ", line " << fInputLineNumber;
 
  return s.str ();
}

string bsrTime::timeKindAsString (
  bsrTimeKind timeKind)
{
  string result;
 
  switch (timeKind) {
    case bsrTime::kTimeNone:
      result = "timeNone";
      break;

    case bsrTime::kTimeCommon:
      result = "timeCommon";
      break;
    case bsrTime::kTimeCut:
      result = "timeCut";
      break;
    case bsrTime::kTimeNumerical:
      result = "timeNumerical";
      break;
  /* 
    case bsrTime::kTimeSymbolDottedNote:
      break;
    case bsrTime::kTimeSymbolSingleNumber:
      break;
    case bsrTime::kTimeSymbolSenzaMisura:
      break;
      */
  } // switch

  return result;
}

void bsrTime::print (ostream& os)
{
  os <<
    "Time" <<
    ", timeKind " << " : " <<
    timeKindAsString (fTimeKind) <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 14;
    
  os <<
    setw (fieldWidth) <<
    "timeCellsList" << " : " << fTimeCellsList->asString () <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrTime& elt)
{
  elt->print (os);
  return os;
}


}
