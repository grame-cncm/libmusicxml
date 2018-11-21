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
  bsrNoteOctaveKind noteOctaveKind,
  bsrNoteOctaveIsNeededKind
                    noteOctaveIsNeededKind)
{
  bsrNote* o =
    new bsrNote (
      inputLineNumber,
      noteValueKind,
      noteOctaveKind,
      noteOctaveIsNeededKind);
  assert (o!=0);
  return o;
}

bsrNote::bsrNote (
  int               inputLineNumber,
  bsrNoteValueKind  noteValueKind,
  bsrNoteOctaveKind noteOctaveKind,
  bsrNoteOctaveIsNeededKind
                    noteOctaveIsNeededKind)
    : bsrBrailleElement (inputLineNumber)
{
  fNoteValueKind  = noteValueKind;
  fNoteOctaveKind = noteOctaveKind;
    
  fNoteOctaveIsNeededKind = noteOctaveIsNeededKind;
}

bsrNote::~bsrNote ()
{}

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
    case kNoteCEighthKind: result = "CEighth"; break; // JMI
    case kNoteCQuarterKind: result = "CQuarter"; break;
    case kNoteCHalfKind: result = "CHalf"; break;
    case kNoteCWholeKind: result = "CWhole"; break;
    
    case kNoteDEighthKind: result = "DEighth"; break;
    case kNoteDQuarterKind: result = "DQuarter"; break;
    case kNoteDHalfKind: result = "DHalf"; break;
    case kNoteDWholeKind: result = "DWhole"; break;
    
    case kNoteEEighthKind: result = "EEighth"; break;
    case kNoteEQuarterKind: result = "EQuarter"; break;
    case kNoteEHalf4Kind: result = "EHalf"; break;
    case kNoteEWholeKind: result = "EWhole"; break;
    
    case kNoteFEighthKind: result = "FEighth"; break;
    case kNoteFQuarterKind: result = "FQuarter"; break;
    case kNoteFHalfKind: result = "FHalf"; break;
    case kNoteFWholeKind: result = "FWhole"; break;
    
    case kNoteGEighthKind: result = "GEighth"; break;
    case kNoteGQuarterKind: result = "GQuarter"; break;
    case kNoteGHalfKind: result = "GHalfbreak"; break;
    case kNoteGWholeKind: result = "GWhole"; break;
    
    case kNoteAEighthKind: result = "AEighth"; break;
    case kNoteAQuarterKind: result = "AQuarter"; break;
    case kNoteAHalfKind: result = "AHalf"; break;
    case kNoteAWholeKind: result = "AWhole"; break;
    
    case kNoteBEighthKind: result = "BEighth"; break;
    case kNoteBQuarterKind: result = "BQuarter"; break;
    case kNoteBHalfKind: result = "BHalf"; break;
    case kNoteBWholeKind: result = "BWhole"; break;
  } // switch

  return result;
}

bsrSixDotsKind bsrNote::noteValueKindAsSixDotsKind (
  bsrNoteValueKind noteValueKind)
{
  bsrSixDotsKind result;

  switch (noteValueKind) {
    case kNoteCEighthKind:  result = kDots145; break;
    case kNoteCQuarterKind: result = kDots1456; break;
    case kNoteCHalfKind:    result = kDots1345; break;
    case kNoteCWholeKind:   result = kDots13456; break;
    
    case kNoteDEighthKind:  result = kDots15; break;
    case kNoteDQuarterKind: result = kDots156; break;
    case kNoteDHalfKind:    result = kDots135; break;
    case kNoteDWholeKind:   result = kDots1356; break;
    
    case kNoteEEighthKind:  result = kDots125; break;
    case kNoteEQuarterKind: result = kDots1256; break;
    case kNoteEHalf4Kind:   result = kDots1234; break;
    case kNoteEWholeKind:   result = kDots12346; break;
    
    case kNoteFEighthKind:  result = kDots1245; break;
    case kNoteFQuarterKind: result = kDots12456; break;
    case kNoteFHalfKind:    result = kDots12345; break;
    case kNoteFWholeKind:   result = kDots123456; break;
    
    case kNoteGEighthKind:  result = kDots125; break;
    case kNoteGQuarterKind: result = kDots1256; break;
    case kNoteGHalfKind:    result = kDots1235; break;
    case kNoteGWholeKind:   result = kDots12356; break;
    
    case kNoteAEighthKind:  result = kDots24; break;
    case kNoteAQuarterKind: result = kDots246; break;
    case kNoteAHalfKind:    result = kDots234; break;
    case kNoteAWholeKind:   result = kDots2346; break;
    
    case kNoteBEighthKind:  result = kDots245; break;
    case kNoteBQuarterKind: result = kDots2456; break;
    case kNoteBHalfKind:    result = kDots2345; break;
    case kNoteBWholeKind:   result = kDots23456; break;
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

S_bsrBrailleSign bsrNote::asBrailleSign ()
{
  S_bsrBrailleSign result;


  return result;
}

bsrSixDotsKind bsrNote::noteOctaveKindAsSixDotsKind ( // JMI
  bsrNoteOctaveKind noteOctaveKind)
{
  bsrSixDotsKind result;
  
  switch (noteOctaveKind) {
    case kNoteOctaveBelow1Kind: result = kDots4; break; // twice JMI
    case kNoteOctave1Kind:      result = kDots4; break;
    case kNoteOctave2Kind:      result = kDots45; break;
    case kNoteOctave3Kind:      result = kDots456; break;
    case kNoteOctave4Kind:      result = kDots5; break;
    case kNoteOctave5Kind:      result = kDots46; break;
    case kNoteOctave6Kind:      result = kDots56; break;
    case kNoteOctave7Kind:      result = kDots6; break;
    case kNoteOctaveAbove7Kind: result = kDots6; break; // twice JMI
  } // switch
  /*
  kCellOctave1, // ,
  kCellOctave2, // ,
  kCellOctave3, // ,
  kCellOctave4, // ,
  kCellOctave5, // ,
  kCellOctave6, // ,
  kCellOctave7, // ;
  kCellOctaveBelow1 , //{ kCellOctave1, kCellOctave1 },
  kCellOctaveAbove7 , //{ kCellOctave7, kCellOctave7 };
  */

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
    ", noteOctaveKind: " <<
    noteOctaveKindAsString (fNoteOctaveKind) <<
    ", noteOctaveIsNeededKind: " <<
    noteOctaveIsNeededKindAsString (fNoteOctaveIsNeededKind) <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrNote::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrNote& elt)
{
  elt->print (os);
  return os;
}


}
