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

#include "bsrMutualDependencies.h"

#include "messagesHandling.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrNote bsrNote::create (
  int                   inputLineNumber,
  bsrNoteValueKind      noteValueKind,
  int                   noteDotsNumber,
  bsrNoteOctaveKind     noteOctaveKind,
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded,
  bsrNoteAccidentalKind noteAccidentalKind)
{
  bsrNote* o =
    new bsrNote (
      inputLineNumber,
      noteValueKind,
      noteDotsNumber,
      noteOctaveKind,
      noteOctaveIsNeeded,
      noteAccidentalKind);
  assert (o!=0);
  return o;
}

bsrNote::bsrNote (
  int                   inputLineNumber,
  bsrNoteValueKind      noteValueKind,
  int                   noteDotsNumber,
  bsrNoteOctaveKind     noteOctaveKind,
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded,
  bsrNoteAccidentalKind noteAccidentalKind)
    : bsrLineElement (inputLineNumber)
{
  fNoteValueKind  = noteValueKind;
  fNoteDotsNumber = noteDotsNumber;

  fNoteValueSizeIsNeeded = false;

  fNoteOctaveKind = noteOctaveKind;
  fNoteOctaveIsNeeded = noteOctaveIsNeeded;

  fNoteAccidentalKind = noteAccidentalKind;
}

bsrNote::~bsrNote ()
{}

S_bsrCellsList bsrNote::noteValueKindAsCellsList (
  int              inputLineNumber,
  bsrNoteValueKind noteValueKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteValueKind) {
    case kNoteValueNone:
      break;

    case kNoteValueRestBreve:
      result->appendCellKindToCellsList (kDots134);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteValueRestWhole:
      result->appendCellKindToCellsList (kDots134);
      break;
    case kNoteValueRestHalf:
      result->appendCellKindToCellsList (kDots136);
      break;
    case kNoteValueRestQuarter:
      result->appendCellKindToCellsList (kDots1236);
      break;
    case kNoteValueRest8th:
      result->appendCellKindToCellsList (kDots1346);
      break;
    case kNoteValueRest16th:
      result->appendCellKindToCellsList (kDots134);
      break;
    case kNoteValueRest32nd:
      result->appendCellKindToCellsList (kDots136);
      break;
    case kNoteValueRest64th:
      result->appendCellKindToCellsList (kDots1236);
      break;
    case kNoteValueRest128th:
      result->appendCellKindToCellsList (kDots1346);
      break;
    case kNoteValueRest256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots134);
      break;

    case kNoteValueCBreve:
      result->appendCellKindToCellsList (kDots13456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteValueCWhole:
      result->appendCellKindToCellsList (kDots13456);
      break;
    case kNoteValueCHalf:
      result->appendCellKindToCellsList (kDots1345);
      break;
    case kNoteValueCQuarter:
      result->appendCellKindToCellsList (kDots1456);
      break;
    case kNoteValueC8th:
      result->appendCellKindToCellsList (kDots145);
      break;
    case kNoteValueC16th:
      result->appendCellKindToCellsList (kDots13456);
      break;
    case kNoteValueC32nd:
      result->appendCellKindToCellsList (kDots1345);
      break;
    case kNoteValueC64th:
      result->appendCellKindToCellsList (kDots1456);
      break;
    case kNoteValueC128th:
      result->appendCellKindToCellsList (kDots145);
      break;
    case kNoteValueC256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots13456);
      break;

    case kNoteValueDBreve:
      result->appendCellKindToCellsList (kDots1356);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteValueDWhole:
      result->appendCellKindToCellsList (kDots1356);
      break;
    case kNoteValueDHalf:
      result->appendCellKindToCellsList (kDots135);
      break;
    case kNoteValueDQuarter:
      result->appendCellKindToCellsList (kDots156);
      break;
    case kNoteValueD8th:
      result->appendCellKindToCellsList (kDots15);
      break;
    case kNoteValueD16th:
      result->appendCellKindToCellsList (kDots1356);
      break;
    case kNoteValueD32nd:
      result->appendCellKindToCellsList (kDots135);
      break;
    case kNoteValueD64th:
      result->appendCellKindToCellsList (kDots156);
      break;
    case kNoteValueD128th:
      result->appendCellKindToCellsList (kDots15);
      break;
    case kNoteValueD256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots1356);
      break;

    case kNoteValueEBreve:
      result->appendCellKindToCellsList (kDots12346);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteValueEWhole:
      result->appendCellKindToCellsList (kDots12346);
      break;
    case kNoteValueEHalf:
      result->appendCellKindToCellsList (kDots1234);
      break;
    case kNoteValueEQuarter:
      result->appendCellKindToCellsList (kDots1246);
      break;
    case kNoteValueE8th:
      result->appendCellKindToCellsList (kDots124);
      break;
    case kNoteValueE16th:
      result->appendCellKindToCellsList (kDots12346);
      break;
    case kNoteValueE32nd:
      result->appendCellKindToCellsList (kDots1234);
      break;
    case kNoteValueE64th:
      result->appendCellKindToCellsList (kDots1246);
      break;
    case kNoteValueE128th:
      result->appendCellKindToCellsList (kDots124);
      break;
    case kNoteValueE256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots12346);
      break;

    case kNoteValueFBreve:
      result->appendCellKindToCellsList (kDots123456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteValueFWhole:
      result->appendCellKindToCellsList (kDots123456);
      break;
    case kNoteValueFHalf:
      result->appendCellKindToCellsList (kDots12345);
      break;
    case kNoteValueFQuarter:
      result->appendCellKindToCellsList (kDots12456);
      break;
    case kNoteValueF8th:
      result->appendCellKindToCellsList (kDots1245);
      break;
    case kNoteValueF16th:
      result->appendCellKindToCellsList (kDots123456);
      break;
    case kNoteValueF32nd:
      result->appendCellKindToCellsList (kDots12345);
      break;
    case kNoteValueF64th:
      result->appendCellKindToCellsList (kDots12456);
      break;
    case kNoteValueF128th:
      result->appendCellKindToCellsList (kDots1245);
      break;
    case kNoteValueF256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots123456);
      break;

    case kNoteValueGBreve:
      result->appendCellKindToCellsList (kDots12356);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteValueGWhole:
      result->appendCellKindToCellsList (kDots12356);
      break;
    case kNoteValueGHalf:
      result->appendCellKindToCellsList (kDots1235);
      break;
    case kNoteValueGQuarter:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteValueG8th:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteValueG16th:
      result->appendCellKindToCellsList (kDots12356);
      break;
    case kNoteValueG32nd:
      result->appendCellKindToCellsList (kDots1235);
      break;
    case kNoteValueG64th:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteValueG128th:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteValueG256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots12356);
      break;

    case kNoteValueABreve:
      result->appendCellKindToCellsList (kDots2346);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteValueAWhole:
      result->appendCellKindToCellsList (kDots2346);
      break;
    case kNoteValueAHalf:
      result->appendCellKindToCellsList (kDots234);
      break;
    case kNoteValueAQuarter:
      result->appendCellKindToCellsList (kDots246);
      break;
    case kNoteValueA8th:
      result->appendCellKindToCellsList (kDots24);
      break;
    case kNoteValueA16th:
      result->appendCellKindToCellsList (kDots2346);
      break;
    case kNoteValueA32nd:
      result->appendCellKindToCellsList (kDots234);
      break;
    case kNoteValueA64th:
      result->appendCellKindToCellsList (kDots246);
      break;
    case kNoteValueA128th:
      result->appendCellKindToCellsList (kDots24);
      break;
    case kNoteValueA256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots2346);
      break;

    case kNoteValueBBreve:
      result->appendCellKindToCellsList (kDots23456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteValueBWhole:
      result->appendCellKindToCellsList (kDots23456);
      break;
    case kNoteValueBHalf:
      result->appendCellKindToCellsList (kDots2345);
      break;
    case kNoteValueBQuarter:
      result->appendCellKindToCellsList (kDots2456);
      break;
    case kNoteValueB8th:
      result->appendCellKindToCellsList (kDots245);
      break;
    case kNoteValueB16th:
      result->appendCellKindToCellsList (kDots23456);
      break;
    case kNoteValueB32nd:
      result->appendCellKindToCellsList (kDots2345);
      break;
    case kNoteValueB64th:
      result->appendCellKindToCellsList (kDots2456);
      break;
    case kNoteValueB128th:
      result->appendCellKindToCellsList (kDots245);
      break;
    case kNoteValueB256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots23456);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::noteValueKindAsCellsList () const
{
  return
    noteValueKindAsCellsList (
      fInputLineNumber,
      fNoteValueKind);
}

S_bsrCellsList bsrNote::noteValueSizeKindAsCellsList () const
{
  return
    noteValueSizeKindAsCellsList (
      fInputLineNumber,
      noteValueSizeKindFromNoteValueKind (
        fNoteValueKind));
}

S_bsrCellsList bsrNote::noteOctaveKindAsCellsList (
  int               inputLineNumber,
  bsrNoteOctaveKind noteOctaveKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteOctaveKind) {
    case kNoteOctaveNone:
      break;
    case kNoteOctaveBelow1:
      result->appendCellKindToCellsList (kDots4);
      result->appendCellKindToCellsList (kDots4);
      break;
    case kNoteOctave1:
      result->appendCellKindToCellsList (kDots4);
      break;
    case kNoteOctave2:
      result->appendCellKindToCellsList (kDots45);
      break;
    case kNoteOctave3:
      result->appendCellKindToCellsList (kDots456);
      break;
    case kNoteOctave4:
      result->appendCellKindToCellsList (kDots5);
      break;
    case kNoteOctave5:
      result->appendCellKindToCellsList (kDots46);
      break;
    case kNoteOctave6:
      result->appendCellKindToCellsList (kDots56);
      break;
    case kNoteOctave7:
      result->appendCellKindToCellsList (kDots6);
      break;
    case kNoteOctaveAbove7:
      result->appendCellKindToCellsList (kDots6);
      result->appendCellKindToCellsList (kDots6);
      break;
  } // switch

  return result;
}

bsrNote::bsrNoteValueSizeKind bsrNote::noteValueSizeKindFromNoteValueKind (
  bsrNoteValueKind noteValueKind)
{
  bsrNoteValueSizeKind
    result = bsrNote::kNoteValueSizeNone;

  switch (noteValueKind) {
    case kNoteValueNone:
      break;

    case kNoteValueRestBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRestWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRestHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRestQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRest8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueRest16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueRest32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueRest64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueRest128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueRest256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueCBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueCWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueCHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueCQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueC8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueC16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueC32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueC64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueC128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueC256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueDBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueDWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueDHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueDQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueD8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueD16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueD32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueD64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueD128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueD256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueEBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueEWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueEHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueEQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueE8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueE16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueE32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueE64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueE128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueE256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueFBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueFWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueFHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueFQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueF8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueF16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueF32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueF64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueF128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueF256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueGBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueGWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueGHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueGQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueG8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueG16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueG32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueG64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueG128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueG256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueABreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueAWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueAHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueAQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueA8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueA16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueA32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueA64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueA128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueA256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;

    case kNoteValueBBreve:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueBWhole:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueBHalf:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueBQuarter:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueB8th:
      result = bsrNote::kNoteValueSizeLarger;
      break;
    case kNoteValueB16th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueB32nd:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueB64th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueB128th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
    case kNoteValueB256th:
      result = bsrNote::kNoteValueSizeSmaller;
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::noteValueSizeKindAsCellsList (
  int                  inputLineNumber,
  bsrNoteValueSizeKind noteValueSizeKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteValueSizeKind) {
    case bsrNote::kNoteValueSizeNone:
      break;
    case bsrNote::kNoteValueSizeLarger:
      result->appendCellKindToCellsList (kDots45);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      break;
    case bsrNote::kNoteValueSizeSmaller:
      result->appendCellKindToCellsList (kDots6);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::noteOctaveKindAsCellsList () const
{
  return
    noteOctaveKindAsCellsList (
      fInputLineNumber,
      fNoteOctaveKind);
}

S_bsrCellsList bsrNote::noteAccidentalKindAsCellsList (
  int                   inputLineNumber,
  bsrNoteAccidentalKind noteAccidentalKind)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);

  switch (noteAccidentalKind) {
    case bsrNote::kNoteAccidentalNone:
      break;

    case bsrNote::kNoteAccidentalSharp:
      result->appendCellKindToCellsList (kCellSharp);
      break;
    case bsrNote::kNoteAccidentalNatural:
      result->appendCellKindToCellsList (kCellNatural);
      break;
    case bsrNote::kNoteAccidentalFlat:
      result->appendCellKindToCellsList (kCellFlat);
      break;

    case bsrNote::kNoteAccidentalDoubleFlat:
      result->appendCellKindToCellsList (kCellFlat);
      result->appendCellKindToCellsList (kCellFlat);
      break;
    case bsrNote::kNoteAccidentalDoubleSharp:
      result->appendCellKindToCellsList (kCellSharp);
      result->appendCellKindToCellsList (kCellSharp);
      break;

    case bsrNote::kNoteAccidentalQuarterSharp:
      result->appendCellKindToCellsList (kDots4);
      result->appendCellKindToCellsList (kCellSharp);
      break;
    case bsrNote::kNoteAccidentalQuarterFlat:
      result->appendCellKindToCellsList (kDots4);
      result->appendCellKindToCellsList (kCellFlat);
      break;

    case bsrNote::kNoteAccidentalThreeQuarterSharp:
      result->appendCellKindToCellsList (kDots456);
      result->appendCellKindToCellsList (kCellSharp);
      break;

    case bsrNote::kNoteAccidentalThreeQuarterFlat:
      result->appendCellKindToCellsList (kDots456);
      result->appendCellKindToCellsList (kCellFlat);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::noteAccidentalKindAsCellsList () const
{
  return
    noteAccidentalKindAsCellsList (
      fInputLineNumber,
      fNoteAccidentalKind);
}

S_bsrCellsList bsrNote::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append the note value sign if needed
  if (fNoteValueSizeIsNeeded) {
    result->appendCellsListToCellsList (
      noteValueSizeKindAsCellsList ());
  }

  // append note accidental if any
  result->appendCellsListToCellsList (
    noteAccidentalKindAsCellsList ());

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

  return result;
}

int bsrNote::fetchCellsNumber() const
{
  return buildCellsList ()->fetchCellsNumber();
}

void bsrNote::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrNote::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrNote>*
    p =
      dynamic_cast<visitor<S_bsrNote>*> (v)) {
        S_bsrNote elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrNote::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrNote::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrNote::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrNote>*
    p =
      dynamic_cast<visitor<S_bsrNote>*> (v)) {
        S_bsrNote elem = this;

#ifdef TRACE_OAH
        if (gBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrNote::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrNote::browseData (basevisitor* v)
{}

string bsrNote::noteValueKindAsString (
  bsrNoteValueKind noteValueKind)
{
  string result;

  switch (noteValueKind) {
    case kNoteValueNone: result = "None"; break;

    case kNoteValueRestBreve: result = "RestBreve"; break;
    case kNoteValueRestWhole: result = "RestWhole"; break;
    case kNoteValueRestHalf: result = "RestHalf"; break;
    case kNoteValueRestQuarter: result = "RestQuarter"; break;
    case kNoteValueRest8th: result = "Rest8th"; break;
    case kNoteValueRest16th: result = "Rest16th"; break;
    case kNoteValueRest32nd: result = "Rest32nd"; break;
    case kNoteValueRest64th: result = "Rest64th"; break;
    case kNoteValueRest128th: result = "Rest128th"; break;
    case kNoteValueRest256th: result = "Rest256th"; break;

    case kNoteValueCBreve: result = "CBreve"; break;
    case kNoteValueCWhole: result = "CWhole"; break;
    case kNoteValueCHalf: result = "CHalf"; break;
    case kNoteValueCQuarter: result = "CQuarter"; break;
    case kNoteValueC8th: result = "C8th"; break;
    case kNoteValueC16th: result = "C16th"; break;
    case kNoteValueC32nd: result = "C32nd"; break;
    case kNoteValueC64th: result = "C64th"; break;
    case kNoteValueC128th: result = "C128th"; break;
    case kNoteValueC256th: result = "C256th"; break;

    case kNoteValueDBreve: result = "DBreve"; break;
    case kNoteValueDWhole: result = "DWhole"; break;
    case kNoteValueDHalf: result = "DHalf"; break;
    case kNoteValueDQuarter: result = "DQuarter"; break;
    case kNoteValueD8th: result = "D8th"; break;
    case kNoteValueD16th: result = "D16th"; break;
    case kNoteValueD32nd: result = "D32nd"; break;
    case kNoteValueD64th: result = "D64th"; break;
    case kNoteValueD128th: result = "D128th"; break;
    case kNoteValueD256th: result = "D256th"; break;

    case kNoteValueEBreve: result = "EBreve"; break;
    case kNoteValueEWhole: result = "EWhole"; break;
    case kNoteValueEHalf: result = "EHalf"; break;
    case kNoteValueEQuarter: result = "EQuarter"; break;
    case kNoteValueE8th: result = "E8th"; break;
    case kNoteValueE16th: result = "E16th"; break;
    case kNoteValueE32nd: result = "E32nd"; break;
    case kNoteValueE64th: result = "E64th"; break;
    case kNoteValueE128th: result = "E128th"; break;
    case kNoteValueE256th: result = "E256th"; break;

    case kNoteValueFBreve: result = "FBreve"; break;
    case kNoteValueFWhole: result = "FWhole"; break;
    case kNoteValueFHalf: result = "FHalf"; break;
    case kNoteValueFQuarter: result = "FQuarter"; break;
    case kNoteValueF8th: result = "F8th"; break;
    case kNoteValueF16th: result = "F16th"; break;
    case kNoteValueF32nd: result = "F32nd"; break;
    case kNoteValueF64th: result = "F64th"; break;
    case kNoteValueF128th: result = "F128th"; break;
    case kNoteValueF256th: result = "F256th"; break;

    case kNoteValueGBreve: result = "GBreve"; break;
    case kNoteValueGWhole: result = "GWhole"; break;
    case kNoteValueGHalf: result = "GHalf"; break;
    case kNoteValueGQuarter: result = "GQuarter"; break;
    case kNoteValueG8th: result = "G8th"; break;
    case kNoteValueG16th: result = "G16th"; break;
    case kNoteValueG32nd: result = "G32nd"; break;
    case kNoteValueG64th: result = "G64th"; break;
    case kNoteValueG128th: result = "G128th"; break;
    case kNoteValueG256th: result = "G256th"; break;

    case kNoteValueABreve: result = "ABreve"; break;
    case kNoteValueAWhole: result = "AWhole"; break;
    case kNoteValueAHalf: result = "AHalf"; break;
    case kNoteValueAQuarter: result = "AQuarter"; break;
    case kNoteValueA8th: result = "A8th"; break;
    case kNoteValueA16th: result = "A16th"; break;
    case kNoteValueA32nd: result = "A32nd"; break;
    case kNoteValueA64th: result = "A64th"; break;
    case kNoteValueA128th: result = "A128th"; break;
    case kNoteValueA256th: result = "A256th"; break;

    case kNoteValueBBreve: result = "BBreve"; break;
    case kNoteValueBWhole: result = "BWhole"; break;
    case kNoteValueBHalf: result = "BHalf"; break;
    case kNoteValueBQuarter: result = "BQuarter"; break;
    case kNoteValueB8th: result = "B8th"; break;
    case kNoteValueB16th: result = "B16th"; break;
    case kNoteValueB32nd: result = "B32nd"; break;
    case kNoteValueB64th: result = "B64th"; break;
    case kNoteValueB128th: result = "B128th"; break;
    case kNoteValueB256th: result = "B256th"; break;
  } // switch

  return result;
}

string bsrNote::noteValueSizeKindAsString (
  bsrNoteValueSizeKind noteValueSizeKind)
{
  string result;

  switch (noteValueSizeKind) {
    case bsrNote::kNoteValueSizeNone:
      result = "None";
      break;
    case bsrNote::kNoteValueSizeLarger:
      result = "noteValueSizeLarger";
      break;
    case bsrNote::kNoteValueSizeSmaller:
      result = "noteValueSizeSmaller";
      break;
  } // switch

  return result;
}

string bsrNote::noteOctaveKindAsString (
  bsrNoteOctaveKind noteOctaveKind)
{
  string result;

  switch (noteOctaveKind) {
    case kNoteOctaveNone:   result = "octaveNone"; break;
    case kNoteOctaveBelow1: result = "octaveBelow1"; break;
    case kNoteOctave1:      result = "octave1"; break;
    case kNoteOctave2:      result = "octave2"; break;
    case kNoteOctave3:      result = "octave3"; break;
    case kNoteOctave4:      result = "octave4"; break;
    case kNoteOctave5:      result = "octave5"; break;
    case kNoteOctave6:      result = "octave6"; break;
    case kNoteOctave7:      result = "octave7"; break;
    case kNoteOctaveAbove7: result = "octaveAbove7"; break;
  } // switch

  return result;
}

string bsrNote::noteOctaveIsNeededAsString (
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded)
{
  string result;

  switch (noteOctaveIsNeeded) {
    case bsrNote::kNoteOctaveIsNeededYes:
      result = "noteOctaveIsNeededYes";
      break;
    case bsrNote::kNoteOctaveIsNeededNo:
      result = "noteOctaveIsNeededNo";
      break;
  } // switch

  return result;
}

string bsrNote::noteAccidentalKindAsString (
  bsrNoteAccidentalKind noteAccidentalKind)
{
  string result;

  switch (noteAccidentalKind) {
    case bsrNote::kNoteAccidentalNone:
      result = "noteAccidentalNone";
      break;

    case bsrNote::kNoteAccidentalSharp:
      result = "noteAccidentalSharp";
      break;
    case bsrNote::kNoteAccidentalNatural:
      result = "noteAccidentalNatural";
      break;
    case bsrNote::kNoteAccidentalFlat:
      result = "noteAccidentalFlat";
      break;

    case bsrNote::kNoteAccidentalDoubleFlat:
      result = "noteAccidentalDoubleFlat";
      break;
    case bsrNote::kNoteAccidentalDoubleSharp:
      result = "noteAccidentalDoubleSharp";
      break;

    case bsrNote::kNoteAccidentalQuarterSharp:
      result = "noteAccidentalQuarterSharp";
      break;
    case bsrNote::kNoteAccidentalQuarterFlat:
      result = "noteAccidentalQuarterFlat";
      break;

    case bsrNote::kNoteAccidentalThreeQuarterSharp:
      result = "noteAccidentalThreeQuarterSharp";
      break;

    case bsrNote::kNoteAccidentalThreeQuarterFlat:
      result = "noteAccidentalThreeQuarterFlat";
      break;
  } // switch

  return result;
}

string bsrNote::asString () const
{
  stringstream s;

  s <<
    "Note" <<
    ", noteValue: " <<
    noteValueKindAsString (fNoteValueKind) <<
    ", noteDotsNumber: " << fNoteDotsNumber <<
    ", noteOctaveKind: " <<
    noteOctaveKindAsString (fNoteOctaveKind) <<
    ", noteValueSizeIsNeeded: " <<
    booleanAsString (fNoteValueSizeIsNeeded) <<
    ", noteOctaveIsNeeded: " <<
    noteOctaveIsNeededAsString (fNoteOctaveIsNeeded) <<
    ", noteAccidentalKindAsString: " <<
    noteAccidentalKindAsString (fNoteAccidentalKind) <<
    ", cellsList (): " <<
    buildCellsList ()->asShortString () <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrNote::print (ostream& os) const
{
  os <<
    "Note" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 23;

  os <<
    setw (fieldWidth) <<
    "noteValue" << " : " <<
    noteValueKindAsString (fNoteValueKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteDotsNumber" << " : " << fNoteDotsNumber <<
    endl <<
    setw (fieldWidth) <<
    "noteOctaveKind" << " : " <<
    noteOctaveKindAsString (fNoteOctaveKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteValueSizeIsNeeded" << " : " <<
    booleanAsString (fNoteValueSizeIsNeeded) <<
    endl <<
    setw (fieldWidth) <<
    "noteOctaveIsNeeded" << " : " <<
    noteOctaveIsNeededAsString (fNoteOctaveIsNeeded) <<
    endl <<
    setw (fieldWidth) <<
    "noteAccidentalKind" << " : " <<
    noteAccidentalKindAsString (fNoteAccidentalKind) <<
    endl <<
    setw (fieldWidth) <<
    "cellsList" << " : " <<
    buildCellsList ()->asShortString () <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrNote& elt)
{
  elt->print (os);
  return os;
}


}
