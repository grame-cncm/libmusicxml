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

namespace MusicXML2 {

//______________________________________________________________________________
S_bsrTimeItem bsrTimeItem::create (
  int inputLineNumber)
{
  bsrTimeItem* o =
    new bsrTimeItem (
      inputLineNumber);
  assert (o!=0);

  return o;
}

bsrTimeItem::bsrTimeItem (
  int inputLineNumber)
    : bsrElement (inputLineNumber)
{
  fTimeBeatValue = -1;
  
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes) {
    gLogIOstream <<
      "Creating time item" <<
      ", line = " << inputLineNumber <<
      endl;
  }
#endif
}

bsrTimeItem::~bsrTimeItem ()
{}

bool bsrTimeItem::isEqualTo (S_bsrTimeItem otherTimeItem) const
{  
  if (! otherTimeItem) {
    return false;
  }
    
  if (
    ! (
        fTimeBeatValue == otherTimeItem->fTimeBeatValue
            &&
        fTimeBeatsNumbersVector.size ()
          ==
        otherTimeItem->fTimeBeatsNumbersVector.size ()
      )
    ) {
    return false;
  }
    
  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); i++) {
    if (
      ! (
        fTimeBeatsNumbersVector [i]
          ==
        otherTimeItem->fTimeBeatsNumbersVector [i]
        )
      ) {
      return false;
    }
  } // for
 
  return true;
}

void bsrTimeItem::appendBeatsNumber (int beatsNumber)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes) {
    gLogIOstream <<
      "Append beat number '" <<
      beatsNumber <<
      "' to time '" << // JMI
      "'" <<
      endl;
    }
#endif

  fTimeBeatsNumbersVector.insert (
    fTimeBeatsNumbersVector.end (),
    beatsNumber);
}

int bsrTimeItem::getTimeBeatsNumber () const
{
  int result = 0;
  
  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); i++) {
    result +=
      fTimeBeatsNumbersVector [i];
    } // for

  return result;
}

void bsrTimeItem::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTimeItem::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrTimeItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeItem>*> (v)) {
        S_bsrTimeItem elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTimeItem::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrTimeItem::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTimeItem::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrTimeItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeItem>*> (v)) {
        S_bsrTimeItem elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTimeItem::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrTimeItem::browseData (basevisitor* v)
{}

string bsrTimeItem::asString () const
{
  stringstream s;

  s <<
    "TimeItem ";

  int vectorSize =
    fTimeBeatsNumbersVector.size ();

  switch (vectorSize) {
    case 0:
      bsrInternalError (
        gXml2brlOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "time item beats numbers vector is empty");
      break;
      
    case 1:
      s <<
        fTimeBeatsNumbersVector [0] << "/" << fTimeBeatValue;
      break;
      
    default:
      s <<
        "beats numbers: ";

      for (int i = 0; i < vectorSize; i++) {
        s <<
          fTimeBeatsNumbersVector [i];
  
        if (i != vectorSize - 1) {
          s <<
            " ";
        }
      } // for

      s <<
        ", beat value: " << fTimeBeatValue;
  } // switch
  
  s <<
    ", line " << fInputLineNumber;
     
  return s.str ();
}

void bsrTimeItem::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrTimeItem& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrTime bsrTime::create (
  int               inputLineNumber,
  bsrTimeSymbolKind timeSymbolKind)
{
  bsrTime* o =
    new bsrTime (
      inputLineNumber, timeSymbolKind);
  assert (o!=0);
  return o;
}

bsrTime::bsrTime (
  int               inputLineNumber,
  bsrTimeSymbolKind timeSymbolKind)
    : bsrElement (inputLineNumber)
{
  fTimeSymbolKind = timeSymbolKind;
    
  fTimeIsCompound = false;
}

bool bsrTime::isEqualTo (S_bsrTime otherTime) const
{
  if (! otherTime) {
    return false;
  }
    
  if (
    ! (
        fTimeSymbolKind == otherTime->fTimeSymbolKind
          &&
        fTimeIsCompound == otherTime->fTimeIsCompound
          &&
        fTimeItemsVector.size () == otherTime->fTimeItemsVector.size ()
      )
    ) {
    return false;
  }
    
  for (unsigned int i = 0; i < fTimeItemsVector.size (); i++) {
    if (
      ! (
        fTimeItemsVector [i]->isEqualTo (
          otherTime->fTimeItemsVector [i])
        )
      ) {
      return false;
    }
  } // for
 
  return true;
}

S_bsrTime bsrTime::createFourQuartersTime (
  int inputLineNumber)
{
  // create the time
  S_bsrTime
    time =
      bsrTime::create (
        inputLineNumber,
        bsrTime::kTimeSymbolNone);

  // create a four quarters time item
  S_bsrTimeItem
    timeItem =
      bsrTimeItem::create (
        inputLineNumber);

  timeItem->
    appendBeatsNumber (4);
  timeItem->
    setTimeBeatValue (4);
        
  // append the time item to the time
  time->
    appendTimeItem (timeItem);

  // return the time
  return time;
}

bsrTime::~bsrTime ()
{}

void bsrTime::appendTimeItem (
  S_bsrTimeItem timeItem)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTimes) {
    gLogIOstream <<
      "Append item '" <<
      timeItem->asString () <<
      "' to time '" <<
      "JMI" <<
      "'" <<
      endl;
    }
#endif

  if (! fTimeItemsVector.size ()) {
    // this is the first item inserted
    if (
      timeItem->
        getTimeBeatsNumbersVector (). size ()
        >
      1) {
      // this time is compound
      fTimeIsCompound = true;
    }
  }
  
  else {
    // there are several time items, this time is compound
    fTimeIsCompound = true;
  }

  // append the time item to the vector
  fTimeItemsVector.insert (
    fTimeItemsVector.end (), timeItem);
}

rational bsrTime::wholeNotesPerMeasure () const
{
  rational result (0, 1); // addition neutral element

  int vectorSize = fTimeItemsVector.size ();
  
  if (vectorSize) {
    /* JMI
    // start with first item
    result =
      rational (
        fTimeItemsVector [0]->getTimeBeatsNumber (),
        fTimeItemsVector [0]->getTimeBeatValue ());
*/

/* JMI
    gLogIOstream <<
      endl <<
      endl <<
      "result1 = " <<
      result.getNumerator () <<
      "/" <<
      result.getDenominator () <<
      endl <<
      endl;
*/

    // iterate over the others
    for (int i = 0; i < vectorSize; i++) {
      result +=
        rational (
          fTimeItemsVector [i]->getTimeBeatsNumber (),
          fTimeItemsVector [i]->getTimeBeatValue ());

/* JMI
      gLogIOstream <<
        endl <<
        endl <<
        "result2 = " <<
        result.getNumerator () <<
        "/" <<
        result.getDenominator () <<
        endl <<
        endl;
        */
        
    } // for
  }

  else {
    bsrInternalError (
      gXml2brlOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      "time items vector is empty");
  }

  // rationalize result
  result.rationalise ();

  // return it
  return result;
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

string bsrTime::timeSymbolKindAsString (
  bsrTimeSymbolKind timeSymbolKind)
{
  string result;
 
  switch (timeSymbolKind) {
    case bsrTime::kTimeSymbolCommon:
      result = "timeSymbolCommon";
      break;
    case bsrTime::kTimeSymbolCut:
      result = "timeSymbolCut";
      break;
    case bsrTime::kTimeSymbolNote:
      result = "timeSymbolNote";
      break;
    case bsrTime::kTimeSymbolDottedNote:
      result = "timeSymbolDottedNote";
      break;
    case bsrTime::kTimeSymbolSingleNumber:
      result = "timeSymbolSingleNumber";
      break;
    case bsrTime::kTimeSymbolSenzaMisura:
      result = "timeSymbolSenzaMisura";
      break;
    case bsrTime::kTimeSymbolNone:
      result = "timeSymbolNone";
      break;
  } // switch

  return result;
}

string bsrTime::timeSeparatorKindAsString (
  bsrTimeSeparatorKind timeSeparatorKind)
{
  string result;
 
  switch (timeSeparatorKind) {
    case bsrTime::kTimeSeparatorNone:
      result = "timeSeparatorNone";
      break;
    case bsrTime::kTimeSeparatorHorizontal:
      result = "timeSeparatorHorizontal";
      break;
    case bsrTime::kTimeSeparatorDiagonal:
      result = "timeSeparatorDiagonal";
      break;
    case bsrTime::kTimeSeparatorVertical:
      result = "timeSeparatorVertical";
      break;
    case bsrTime::kTimeSeparatorAdjacent:
      result = "timeSeparatorAdjacent";
      break;
  } // switch

  return result;
}

string bsrTime::timeRelationKindAsString (
  bsrTimeRelationKind timeRelationKind)
{
  string result;
 
  switch (timeRelationKind) {
    case bsrTime::kTimeRelationNone:
      result = "timeRelationNone";
      break;
    case bsrTime::kTimeRelationParentheses:
      result = "timeRelationParentheses";
      break;
    case bsrTime::kTimeRelationBracket:
      result = "timeRelationBracket";
      break;
    case bsrTime::kTimeRelationEquals:
      result = "timeRelationEquals";
      break;
    case bsrTime::kTimeRelationSlash:
      result = "timeRelationSlash";
      break;
    case bsrTime::kTimeRelationSpace:
      result = "timeRelationSpace";
      break;
    case bsrTime::kTimeRelationHyphen:
      result = "timeRelationHyphen";
      break;
  } // switch

  return result;
}

string bsrTime::asString () const
{
  stringstream s;

  s <<
    "Time, " << 
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", timeIsCompound: " <<
    booleanAsString (
      fTimeIsCompound) <<
    ", " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  if (fTimeItemsVector.size ()) {
    s <<
      ", ";

    vector<S_bsrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin (),
      iEnd   = fTimeItemsVector.end (),
      i      = iBegin;
  
    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }
  else {
    if (fTimeSymbolKind != bsrTime::kTimeSymbolSenzaMisura) {
      bsrInternalError (
        gXml2brlOptions->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "time  items vector is empty");
    }
  }

  return s.str ();
}

string bsrTime::asShortString () const
{
  /* JMI
  stringstream s;

  s <<
    "Time, " <<
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", compound " <<
    booleanAsString (
      fTimeIsCompound) <<
    ", " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber;

  return s.str ();
  */

  return asString ();
}

void bsrTime::print (ostream& os)
{
  os <<
    "Time" <<
    ", timeSymbolKind: " <<
    timeSymbolKindAsString (fTimeSymbolKind) <<
    ", compound: " <<
    booleanAsString (
      fTimeIsCompound) <<
    ", " <<
    singularOrPlural (
      fTimeItemsVector.size (), "item", "items") <<
    ", line " << fInputLineNumber <<
    ":";

  if (fTimeItemsVector.size ()) {
    os <<
      endl;
      
    gIndenter++;
    
    vector<S_bsrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin (),
      iEnd   = fTimeItemsVector.end (),
      i      = iBegin;
      
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for
    
    gIndenter--;
  }
  
  else
    {
      os <<
        " none" <<
        endl;
    }
}

ostream& operator<< (ostream& os, const S_bsrTime& elt)
{
  elt->print (os);
  return os;
}


}
