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

#include "bsrNumbers.h"

#include "utilities.h"
#include "waeMessagesHandling.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrNumber bsrNumber::create (
  int       inputLineNumber,
  int       numberValue,
  bsrNumberSignIsNeededKind
            numberSignIsNeededKind)
{
  bsrNumber* o =
    new bsrNumber (
      inputLineNumber, numberValue, numberSignIsNeededKind);
  assert (o != nullptr);
  return o;
}

bsrNumber::bsrNumber (
  int       inputLineNumber,
  int       numberValue,
  bsrNumberSignIsNeededKind
            numberSignIsNeededKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fNumberValue = numberValue;

  fNumberSignIsNeededKind = numberSignIsNeededKind;

  fNumberCellsList = buildCellsList ();
}

bsrNumber::~bsrNumber ()
{}

S_bsrCellsList bsrNumber::numberValueAsCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  int n = fNumberValue;

  bool numberValueIsNegative = false;

  if (n < 0) {
    numberValueIsNegative = true;
    n = -n;
  }

  while (n > 0) {
    int div = n / 10;
    int mod = n % 10;

/* JMI
    gLogStream << // JMI
      "% ==> numberValueAsCellsList ()" <<
      ", n = " << n <<
      ", div = " << div <<
      ", mod = " << mod <<
      endl;
*/

    bsrCellKind cellKind = bsrCellKind::kCellUnknown; // SEE TICINO

    switch (mod) {
      case 1: cellKind = kCell1; break;
      case 2: cellKind = kCell2; break;
      case 3: cellKind = kCell3; break;
      case 4: cellKind = kCell4; break;
      case 5: cellKind = kCell5; break;
      case 6: cellKind = kCell6; break;
      case 7: cellKind = kCell7; break;
      case 8: cellKind = kCell8; break;
      case 9: cellKind = kCell9; break;
      case 0: cellKind = kCell0; break;
      default:
        ;
    } // switch

    result->prependCellsListToCellsList (
      bsrCellsList::create (
        fInputLineNumber,
        cellKind));

    n = div;
  } // while

  if (numberValueIsNegative) {
    result->prependCellsListToCellsList (
      bsrCellsList::create (
        fInputLineNumber,
        kCell_ac_plus)); // JMI ??? other plus sign?
  }

  return result;
}

S_bsrCellsList bsrNumber::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append number sign if needed
  switch (fNumberSignIsNeededKind) {
    case bsrNumber::kNumberSignIsNeededYes:
      result->appendCellsListToCellsList (
        bsrCellsList::create (
          fInputLineNumber,
          kCellNumberSign));
      break;
    case bsrNumber::kNumberSignIsNeededNo:
      break;
  } // switch

  // append number value
  result->appendCellsListToCellsList (
    numberValueAsCellsList ());

  return result;
}

int bsrNumber::fetchCellsNumber() const
{
  return fNumberCellsList->fetchCellsNumber();
}

void bsrNumber::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrNumber::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrNumber>*
    p =
      dynamic_cast<visitor<S_bsrNumber>*> (v)) {
        S_bsrNumber elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrNumber::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrNumber::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrNumber::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrNumber>*
    p =
      dynamic_cast<visitor<S_bsrNumber>*> (v)) {
        S_bsrNumber elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrNumber::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrNumber::browseData (basevisitor* v)
{}

string bsrNumber::numberSignIsNeededKindAsString (
  bsrNumberSignIsNeededKind numberSignIsNeededKind)
{
  string result;

  switch (numberSignIsNeededKind) {
    case bsrNumber::kNumberSignIsNeededYes:
      result = "numberSignIsNeededYes";
      break;
    case bsrNumber::kNumberSignIsNeededNo:
      result = "numberSignIsNeededNo";
      break;
  } // switch

  return result;
}

string bsrNumber::asString () const
{
  stringstream s;

  s <<
    "Number" <<
    ", numberValue: " << fNumberValue <<
    ", numberSignIsNeeded: " <<
    numberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    ", numberCellsList: " <<
    fNumberCellsList->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrNumber::asDebugString () const
{
  stringstream s;

  s <<
    "N" << // JMI
    fNumberValue <<
    ", numberSignIsNeeded: " <<
    numberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    ", spacesBefore: " << fSpacesBefore;

  return s.str ();
}

void bsrNumber::print (ostream& os) const
{
  os <<
    "Number" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 21;

  os <<
    setw (fieldWidth) <<
    ", numberValue" << " : " << fNumberValue <<
    endl <<
    setw (fieldWidth) <<
    ", numberSignIsNeeded" << " : " <<
    numberSignIsNeededKindAsString (
      fNumberSignIsNeededKind) <<
    setw (fieldWidth) <<
    ", numberCellsList" << " : " <<
    fNumberCellsList->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrNumber& elt)
{
  elt->print (os);
  return os;
}


}
