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

#include "msrStavesDetails.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrStaffTuning msrStaffTuning::create (
  int                     inputLineNumber,
  int                     staffTuningLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  int                     staffTuningOctave)
{
  msrStaffTuning* o =
    new msrStaffTuning (
      inputLineNumber,
      staffTuningLineNumber,
      quarterTonesPitchKind,
      staffTuningOctave);
  assert(o!=0);
  return o;
}

msrStaffTuning::msrStaffTuning (
  int                      inputLineNumber,
  int                      staffTuningLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  int                      staffTuningOctave)
    : msrElement (inputLineNumber)
{
  fStaffTuningLineNumber            = staffTuningLineNumber;
  fStaffTuningQuarterTonesPitchKind = quarterTonesPitchKind;
  fStaffTuningOctave                = staffTuningOctave;
}

msrStaffTuning::~ msrStaffTuning ()
{}

S_msrStaffTuning msrStaffTuning::createStaffTuningNewbornClone ()
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceStaffTuning) {
    gLogIOstream <<
      "Creating a newborn clone of staff tuning '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

 S_msrStaffTuning
    newbornClone =
      msrStaffTuning::create (
        fInputLineNumber,
        fStaffTuningLineNumber,
        fStaffTuningQuarterTonesPitchKind,
        fStaffTuningOctave);
  
  return newbornClone;
}

void msrStaffTuning::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaffTuning::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaffTuning::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffTuning::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaffTuning::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaffTuning::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffTuning::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrStaffTuning& elt)
{
  elt->print (os);
  return os;
}

string msrStaffTuning::asString () const
{
  stringstream s;

  s <<
    "StaffTuning" <<
    ", line " << fStaffTuningLineNumber <<
    ", " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fStaffTuningQuarterTonesPitchKind) <<
    ", octave " << fStaffTuningOctave;
    
  return s.str ();
}

void msrStaffTuning::print (ostream& os)
{
  os <<
    "StaffTuning" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 29;

  os << left <<  
    setw (fieldWidth) <<
    "staffTuningLineNumber" << " : " <<
    fStaffTuningLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "staffTuningQuarterTonesPitch" << " : " <<
    msrQuarterTonesPitchKindAsString (
      gMsrOptions->fMsrQuarterTonesPitchesLanguageKind,
      fStaffTuningQuarterTonesPitchKind) <<
    endl <<
    setw (fieldWidth) <<
    "staffTuningOctave" << " : " <<
    fStaffTuningOctave <<
    endl;

  gIndenter--;
}

//______________________________________________________________________________
S_msrStaffDetails msrStaffDetails::create (
  int                 inputLineNumber,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
{
  msrStaffDetails* o =
    new msrStaffDetails (
      inputLineNumber,
      staffTypeKind,
      showFretsKind,
      printObjectKind,
      printSpacingKind);
  assert(o!=0);
  return o;
}

msrStaffDetails::msrStaffDetails (
  int                 inputLineNumber,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
    : msrElement (inputLineNumber)
{
  fStaffTypeKind    = staffTypeKind;
  
  fStaffLinesNumber = 5; // default value JMI ???

  fShowFretsKind    = showFretsKind;
  
  fPrintObjectKind  = printObjectKind;
  fPrintSpacingKind = printSpacingKind;
}

msrStaffDetails::~msrStaffDetails ()
{}

void msrStaffDetails::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaffDetails::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaffDetails::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffDetails::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrStaffDetails::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrStaffDetails::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffDetails::browseData (basevisitor* v)
{
  if (fStaffTuningsList.size ()) {
    for (
      list<S_msrStaffTuning>::const_iterator i = fStaffTuningsList.begin ();
      i != fStaffTuningsList.end ();
      i++) {
        // browse the staff tuning
        msrBrowser<msrStaffTuning> browser (v);
        browser.browse (*(*i));
    } // for
  }
}

string msrStaffDetails::staffTypeKindAsString (
  msrStaffTypeKind staffTypeKind)
{
  string result;
  
  switch (staffTypeKind) {
    case msrStaffDetails::kRegularStaffType:
      result = "regular";
      break;
    case msrStaffDetails::kOssiaStaffType:
      result = "ossia";
      break;
    case msrStaffDetails::kCueStaffType:
      result = "cue";
      break;
    case msrStaffDetails::kEditorialStaffType:
      result = "editorial";
      break;
    case msrStaffDetails::kAlternateStaffType:
      result = "alternate";
      break;
  } // switch

  return result;
}

string msrStaffDetails::showFretsKindAsString (
  msrShowFretsKind showFretsKind)
{
  string result;
  
  switch (showFretsKind) {
    case msrStaffDetails::kShowFretsNumbers:
      result = "numbers";
      break;
    case msrStaffDetails::kShowFretsLetters:
      result = "letters";
      break;
  } // switch

  return result;
}
      
string msrStaffDetails::printObjectKindAsString (
  msrPrintObjectKind printObjectKind)
{
  string result;
  
  switch (printObjectKind) {
    case msrStaffDetails::kPrintObjectYes:
      result = "print object yes";
      break;
    case msrStaffDetails::kPrintObjectNo:
      result = "print object no";
      break;
  } // switch

  return result;
}
      
string msrStaffDetails::printSpacingKindAsString (
  msrPrintSpacingKind printSpacingKind)
{
  string result;
  
  switch (printSpacingKind) {
    case msrStaffDetails::kPrintSpacingYes:
      result = "print spacing yes";
      break;
    case msrStaffDetails::kPrintSpacingNo:
      result = "print spacing no";
      break;
  } // switch

  return result;
}
      
ostream& operator<< (ostream& os, const S_msrStaffDetails& elt)
{
  elt->print (os);
  return os;
}

string msrStaffDetails::asShortString ()
{
  stringstream s;

  s <<
    "StaffDetails" <<
    ", staffTypeKind" <<
    staffTypeKindAsString (fStaffTypeKind) <<
    ", line " << fInputLineNumber <<
    endl;

  // print the staff lines number
  s << ", staffLinesNumber: " << fStaffLinesNumber;

  // print the staff tunings if any
  s <<
    ", StaffTunings: " << fStaffTuningsList.size ();

  s <<
    ", showFretsKind = " <<
    showFretsKindAsString (fShowFretsKind) <<
    ", printObjectKind = " <<
    printObjectKindAsString (fPrintObjectKind) <<
    ", printSpacingKind = " <<
    printSpacingKindAsString (fPrintSpacingKind);

  return s.str ();
}

void msrStaffDetails::print (ostream& os)
{
  os <<
    "StaffDetails" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 17;
  
  os << left <<
    setw (fieldWidth) <<
    "staffTypeKind" << " : " <<
    staffTypeKindAsString (fStaffTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "staffLinesNumber" << " : " << fStaffLinesNumber <<
    endl;

  // print the staff tunings if any
  if (fStaffTuningsList.size ()) {
    os <<
      endl;
      
    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;
      
    gIndenter++;
    
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os <<
      endl;
    
    gIndenter--;
  }
  else {
    os << left <<
      setw (fieldWidth) <<
      "staffTunings" << " : " << "none" <<
      endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "showFretsKind" << " : " <<
    showFretsKindAsString (fShowFretsKind) <<
    endl <<
    setw (fieldWidth) <<
    "printObjectKind" << " : " <<
    printObjectKindAsString (fPrintObjectKind) <<
    endl <<
    setw (fieldWidth) <<
    "printSpacingKind" << " : " <<
    printSpacingKindAsString (fPrintSpacingKind) <<
    endl;

  gIndenter--;
}


}
