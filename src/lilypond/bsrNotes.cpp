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

namespace MusicXML2 {

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

wchar_t bsrNote::noteValueKindAsBraille (
  bsrNoteValueKind noteValueKind)
{
  wchar_t result;
  
  switch (noteValueKind) {
    case kBsrCEighthKind: result = kBrlCEighth; break;
    case kBsrCQuarterKind: result = kBrlCQuarter; break;
    case kBsrCHalfKind: result = kBrlCHalf; break;
    case kBsrCWholeKind: result = kBrlCWhole; break;
    
    case kBsrDEighthKind: result = kBrlDEighth; break;
    case kBsrDQuarterKind: result = kBrlDQuarter; break;
    case kBrlDHalfKind: result = kBrlDHalf; break;
    case kBsrDWholeKind: result = kBrlDWhole; break;
    
    case kBsrEEighthKind: result = kBrlEEighth; break;
    case kBsrEQuarterKind: result = kBrlEQuarter; break;
    case kBsrEHalf4Kind: result = kBrlEHalf; break;
    case kBsrEWholeKind: result = kBrlEWhole; break;
    
    case kBsrFEighthKind: result = kBrlFEighth; break;
    case kBsrFQuarterKind: result = kBrlFQuarter; break;
    case kBsrFHalfKind: result = kBrlFHalf; break;
    case kBsrFWholeKind: result = kBrlFWhole; break;
    
    case kBsrGEighthKind: result = kBrlGEighth; break;
    case kBsrGQuarterKind: result = kBrlGQuarter; break;
    case kBsrGHalfKind: result = kBrlGHalf; break;
    case kBsrGWholeKind: result = kBrlGWhole; break;
    
    case kBsrAEighthKind: result = kBrlAEighth; break;
    case kBsrAQuarterKind: result = kBrlAQuarter; break;
    case kBsrAHalfKind: result = kBrlAHalf; break;
    case kBsrAWholeKind: result = kBrlAWhole; break;
    
    case kBsrBEighthKind: result = kBrlBEighth; break;
    case kBsrBQuarterKind: result = kBrlBQuarter; break;
    case kBsrBHalfKind: result = kBrlBHalf; break;
    case kBsrBWholeKind: result = kBrlBWhole; break;
  } // switch

  return result;
}

string bsrNote::noteValueKindAsString (
  bsrNoteValueKind noteValueKind)
{
  string result;
  
  switch (noteValueKind) {
    case kBsrCEighthKind: result = "CEighth"; break; // JMI
    case kBsrCQuarterKind: result = "CQuarter"; break;
    case kBsrCHalfKind: result = "CHalf"; break;
    case kBsrCWholeKind: result = "CWhole"; break;
    
    case kBsrDEighthKind: result = "DEighth"; break;
    case kBsrDQuarterKind: result = "DQuarter"; break;
    case kBrlDHalfKind: result = "DHalf"; break;
    case kBsrDWholeKind: result = "DWhole"; break;
    
    case kBsrEEighthKind: result = "EEighth"; break;
    case kBsrEQuarterKind: result = "EQuarter"; break;
    case kBsrEHalf4Kind: result = "EHalf"; break;
    case kBsrEWholeKind: result = "EWhole"; break;
    
    case kBsrFEighthKind: result = "FEighth"; break;
    case kBsrFQuarterKind: result = "FQuarter"; break;
    case kBsrFHalfKind: result = "FHalf"; break;
    case kBsrFWholeKind: result = "FWhole"; break;
    
    case kBsrGEighthKind: result = "GEighth"; break;
    case kBsrGQuarterKind: result = "GQuarter"; break;
    case kBsrGHalfKind: result = "GHalfbreak"; break;
    case kBsrGWholeKind: result = "GWhole"; break;
    
    case kBsrAEighthKind: result = "AEighth"; break;
    case kBsrAQuarterKind: result = "AQuarter"; break;
    case kBsrAHalfKind: result = "AHalf"; break;
    case kBsrAWholeKind: result = "AWhole"; break;
    
    case kBsrBEighthKind: result = "BEighth"; break;
    case kBsrBQuarterKind: result = "BQuarter"; break;
    case kBsrBHalfKind: result = "BHalf"; break;
    case kBsrBWholeKind: result = "BWhole"; break;
  } // switch

  return result;
}

wchar_t bsrNote::noteOctaveKindAsBraille ( // JMI
  bsrNoteOctaveKind noteOctaveKind)
{
  wchar_t result;
  
  switch (noteOctaveKind) {
 //   case kBrlOctaveBelow1Kind: result = kBrlOctaveBelow1; break; // twice JMI
    case kBrlOctave1Kind:      result = kBrlOctave1; break;
    case kBrlOctave2Kind:      result = kBrlOctave2; break;
    case kBrlOctave3Kind:      result = kBrlOctave3; break;
    case kBrlOctave4Kind:      result = kBrlOctave4; break;
    case kBrlOctave5Kind:      result = kBrlOctave5; break;
    case kBrlOctave6Kind:      result = kBrlOctave6; break;
    case kBrlOctave7Kind:      result = kBrlOctave7; break;
 //   case kBrlOctaveAbove7Kind: result = kBrlOctaveAbove7; break; // twice JMI
  } // switch

  return result;
}

string bsrNote::noteOctaveKindAsString (
  bsrNoteOctaveKind noteOctaveKind)
{
  string result;

  switch (noteOctaveKind) {
    case kBrlOctaveBelow1Kind: result = "OctaveBelow1"; break;
    case kBrlOctave1Kind:      result = "Octave1"; break;
    case kBrlOctave2Kind:      result = "Octave2"; break;
    case kBrlOctave3Kind:      result = "Octave3"; break;
    case kBrlOctave4Kind:      result = "Octave4"; break;
    case kBrlOctave5Kind:      result = "Octave5"; break;
    case kBrlOctave6Kind:      result = "Octave6"; break;
    case kBrlOctave7Kind:      result = "Octave7"; break;
    case kBrlOctaveAbove7Kind: result = "OctaveAbove7"; break;
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
