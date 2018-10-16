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
#include <iomanip>      // setw, setprecision, ...

#include "msrTranspositions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrOctaveShift msrOctaveShift::create (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
{
  msrOctaveShift* o =
    new msrOctaveShift (
      inputLineNumber, octaveShiftKind, octaveShiftSize);
  assert(o!=0);
  return o;
}

msrOctaveShift::msrOctaveShift (
  int                inputLineNumber,
  msrOctaveShiftKind octaveShiftKind,
  int                octaveShiftSize)
    : msrElement (inputLineNumber)
{
  fOctaveShiftKind = octaveShiftKind;

  fOctaveShiftSize = octaveShiftSize;
}

msrOctaveShift::~msrOctaveShift ()
{}

void msrOctaveShift::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOctaveShift::acceptIn ()" <<
      endl;
  }
  
  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOctaveShift::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrOctaveShift::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrOctaveShift::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrOctaveShift>*
    p =
      dynamic_cast<visitor<S_msrOctaveShift>*> (v)) {
        S_msrOctaveShift elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrOctaveShift::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrOctaveShift::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrOctaveShift& elt)
{
  elt->print (os);
  return os;
}

string msrOctaveShift::octaveShiftKindAsString () const
{
  string result;
  
  switch (fOctaveShiftKind) {
    case msrOctaveShift::kOctaveShiftNone:
      result = "octaveShiftNone";
      break;
    case msrOctaveShift::kOctaveShiftUp:
      result = "octaveShiftUp";
      break;
    case msrOctaveShift::kOctaveShiftDown:
      result = "octaveShiftDown";
      break;
    case msrOctaveShift::kOctaveShiftStop:
      result = "octaveShiftStop";
      break;
    case msrOctaveShift::kOctaveShiftContinue:
      result = "octaveShiftContinue";
      break;
  } // switch

  return result;
}

void msrOctaveShift::print (ostream& os)
{
  gIndenter++;
  
  os <<
    "OctaveShift" <<
    ", kind: " << octaveShiftKindAsString () <<
    ", size: " << fOctaveShiftSize <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrTranspose msrTranspose::create (
  int  inputLineNumber,
  int  transposeDiatonic,
  int  transposeChromatic,
  int  transposeOctaveChange,
  bool transposeDouble)
{
  msrTranspose* o =
    new msrTranspose (
      inputLineNumber,
      transposeDiatonic,
      transposeChromatic,
      transposeOctaveChange,
      transposeDouble);
  assert (o!=0);
  return o;
}

msrTranspose::msrTranspose (
  int  inputLineNumber,
  int  transposeDiatonic,
  int  transposeChromatic,
  int  transposeOctaveChange,
  bool transposeDouble)
    : msrElement (inputLineNumber)
{
  fTransposeDiatonic     = transposeDiatonic;
  fTransposeChromatic    = transposeChromatic;
  fTransposeOctaveChange = transposeOctaveChange;
  fTransposeDouble       = transposeDouble;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTranspositions) {
    gLogIOstream <<
      "Creating transpose '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif
}

msrTranspose::~msrTranspose ()
{}

bool msrTranspose::isEqualTo (S_msrTranspose otherTranspose) const
{
  if (! otherTranspose) {
    return false;
  }
    
  return
    fTransposeDiatonic ==
      otherTranspose->fTransposeDiatonic
      &&
    fTransposeChromatic ==
      otherTranspose->fTransposeChromatic
      &&
    fTransposeOctaveChange ==
      otherTranspose->fTransposeOctaveChange
      &&
    fTransposeDouble ==
      otherTranspose->fTransposeDouble;
}

void msrTranspose::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTranspose::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTranspose::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTranspose::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrTranspose::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTranspose>*
    p =
      dynamic_cast<visitor<S_msrTranspose>*> (v)) {
        S_msrTranspose elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrTranspose::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTranspose::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrTranspose& elt)
{
  elt->print (os);
  return os;
}

string msrTranspose::asString () const
{
  stringstream s;

  s <<
    "Transpose" << 
    ", diatonic = " << fTransposeDiatonic <<
    ", chromatic = " << fTransposeChromatic <<
    ", transposeOctaveChange = " << fTransposeOctaveChange <<
    ", transposeDouble = " << fTransposeDouble <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrTranspose::print (ostream& os)
{
  const int fieldWidth = 22;
  
  os <<
    "Transpose" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  os << left <<
    setw (fieldWidth) <<
    "Diatonic" << " = " << fTransposeDiatonic <<
    endl <<
    setw (fieldWidth) <<
    "Chromatic" << " = " << fTransposeChromatic <<
    endl <<
    setw (fieldWidth) <<
    "TransposeOctaveChange" << " = " << fTransposeOctaveChange <<
    endl <<
    setw (fieldWidth) <<
    "TransposeDouble" << " = " << fTransposeDouble <<
    endl <<
    endl;
    
  gIndenter--;
}


}
