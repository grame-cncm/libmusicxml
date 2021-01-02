/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>

#include "bsrTimes.h"

#include "bsrNotes.h"
#include "bsrNumbers.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

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

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
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

  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); ++i) {
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending beat number '" <<
      beatsNumber <<
      "' to time item '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  fTimeBeatsNumbersVector.insert (
    fTimeBeatsNumbersVector.end (),
    beatsNumber);
}

void bsrTimeItem::setTimeBeatValue (int timeBeatValue)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Setting beat value to '" <<
      timeBeatValue <<
      "' in time item '" <<
      asString () <<
      "'" <<
      endl;
    }
#endif

  fTimeBeatValue = timeBeatValue;
}

int bsrTimeItem::getTimeBeatsNumber () const
{
  int result = 0;

  for (unsigned int i = 0; i < fTimeBeatsNumbersVector.size (); ++i) {
    result +=
      fTimeBeatsNumbersVector [i];
  } // for

  return result;
}

void bsrTimeItem::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTimeItem::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTimeItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeItem>*> (v)) {
        S_bsrTimeItem elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTimeItem::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTimeItem::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTimeItem::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTimeItem>*
    p =
      dynamic_cast<visitor<S_bsrTimeItem>*> (v)) {
        S_bsrTimeItem elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTimeItem::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTimeItem::browseData (basevisitor* v)
{}

string bsrTimeItem::asString () const
{
  stringstream s;

  unsigned int timeBeatsNumbersVectorSize =
    fTimeBeatsNumbersVector.size ();

  s <<
    "TimeItem " <<
    ", " <<
    singularOrPlural (
      timeBeatsNumbersVectorSize, "timeBeatNumber", "timeBeatNumbers");

  switch (timeBeatsNumbersVectorSize) {
    case 0:
    /* JMI
      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "time item beats numbers vector is empty");
        */
      break;

    case 1:
      s <<
        ", " <<
        fTimeBeatsNumbersVector [0] << "/" << fTimeBeatValue;
      break;

    default:
      s <<
        ", beats numbers: ";

      for (unsigned int i = 0; i < timeBeatsNumbersVectorSize; ++i) {
        s <<
          fTimeBeatsNumbersVector [i];

        if (i != timeBeatsNumbersVectorSize - 1) {
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

void bsrTimeItem::print (ostream& os) const
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
    : bsrLineContentsElement (inputLineNumber)
{
  fTimeKind = timeKind;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
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

void bsrTime::appendTimeItem (S_bsrTimeItem timeItem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTimes ()) {
    gLogStream <<
      "Appending time item '" <<
      timeItem->asString () <<
      "' to time '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif

  fTimeItemsVector.push_back (timeItem);
}

S_bsrCellsList bsrTime::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  switch (fTimeKind) {
    case bsrTime::kTimeNone:
      break;

    case bsrTime::kTimeCommon:
      result->appendCellKindToCellsList (kCellNumberSign);
      result->appendCellKindToCellsList (kCell4);
      result->appendCellKindToCellsList (kCellLower4);
      break;

    case bsrTime::kTimeCut:
      result->appendCellKindToCellsList (kCellNumberSign);
      result->appendCellKindToCellsList (kCell2);
      result->appendCellKindToCellsList (kCellLower2);
      break;

    case bsrTime::kTimeNumerical: // JMI ???
    case bsrTime::kTimeNote:
    case bsrTime::kTimeDottedNote:
      {
        if (fTimeItemsVector.size ()) {
          vector<S_bsrTimeItem>::const_iterator
            iBegin = fTimeItemsVector.begin (),
        // JMI    iEnd   = fTimeItemsVector.end (),
            i      = iBegin;

          S_bsrTimeItem bTimeItem = (*i);

          const vector<int>&
            timeBeatsNumbersVector =
              bTimeItem->getTimeBeatsNumbersVector ();

          int bTimeBeatValue =
              bTimeItem->getTimeBeatValue ();

          unsigned int vectorSize =
            timeBeatsNumbersVector.size ();

          switch (vectorSize) {
            case 0:
            /* JMI
              msrInternalError (
                gGlobalOahOahGroup->getInputSourceName (),
                fInputLineNumber,
                __FILE__, __LINE__,
                "time item beats numbers vector is empty");
                */
              break;

            case 1:
              {
                // determine the beats number
                int beatsNumberToBeUsed = timeBeatsNumbersVector [0];

                switch (fTimeKind) {
                  case bsrTime::kTimeNote:
                    break;
                  case bsrTime::kTimeDottedNote:
                    beatsNumberToBeUsed /= 3;
                    break;
                  default:
                    ;
                } // switch

                // create the beats number
                S_bsrNumber
                  beatsNumber =
                    bsrNumber::create (
                      fInputLineNumber,
                      beatsNumberToBeUsed,
                      bsrNumber::kNumberSignIsNeededYes);

                // append it to result
                result->appendCellsListToCellsList (
                  beatsNumber->fetchCellsList ());

                // create music code indicator
                S_bsrCellsList
                  musicCodeIndicator =
                    bsrCellsList::create (
                      fInputLineNumber,
                      kDots6, kDots3);

                // append it to result
                result->appendCellsListToCellsList (
                  musicCodeIndicator);

                // determine the beat value to be used
                int beatValueToBeUsed = bTimeBeatValue;

                switch (fTimeKind) {
                  case bsrTime::kTimeNote:
                    break;
                  case bsrTime::kTimeDottedNote:
                    beatValueToBeUsed /= 2;
                    break;
                  default:
                    ;
                } // switch

                // append the beat value to result
                switch (beatValueToBeUsed) {
                  case 1:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueCWhole));
                    break;
                  case 2:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueCHalf));
                    break;
                  case 4:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueCQuarter));
                    break;
                  case 8:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueC8th));
                    break;
                  case 16:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueC16th));
                    break;
                  case 32:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueC32nd));
                    break;
                  case 64:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueC64th));
                    break;
                  case 128:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueC128th));
                    break;
                  case 256:
                    result->appendCellsListToCellsList (
                      bsrNote::noteValueKindAsCellsList (
                        fInputLineNumber,
                        bsrNote::kNoteValueC256th));
                    break;
                  default:
                    {
                      stringstream s;

                      s <<
                        "MSR time beat value '" <<
                        bTimeBeatValue <<
                        "' is not supported in Braille music";

                      bsrWarning (
                        gGlobalOahOahGroup->getInputSourceName (),
                        fInputLineNumber,
                        s.str ());
                    }
                } // switch

                // append a dot to the beat number if needed
                switch (fTimeKind) {
                  case bsrTime::kTimeNote:
                    break;
                  case bsrTime::kTimeDottedNote:
                    result->appendCellKindToCellsList (
                      kCellAugmentationDot);
                    break;
                  default:
                    ;
                } // switch
              }
              break;

            default:
              ;
            /*
              s <<
                "beats numbers: ";

              for (int i = 0; i < vectorSize; ++i) {
                s <<
                  timeBeatsNumbersVector [i];

                if (i != vectorSize - 1) {
                  s <<
                    " ";
                }
              } // for

              s <<
                ", beat value: " << fTimeBeatValue;
                */
          } // switch
        }
      }
      break;

    case bsrTime::kTimeSingleNumber:
      {
        if (fTimeItemsVector.size ()) {
          vector<S_bsrTimeItem>::const_iterator
            iBegin = fTimeItemsVector.begin (),
   // JMI         iEnd   = fTimeItemsVector.end (),
            i      = iBegin;

          S_bsrTimeItem bTimeItem = (*i);

          const vector<int>&
            timeBeatsNumbersVector =
              bTimeItem->getTimeBeatsNumbersVector ();

          int bTimeBeatValue =
              bTimeItem->getTimeBeatValue ();

          unsigned int vectorSize =
            timeBeatsNumbersVector.size ();

          switch (vectorSize) {
            case 0:
            /* JMI
              msrInternalError (
                gGlobalOahOahGroup->getInputSourceName (),
                fInputLineNumber,
                __FILE__, __LINE__,
                "time item beats numbers vector is empty");
                */
              break;

            case 1:
              {
                // create the beats number
                S_bsrNumber
                  beatsNumber =
                    bsrNumber::create (
                      fInputLineNumber,
                      timeBeatsNumbersVector [0],
                      bsrNumber::kNumberSignIsNeededYes);

                // append it to result
                result->appendCellsListToCellsList (
                  beatsNumber->fetchCellsList ());

                // append the beat value sign to result
                switch (bTimeBeatValue) {
                  case 1:
                    result->appendCellKindToCellsList (
                      kCellLower1);
                    break;
                  case 2:
                    result->appendCellKindToCellsList (
                      kCellLower2);
                    break;
                  case 4:
                    result->appendCellKindToCellsList (
                      kCellLower4);
                    break;
                  case 8:
                    result->appendCellKindToCellsList (
                      kCellLower8);
                    break;
                  case 16:
                    result->appendCellKindToCellsList (
                      kCellLower1);
                    result->appendCellKindToCellsList (
                      kCellLower6);
                    break;
                  case 32:
                    result->appendCellKindToCellsList (
                      kCellLower3);
                    result->appendCellKindToCellsList (
                      kCellLower2);
                    break;
                  default:
                    {
                      stringstream s;

                      s <<
                        "MSR time beat value '" <<
                        bTimeBeatValue <<
                        "' is not supported in Braille music";

                      bsrWarning (
                        gGlobalOahOahGroup->getInputSourceName (),
                        fInputLineNumber,
                        s.str ());
                    }
                } // switch
              }
              break;

            default:
              ;
            /*
              s <<
                "beats numbers: ";

              for (int i = 0; i < vectorSize; ++i) {
                s <<
                  timeBeatsNumbersVector [i];

                if (i != vectorSize - 1) {
                  s <<
                    " ";
                }
              } // for

              s <<
                ", beat value: " << fTimeBeatValue;
                */
          } // switch
        }
      }
      break;

    case bsrTime::kTimeSenzaMisura:
      break;
  } // switch

  return result;
}

int bsrTime::fetchCellsNumber() const
{
  // time items may have been appended after construction
  return fetchCellsList ()->fetchCellsNumber ();
}

void bsrTime::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTime::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTime>*
    p =
      dynamic_cast<visitor<S_bsrTime>*> (v)) {
        S_bsrTime elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTime::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTime::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrTime::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTime>*
    p =
      dynamic_cast<visitor<S_bsrTime>*> (v)) {
        S_bsrTime elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrTime::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTime::browseData (basevisitor* v)
{}

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
    case bsrTime::kTimeNote:
      result = "timeNote";
      break;
    case bsrTime::kTimeDottedNote:
      result = "timeDottedNote";
      break;
    case bsrTime::kTimeSingleNumber:
      result = "timeSingleNumber";
      break;
    case bsrTime::kTimeSenzaMisura:
      result = "timeSenzaMisura";
      break;
  } // switch

  return result;
}

string bsrTime::timeKindAsDebugString (
  bsrTimeKind timeKind)
{
  string result;

  switch (timeKind) {
    case bsrTime::kTimeNone:
      result = "_";
      break;

    case bsrTime::kTimeCommon:
      result = "C";
      break;
    case bsrTime::kTimeCut:
      result = "C/";
      break;
    case bsrTime::kTimeNumerical:
      result = "Num";
      break;
    case bsrTime::kTimeNote:
      result = "Note";
      break;
    case bsrTime::kTimeDottedNote:
      result = "DottedNote";
      break;
    case bsrTime::kTimeSingleNumber:
      result = "SingleNumber";
      break;
    case bsrTime::kTimeSenzaMisura:
      result = "SenzaMisura";
      break;
  } // switch

  return result;
}

string bsrTime::asString () const
{
  stringstream s;

  s <<
    "Time" <<
    ", timeKind " << " : " <<
    timeKindAsString (fTimeKind) <<
    ", timeCellsList: " << fetchCellsList ()->asString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrTime::asDebugString () const
{
  stringstream s;

  s <<
    "[TIME " <<
    timeKindAsDebugString (fTimeKind) <<
    "]";

  return s.str ();
}

void bsrTime::print (ostream& os) const
{
  unsigned int timeItemsVectorSize = fTimeItemsVector.size ();

  os <<
    "Time" <<
    ", " <<
    singularOrPlural (
      timeItemsVectorSize, "item", "items") <<
    ", line " << fInputLineNumber <<
    ":" <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 16;

  os << left <<
    setw (fieldWidth) <<
    "timeKind " << " : " <<
    timeKindAsString (fTimeKind) <<
    endl <<
    setw (fieldWidth) <<
    "timeItemsVector" << " : ";

  if (timeItemsVectorSize) {
    os << endl;

    ++gIndenter;

    vector<S_bsrTimeItem>::const_iterator
      iBegin = fTimeItemsVector.begin (),
      iEnd   = fTimeItemsVector.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    --gIndenter;
  }

  else {
    os <<
      " none" <<
      endl;
  }

  os <<
    setw (fieldWidth) <<
    "timeCellsList" << " : " << fetchCellsList ()->asString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
   endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrTime& elt)
{
  elt->print (os);
  return os;
}


}
