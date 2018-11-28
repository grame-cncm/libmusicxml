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
#endif

#include "bsrOptions.h"
#include "xml2brlOptionsHandling.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrNote bsrNote::create (
  int               inputLineNumber,
  bsrNoteValueKind  noteValueKind,
  int               noteDotsNumber,
  bsrNoteOctaveKind noteOctaveKind,
  bsrNoteOctaveIsNeededKind
                    noteOctaveIsNeededKind)
{
  bsrNote* o =
    new bsrNote (
      inputLineNumber,
      noteValueKind,
      noteDotsNumber,
      noteOctaveKind,
      noteOctaveIsNeededKind);
  assert (o!=0);
  return o;
}

bsrNote::bsrNote (
  int               inputLineNumber,
  bsrNoteValueKind  noteValueKind,
  int               noteDotsNumber,
  bsrNoteOctaveKind noteOctaveKind,
  bsrNoteOctaveIsNeededKind
                    noteOctaveIsNeededKind)
    : bsrBrailleElement (inputLineNumber)
{
  fNoteValueKind  = noteValueKind;
  fNoteDotsNumber = noteDotsNumber;
  
  fNoteOctaveKind = noteOctaveKind;
    
  fNoteOctaveIsNeededKind = noteOctaveIsNeededKind;

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
    case kNoteRestBreveKind:
      result->appendCellKindToCellsList (kDots134);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteRestWholeKind:
      result->appendCellKindToCellsList (kDots134);
      break;
    case kNoteRestHalfKind:
      result->appendCellKindToCellsList (kDots136);
      break;
    case kNoteRestQuarterKind:
      result->appendCellKindToCellsList (kDots1236);
      break;
    case kNoteRest8thKind:
      result->appendCellKindToCellsList (kDots1346);
      break;
    case kNoteRest16thKind:
      result->appendCellKindToCellsList (kDots134);
      break;
    case kNoteRest32ndKind:
      result->appendCellKindToCellsList (kDots136);
      break;
    case kNoteRest64thKind:
      result->appendCellKindToCellsList (kDots1236);
      break;
    case kNoteRest128thKind:
      result->appendCellKindToCellsList (kDots1346);
      break;
    case kNoteRest256thKind:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots134);
      break;

    case kNoteCBreveKind:
      result->appendCellKindToCellsList (kDots13456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteCWholeKind:
      result->appendCellKindToCellsList (kDots13456);
      break;
    case kNoteCHalfKind:
      result->appendCellKindToCellsList (kDots1345);
      break;
    case kNoteCQuarterKind:
      result->appendCellKindToCellsList (kDots1456);
      break;
    case kNoteC8thKind:
      result->appendCellKindToCellsList (kDots145);
      break;
    case kNoteC16thKind:
      result->appendCellKindToCellsList (kDots13456);
      break;
    case kNoteC32ndKind:
      result->appendCellKindToCellsList (kDots1345);
      break;
    case kNoteC64thKind:
      result->appendCellKindToCellsList (kDots1456);
      break;
    case kNoteC128thKind:
      result->appendCellKindToCellsList (kDots145);
      break;
    case kNoteC256thKind:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots13456);
      break;

    case kNoteDBreveKind:
      result->appendCellKindToCellsList (kDots1356);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteDWholeKind:
      result->appendCellKindToCellsList (kDots1356);
      break;
    case kNoteDHalfKind:
      result->appendCellKindToCellsList (kDots135);
      break;
    case kNoteDQuarterKind:
      result->appendCellKindToCellsList (kDots156);
      break;
    case kNoteD8thKind:
      result->appendCellKindToCellsList (kDots15);
      break;
    case kNoteD16thKind:
      result->appendCellKindToCellsList (kDots1356);
      break;
    case kNoteD32ndKind:
      result->appendCellKindToCellsList (kDots135);
      break;
    case kNoteD64thKind:
      result->appendCellKindToCellsList (kDots156);
      break;
    case kNoteD128thKind:
      result->appendCellKindToCellsList (kDots15);
      break;
    case kNoteD256thKind:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots1356);
      break;

    case kNoteEBreveKind:
      result->appendCellKindToCellsList (kDots12346);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteEWholeKind:
      result->appendCellKindToCellsList (kDots12346);
      break;
    case kNoteEHalfKind:
      result->appendCellKindToCellsList (kDots1234);
      break;
    case kNoteEQuarterKind:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteE8thKind:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteE16thKind:
      result->appendCellKindToCellsList (kDots12346);
      break;
    case kNoteE32ndKind:
      result->appendCellKindToCellsList (kDots1234);
      break;
    case kNoteE64thKind:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteE128thKind:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteE256thKind:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots12346);
      break;

    case kNoteFBreveKind:
      result->appendCellKindToCellsList (kDots123456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteFWholeKind:
      result->appendCellKindToCellsList (kDots123456);
      break;
    case kNoteFHalfKind:
      result->appendCellKindToCellsList (kDots12345);
      break;
    case kNoteFQuarterKind:
      result->appendCellKindToCellsList (kDots12456);
      break;
    case kNoteF8thKind:
      result->appendCellKindToCellsList (kDots1245);
      break;
    case kNoteF16thKind:
      result->appendCellKindToCellsList (kDots123456);
      break;
    case kNoteF32ndKind:
      result->appendCellKindToCellsList (kDots12345);
      break;
    case kNoteF64thKind:
      result->appendCellKindToCellsList (kDots12456);
      break;
    case kNoteF128thKind:
      result->appendCellKindToCellsList (kDots1245);
      break;
    case kNoteF256thKind:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots123456);
      break;

    case kNoteGBreveKind:
      result->appendCellKindToCellsList (kDots12356);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteGWholeKind:
      result->appendCellKindToCellsList (kDots12356);
      break;
    case kNoteGHalfKind:
      result->appendCellKindToCellsList (kDots1235);
      break;
    case kNoteGQuarterKind:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteG8thKind:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteG16thKind:
      result->appendCellKindToCellsList (kDots12356);
      break;
    case kNoteG32ndKind:
      result->appendCellKindToCellsList (kDots1235);
      break;
    case kNoteG64thKind:
      result->appendCellKindToCellsList (kDots1256);
      break;
    case kNoteG128thKind:
      result->appendCellKindToCellsList (kDots125);
      break;
    case kNoteG256thKind:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots12356);
      break;

    case kNoteABreveKind:
      result->appendCellKindToCellsList (kDots2346);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteAWholeKind:
      result->appendCellKindToCellsList (kDots2346);
      break;
    case kNoteAHalfKind:
      result->appendCellKindToCellsList (kDots234);
      break;
    case kNoteAQuarterKind:
      result->appendCellKindToCellsList (kDots246);
      break;
    case kNoteA8thKind:
      result->appendCellKindToCellsList (kDots24);
      break;
    case kNoteA16thKind:
      result->appendCellKindToCellsList (kDots2346);
      break;
    case kNoteA32ndKind:
      result->appendCellKindToCellsList (kDots234);
      break;
    case kNoteA64thKind:
      result->appendCellKindToCellsList (kDots246);
      break;
    case kNoteA128thKind:
      result->appendCellKindToCellsList (kDots24);
      break;
    case kNoteA256thKind:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots2346);
      break;

    case kNoteBBreveKind:
      result->appendCellKindToCellsList (kDots23456);
      result->appendCellKindToCellsList (kDots13);
      break;
    case kNoteBWholeKind:
      result->appendCellKindToCellsList (kDots23456);
      break;
    case kNoteBHalfKind:
      result->appendCellKindToCellsList (kDots2345);
      break;
    case kNoteBQuarterKind:
      result->appendCellKindToCellsList (kDots2456);
      break;
    case kNoteB8thKind:
      result->appendCellKindToCellsList (kDots245);
      break;
    case kNoteB16thKind:
      result->appendCellKindToCellsList (kDots23456);
      break;
    case kNoteB32ndKind:
      result->appendCellKindToCellsList (kDots2345);
      break;
    case kNoteB64thKind:
      result->appendCellKindToCellsList (kDots2456);
      break;
    case kNoteB128thKind:
      result->appendCellKindToCellsList (kDots245);
      break;
    case kNoteB256thKind:
      result->appendCellKindToCellsList (kDots56);
      result->appendCellKindToCellsList (kDots126);
      result->appendCellKindToCellsList (kDots2);
      result->appendCellKindToCellsList (kDots23456);
      break;
  } // switch

  return result;
}

S_bsrCellsList bsrNote::noteValueKindAsCellsList ()
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
    case kNoteOctaveBelow1Kind:
      result->appendCellKindToCellsList (kDots4);
      result->appendCellKindToCellsList (kDots4);
      break;
    case kNoteOctave1Kind:
      result->appendCellKindToCellsList (kDots4);
      break;
    case kNoteOctave2Kind:
      result->appendCellKindToCellsList (kDots45);
      break;
    case kNoteOctave3Kind:
      result->appendCellKindToCellsList (kDots456);
      break;
    case kNoteOctave4Kind:
      result->appendCellKindToCellsList (kDots5);
      break;
    case kNoteOctave5Kind:
      result->appendCellKindToCellsList (kDots46);
      break;
    case kNoteOctave6Kind:
      result->appendCellKindToCellsList (kDots56);
      break;
    case kNoteOctave7Kind:
      result->appendCellKindToCellsList (kDots6);
      break;
    case kNoteOctaveAbove7Kind:
      result->appendCellKindToCellsList (kDots6);
      result->appendCellKindToCellsList (kDots6);
      break;
  } // switch
 
  return result;
}

S_bsrCellsList bsrNote::noteOctaveKindAsCellsList ()
{
  return
    noteOctaveKindAsCellsList (
      fInputLineNumber,
      fNoteOctaveKind);
}

S_bsrCellsList bsrNote::asCellsList ()
{
  S_bsrCellsList
    result =
      bsrCellsList::create (fInputLineNumber);

  // append note octave if needed
  switch (fNoteOctaveIsNeededKind) {
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

void bsrNote::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrNote::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrNote>*
    p =
      dynamic_cast<visitor<S_bsrNote>*> (v)) {
        S_bsrNote elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrNote::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrNote::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrNote::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrNote>*
    p =
      dynamic_cast<visitor<S_bsrNote>*> (v)) {
        S_bsrNote elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
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
    case kNoteRestBreveKind: result = "RestBreve"; break;
    case kNoteRestWholeKind: result = "RestWhole"; break;
    case kNoteRestHalfKind: result = "RestHalf"; break;
    case kNoteRestQuarterKind: result = "RestQuarter"; break;
    case kNoteRest8thKind: result = "Rest8th"; break;
    case kNoteRest16thKind: result = "Rest16th"; break;
    case kNoteRest32ndKind: result = "Rest32nd"; break;
    case kNoteRest64thKind: result = "Rest64th"; break;
    case kNoteRest128thKind: result = "Rest128th"; break;
    case kNoteRest256thKind: result = "Rest256th"; break;

    case kNoteCBreveKind: result = "CBreve"; break;
    case kNoteCWholeKind: result = "CWhole"; break;
    case kNoteCHalfKind: result = "CHalf"; break;
    case kNoteCQuarterKind: result = "CQuarter"; break;
    case kNoteC8thKind: result = "C8th"; break;
    case kNoteC16thKind: result = "C16th"; break;
    case kNoteC32ndKind: result = "C32nd"; break;
    case kNoteC64thKind: result = "C64th"; break;
    case kNoteC128thKind: result = "C128th"; break;
    case kNoteC256thKind: result = "C256th"; break;

    case kNoteDBreveKind: result = "DBreve"; break;
    case kNoteDWholeKind: result = "DWhole"; break;
    case kNoteDHalfKind: result = "DHalf"; break;
    case kNoteDQuarterKind: result = "DQuarter"; break;
    case kNoteD8thKind: result = "D8th"; break;
    case kNoteD16thKind: result = "D16th"; break;
    case kNoteD32ndKind: result = "D32nd"; break;
    case kNoteD64thKind: result = "D64th"; break;
    case kNoteD128thKind: result = "D128th"; break;
    case kNoteD256thKind: result = "D256th"; break;

    case kNoteEBreveKind: result = "EBreve"; break;
    case kNoteEWholeKind: result = "EWhole"; break;
    case kNoteEHalfKind: result = "EHalf"; break;
    case kNoteEQuarterKind: result = "EQuarter"; break;
    case kNoteE8thKind: result = "E8th"; break;
    case kNoteE16thKind: result = "E16th"; break;
    case kNoteE32ndKind: result = "E32nd"; break;
    case kNoteE64thKind: result = "E64th"; break;
    case kNoteE128thKind: result = "E128th"; break;
    case kNoteE256thKind: result = "E256th"; break;

    case kNoteFBreveKind: result = "FBreve"; break;
    case kNoteFWholeKind: result = "FWhole"; break;
    case kNoteFHalfKind: result = "FHalf"; break;
    case kNoteFQuarterKind: result = "FQuarter"; break;
    case kNoteF8thKind: result = "F8th"; break;
    case kNoteF16thKind: result = "F16th"; break;
    case kNoteF32ndKind: result = "F32nd"; break;
    case kNoteF64thKind: result = "F64th"; break;
    case kNoteF128thKind: result = "F128th"; break;
    case kNoteF256thKind: result = "F256th"; break;

    case kNoteGBreveKind: result = "GBreve"; break;
    case kNoteGWholeKind: result = "GWhole"; break;
    case kNoteGHalfKind: result = "GHalf"; break;
    case kNoteGQuarterKind: result = "GQuarter"; break;
    case kNoteG8thKind: result = "G8th"; break;
    case kNoteG16thKind: result = "G16th"; break;
    case kNoteG32ndKind: result = "G32nd"; break;
    case kNoteG64thKind: result = "G64th"; break;
    case kNoteG128thKind: result = "G128th"; break;
    case kNoteG256thKind: result = "G256th"; break;

    case kNoteABreveKind: result = "ABreve"; break;
    case kNoteAWholeKind: result = "AWhole"; break;
    case kNoteAHalfKind: result = "AHalf"; break;
    case kNoteAQuarterKind: result = "AQuarter"; break;
    case kNoteA8thKind: result = "A8th"; break;
    case kNoteA16thKind: result = "A16th"; break;
    case kNoteA32ndKind: result = "A32nd"; break;
    case kNoteA64thKind: result = "A64th"; break;
    case kNoteA128thKind: result = "A128th"; break;
    case kNoteA256thKind: result = "A256th"; break;

    case kNoteBBreveKind: result = "BBreve"; break;
    case kNoteBWholeKind: result = "BWhole"; break;
    case kNoteBHalfKind: result = "BHalf"; break;
    case kNoteBQuarterKind: result = "BQuarter"; break;
    case kNoteB8thKind: result = "B8th"; break;
    case kNoteB16thKind: result = "B16th"; break;
    case kNoteB32ndKind: result = "B32nd"; break;
    case kNoteB64thKind: result = "B64th"; break;
    case kNoteB128thKind: result = "B128th"; break;
    case kNoteB256thKind: result = "B256th"; break;
  } // switch

  return result;
}

string bsrNote::noteOctaveKindAsString (
  bsrNoteOctaveKind noteOctaveKind)
{
  string result;

  switch (noteOctaveKind) {
    case kNoteOctaveBelow1Kind: result = "OctaveBelow1"; break;
    case kNoteOctave1Kind:      result = "Octave1"; break;
    case kNoteOctave2Kind:      result = "Octave2"; break;
    case kNoteOctave3Kind:      result = "Octave3"; break;
    case kNoteOctave4Kind:      result = "Octave4"; break;
    case kNoteOctave5Kind:      result = "Octave5"; break;
    case kNoteOctave6Kind:      result = "Octave6"; break;
    case kNoteOctave7Kind:      result = "Octave7"; break;
    case kNoteOctaveAbove7Kind: result = "OctaveAbove7"; break;
  } // switch

  return result;
}

string bsrNote::noteOctaveIsNeededKindAsString (
  bsrNoteOctaveIsNeededKind noteOctaveIsNeededKind)
{
  string result;
  
  switch (noteOctaveIsNeededKind) {
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
    ", noteOctaveIsNeededKind: " <<
    noteOctaveIsNeededKindAsString (fNoteOctaveIsNeededKind) <<
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
    "noteOctaveIsNeededKind" << " : " <<
    noteOctaveIsNeededKindAsString (fNoteOctaveIsNeededKind) <<
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
