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

#include "bsrNotes.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
  #include "bsrTraceOptions.h"
#endif

#include "bsrOptions.h"
#include "xml2brlOptionsHandling.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrNote bsrNote::create (
  int                   inputLineNumber,
  bsrNoteValueKind      noteValueKind,
  int                   noteDotsNumber,
  bsrNoteOctaveKind     noteOctaveKind,
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded)
{
  bsrNote* o =
    new bsrNote (
      inputLineNumber,
      noteValueKind,
      noteDotsNumber,
      noteOctaveKind,
      noteOctaveIsNeeded);
  assert (o!=0);
  return o;
}

bsrNote::bsrNote (
  int                   inputLineNumber,
  bsrNoteValueKind      noteValueKind,
  int                   noteDotsNumber,
  bsrNoteOctaveKind     noteOctaveKind,
  bsrNoteOctaveIsNeeded noteOctaveIsNeeded)
    : bsrLineElement (inputLineNumber)
{
  fNoteValueKind  = noteValueKind;
  fNoteDotsNumber = noteDotsNumber;
  
  fNoteOctaveKind = noteOctaveKind;
    
  fNoteOctaveIsNeeded = noteOctaveIsNeeded;

  fNoteCellsList = asCellsList ();
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
    case kNoteKindNone:
      break;
      
    case kNoteKindRestBreve:
      result->appendCellKindToCellsList (kDots134);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteKindRestWhole:
      result->appendCellKindToCellsList (kDots134);
      break;
    case kNoteKindRestHalf:
      result->appendCellKindToCellsList (kDots136);
      break;
    case kNoteKindRestQuarter:
      result->appendCellKindToCellsList (kDots1236);
      break;
    case kNoteKindRest8th:
      result->appendCellKindToCellsList (kDots1346);
      break;
    case kNoteKindRest16th:
      result->appendCellKindToCellsList (kDots134);
      break;
    case kNoteKindRest32nd:
      result->appendCellKindToCellsList (kDots136);
      break;
    case kNoteKindRest64th:
      result->appendCellKindToCellsList (kDots1236);
      break;
    case kNoteKindRest128th:
      result->appendCellKindToCellsList (kDots1346);
      break;
    case kNoteKindRest256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots134);
      break;

    case kNoteKindCBreve:
      result->appendCellKindToCellsList (kDots13456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteKindCWhole:
      result->appendCellKindToCellsList (kDots13456);
      break;
    case kNoteKindCHalf:
      result->appendCellKindToCellsList (kDots1345);
      break;
    case kNoteKindCQuarter:
      result->appendCellKindToCellsList (kDots1456);
      break;
    case kNoteKindC8th:
      result->appendCellKindToCellsList (kDots145);
      break;
    case kNoteKindC16th:
      result->appendCellKindToCellsList (kDots13456);
      break;
    case kNoteKindC32nd:
      result->appendCellKindToCellsList (kDots1345);
      break;
    case kNoteKindC64th:
      result->appendCellKindToCellsList (kDots1456);
      break;
    case kNoteKindC128th:
      result->appendCellKindToCellsList (kDots145);
      break;
    case kNoteKindC256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots13456);
      break;

    case kNoteKindDBreve:
      result->appendCellKindToCellsList (kDots1356);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteKindDWhole:
      result->appendCellKindToCellsList (kDots1356);
      break;
    case kNoteKindDHalf:
      result->appendCellKindToCellsList (kDots135);
      break;
    case kNoteKindDQuarter:
      result->appendCellKindToCellsList (kDots156);
      break;
    case kNoteKindD8th:
      result->appendCellKindToCellsList (kDots15);
      break;
    case kNoteKindD16th:
      result->appendCellKindToCellsList (kDots1356);
      break;
    case kNoteKindD32nd:
      result->appendCellKindToCellsList (kDots135);
      break;
    case kNoteKindD64th:
      result->appendCellKindToCellsList (kDots156);
      break;
    case kNoteKindD128th:
      result->appendCellKindToCellsList (kDots15);
      break;
    case kNoteKindD256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots1356);
      break;

    case kNoteKindEBreve:
      result->appendCellKindToCellsList (kDots12346);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteKindEWhole:
      result->appendCellKindToCellsList (kDots12346);
      break;
    case kNoteKindEHalf:
      result->appendCellKindToCellsList (kDots1234);
      break;
    case kNoteKindEQuarter:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteKindE8th:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteKindE16th:
      result->appendCellKindToCellsList (kDots12346);
      break;
    case kNoteKindE32nd:
      result->appendCellKindToCellsList (kDots1234);
      break;
    case kNoteKindE64th:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteKindE128th:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteKindE256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots12346);
      break;

    case kNoteKindFBreve:
      result->appendCellKindToCellsList (kDots123456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteKindFWhole:
      result->appendCellKindToCellsList (kDots123456);
      break;
    case kNoteKindFHalf:
      result->appendCellKindToCellsList (kDots12345);
      break;
    case kNoteKindFQuarter:
      result->appendCellKindToCellsList (kDots12456);
      break;
    case kNoteKindF8th:
      result->appendCellKindToCellsList (kDots1245);
      break;
    case kNoteKindF16th:
      result->appendCellKindToCellsList (kDots123456);
      break;
    case kNoteKindF32nd:
      result->appendCellKindToCellsList (kDots12345);
      break;
    case kNoteKindF64th:
      result->appendCellKindToCellsList (kDots12456);
      break;
    case kNoteKindF128th:
      result->appendCellKindToCellsList (kDots1245);
      break;
    case kNoteKindF256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots123456);
      break;

    case kNoteKindGBreve:
      result->appendCellKindToCellsList (kDots12356);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteKindGWhole:
      result->appendCellKindToCellsList (kDots12356);
      break;
    case kNoteKindGHalf:
      result->appendCellKindToCellsList (kDots1235);
      break;
    case kNoteKindGQuarter:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteKindG8th:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteKindG16th:
      result->appendCellKindToCellsList (kDots12356);
      break;
    case kNoteKindG32nd:
      result->appendCellKindToCellsList (kDots1235);
      break;
    case kNoteKindG64th:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteKindG128th:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteKindG256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots12356);
      break;

    case kNoteKindABreve:
      result->appendCellKindToCellsList (kDots2346);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteKindAWhole:
      result->appendCellKindToCellsList (kDots2346);
      break;
    case kNoteKindAHalf:
      result->appendCellKindToCellsList (kDots234);
      break;
    case kNoteKindAQuarter:
      result->appendCellKindToCellsList (kDots246);
      break;
    case kNoteKindA8th:
      result->appendCellKindToCellsList (kDots24);
      break;
    case kNoteKindA16th:
      result->appendCellKindToCellsList (kDots2346);
      break;
    case kNoteKindA32nd:
      result->appendCellKindToCellsList (kDots234);
      break;
    case kNoteKindA64th:
      result->appendCellKindToCellsList (kDots246);
      break;
    case kNoteKindA128th:
      result->appendCellKindToCellsList (kDots24);
      break;
    case kNoteKindA256th:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots2346);
      break;

    case kNoteKindBBreve:
      result->appendCellKindToCellsList (kDots23456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteKindBWhole:
      result->appendCellKindToCellsList (kDots23456);
      break;
    case kNoteKindBHalf:
      result->appendCellKindToCellsList (kDots2345);
      break;
    case kNoteKindBQuarter:
      result->appendCellKindToCellsList (kDots2456);
      break;
    case kNoteKindB8th:
      result->appendCellKindToCellsList (kDots245);
      break;
    case kNoteKindB16th:
      result->appendCellKindToCellsList (kDots23456);
      break;
    case kNoteKindB32nd:
      result->appendCellKindToCellsList (kDots2345);
      break;
    case kNoteKindB64th:
      result->appendCellKindToCellsList (kDots2456);
      break;
    case kNoteKindB128th:
      result->appendCellKindToCellsList (kDots245);
      break;
    case kNoteKindB256th:
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

S_bsrCellsList bsrNote::noteOctaveKindAsCellsList () const
{
  return
    noteOctaveKindAsCellsList (
      fInputLineNumber,
      fNoteOctaveKind);
}

S_bsrCellsList bsrNote::asCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

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
  return asCellsList ()->fetchCellsNumber();
}

void bsrNote::acceptIn (basevisitor* v)
{
  if (gBsrTraceOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrNote::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrNote>*
    p =
      dynamic_cast<visitor<S_bsrNote>*> (v)) {
        S_bsrNote elem = this;
        
        if (gBsrTraceOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrNote::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrNote::acceptOut (basevisitor* v)
{
  if (gBsrTraceOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrNote::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrNote>*
    p =
      dynamic_cast<visitor<S_bsrNote>*> (v)) {
        S_bsrNote elem = this;
      
        if (gBsrTraceOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrNote::visitEnd ()" <<
            endl;
        }
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
    case kNoteKindNone: result = "None"; break;
      
    case kNoteKindRestBreve: result = "RestBreve"; break;
    case kNoteKindRestWhole: result = "RestWhole"; break;
    case kNoteKindRestHalf: result = "RestHalf"; break;
    case kNoteKindRestQuarter: result = "RestQuarter"; break;
    case kNoteKindRest8th: result = "Rest8th"; break;
    case kNoteKindRest16th: result = "Rest16th"; break;
    case kNoteKindRest32nd: result = "Rest32nd"; break;
    case kNoteKindRest64th: result = "Rest64th"; break;
    case kNoteKindRest128th: result = "Rest128th"; break;
    case kNoteKindRest256th: result = "Rest256th"; break;

    case kNoteKindCBreve: result = "CBreve"; break;
    case kNoteKindCWhole: result = "CWhole"; break;
    case kNoteKindCHalf: result = "CHalf"; break;
    case kNoteKindCQuarter: result = "CQuarter"; break;
    case kNoteKindC8th: result = "C8th"; break;
    case kNoteKindC16th: result = "C16th"; break;
    case kNoteKindC32nd: result = "C32nd"; break;
    case kNoteKindC64th: result = "C64th"; break;
    case kNoteKindC128th: result = "C128th"; break;
    case kNoteKindC256th: result = "C256th"; break;

    case kNoteKindDBreve: result = "DBreve"; break;
    case kNoteKindDWhole: result = "DWhole"; break;
    case kNoteKindDHalf: result = "DHalf"; break;
    case kNoteKindDQuarter: result = "DQuarter"; break;
    case kNoteKindD8th: result = "D8th"; break;
    case kNoteKindD16th: result = "D16th"; break;
    case kNoteKindD32nd: result = "D32nd"; break;
    case kNoteKindD64th: result = "D64th"; break;
    case kNoteKindD128th: result = "D128th"; break;
    case kNoteKindD256th: result = "D256th"; break;

    case kNoteKindEBreve: result = "EBreve"; break;
    case kNoteKindEWhole: result = "EWhole"; break;
    case kNoteKindEHalf: result = "EHalf"; break;
    case kNoteKindEQuarter: result = "EQuarter"; break;
    case kNoteKindE8th: result = "E8th"; break;
    case kNoteKindE16th: result = "E16th"; break;
    case kNoteKindE32nd: result = "E32nd"; break;
    case kNoteKindE64th: result = "E64th"; break;
    case kNoteKindE128th: result = "E128th"; break;
    case kNoteKindE256th: result = "E256th"; break;

    case kNoteKindFBreve: result = "FBreve"; break;
    case kNoteKindFWhole: result = "FWhole"; break;
    case kNoteKindFHalf: result = "FHalf"; break;
    case kNoteKindFQuarter: result = "FQuarter"; break;
    case kNoteKindF8th: result = "F8th"; break;
    case kNoteKindF16th: result = "F16th"; break;
    case kNoteKindF32nd: result = "F32nd"; break;
    case kNoteKindF64th: result = "F64th"; break;
    case kNoteKindF128th: result = "F128th"; break;
    case kNoteKindF256th: result = "F256th"; break;

    case kNoteKindGBreve: result = "GBreve"; break;
    case kNoteKindGWhole: result = "GWhole"; break;
    case kNoteKindGHalf: result = "GHalf"; break;
    case kNoteKindGQuarter: result = "GQuarter"; break;
    case kNoteKindG8th: result = "G8th"; break;
    case kNoteKindG16th: result = "G16th"; break;
    case kNoteKindG32nd: result = "G32nd"; break;
    case kNoteKindG64th: result = "G64th"; break;
    case kNoteKindG128th: result = "G128th"; break;
    case kNoteKindG256th: result = "G256th"; break;

    case kNoteKindABreve: result = "ABreve"; break;
    case kNoteKindAWhole: result = "AWhole"; break;
    case kNoteKindAHalf: result = "AHalf"; break;
    case kNoteKindAQuarter: result = "AQuarter"; break;
    case kNoteKindA8th: result = "A8th"; break;
    case kNoteKindA16th: result = "A16th"; break;
    case kNoteKindA32nd: result = "A32nd"; break;
    case kNoteKindA64th: result = "A64th"; break;
    case kNoteKindA128th: result = "A128th"; break;
    case kNoteKindA256th: result = "A256th"; break;

    case kNoteKindBBreve: result = "BBreve"; break;
    case kNoteKindBWhole: result = "BWhole"; break;
    case kNoteKindBHalf: result = "BHalf"; break;
    case kNoteKindBQuarter: result = "BQuarter"; break;
    case kNoteKindB8th: result = "B8th"; break;
    case kNoteKindB16th: result = "B16th"; break;
    case kNoteKindB32nd: result = "B32nd"; break;
    case kNoteKindB64th: result = "B64th"; break;
    case kNoteKindB128th: result = "B128th"; break;
    case kNoteKindB256th: result = "B256th"; break;
  } // switch

  return result;
}

string bsrNote::noteOctaveKindAsString (
  bsrNoteOctaveKind noteOctaveKind)
{
  string result;

  switch (noteOctaveKind) {
    case kNoteOctaveNone:   result = "OctaveNone"; break;
    case kNoteOctaveBelow1: result = "OctaveBelow1"; break;
    case kNoteOctave1:      result = "Octave1"; break;
    case kNoteOctave2:      result = "Octave2"; break;
    case kNoteOctave3:      result = "Octave3"; break;
    case kNoteOctave4:      result = "Octave4"; break;
    case kNoteOctave5:      result = "Octave5"; break;
    case kNoteOctave6:      result = "Octave6"; break;
    case kNoteOctave7:      result = "Octave7"; break;
    case kNoteOctaveAbove7: result = "OctaveAbove7"; break;
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
    ", noteOctaveIsNeeded: " <<
    noteOctaveIsNeededAsString (fNoteOctaveIsNeeded) <<
    ", noteCellsList: " <<
    fNoteCellsList->asShortString () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrNote::print (ostream& os)
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
    "noteOctaveIsNeeded" << " : " <<
    noteOctaveIsNeededAsString (fNoteOctaveIsNeeded) <<
    endl <<
    setw (fieldWidth) <<
    "noteCellsList" << " : " <<
    fNoteCellsList->asShortString () <<
    endl;
        
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrNote& elt)
{
  elt->print (os);
  return os;
}


}
