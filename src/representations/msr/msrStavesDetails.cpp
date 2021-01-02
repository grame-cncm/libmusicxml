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
#include <iomanip>      // setw, setprecision, ...

#include "utilities.h"

#include "msrStavesDetails.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrStaffTuning msrStaffTuning::create (
  int                      inputLineNumber,
  int                      staffTuningLineNumber,
  msrQuarterTonesPitchKind staffTuningQuarterTonesPitchKind,
  msrOctaveKind            staffTuningOctaveKind)
{
  msrStaffTuning* o =
    new msrStaffTuning (
      inputLineNumber,
      staffTuningLineNumber,
      staffTuningQuarterTonesPitchKind,
      staffTuningOctaveKind);
  assert (o!=0);
  return o;
}

msrStaffTuning::msrStaffTuning (
  int                      inputLineNumber,
  int                      staffTuningLineNumber,
  msrQuarterTonesPitchKind staffTuningQuarterTonesPitchKind,
  msrOctaveKind            staffTuningOctaveKind)
    : msrElement (inputLineNumber)
{
  fStaffTuningLineNumber            = staffTuningLineNumber;
  fStaffTuningQuarterTonesPitchKind = staffTuningQuarterTonesPitchKind;
  fStaffTuningOctaveKind            = staffTuningOctaveKind;
}

msrStaffTuning::~ msrStaffTuning ()
{}

S_msrStaffTuning msrStaffTuning::createStaffTuningNewbornClone ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceStaffDetails ()) {
    gLogStream <<
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
        fStaffTuningOctaveKind);

  return newbornClone;
}

void msrStaffTuning::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffTuning::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffTuning::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffTuning::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffTuning::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaffTuning>*
    p =
      dynamic_cast<visitor<S_msrStaffTuning>*> (v)) {
        S_msrStaffTuning elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffTuning::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffTuning::browseData (basevisitor* v)
{}

string msrStaffTuning::asString () const
{
  stringstream s;

  s <<
    "StaffTuning" <<
    ", line " << fStaffTuningLineNumber <<
    ", " <<
    quarterTonesPitchKindAsStringInLanguage (
      fStaffTuningQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    ", octave " <<
    msrOctaveKindAsString (fStaffTuningOctaveKind);

  return s.str ();
}

void msrStaffTuning::print (ostream& os) const
{
  os <<
    "StaffTuning" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 29;

  os << left <<
    setw (fieldWidth) <<
    "staffTuningLineNumber" << " : " <<
    fStaffTuningLineNumber <<
    endl <<
    setw (fieldWidth) <<
    "staffTuningQuarterTonesPitch" << " : " <<
    quarterTonesPitchKindAsStringInLanguage (
      fStaffTuningQuarterTonesPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ()) <<
    endl <<
    setw (fieldWidth) <<
    "staffTuningOctaveKind" << " : " <<
    msrOctaveKindAsString (fStaffTuningOctaveKind) <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrStaffTuning& elt)
{
  elt->print (os);
  return os;
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
  assert (o!=0);
  return o;
}

msrStaffDetails::msrStaffDetails (
  int                 inputLineNumber,
  msrStaffTypeKind    staffTypeKind,
  msrShowFretsKind    showFretsKind,
  msrPrintObjectKind  printObjectKind,
  msrPrintSpacingKind printSpacingKind)
    : msrMeasureElement (inputLineNumber)
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffDetails::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrStaffDetails::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffDetails::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrStaffDetails::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaffDetails>*
    p =
      dynamic_cast<visitor<S_msrStaffDetails>*> (v)) {
        S_msrStaffDetails elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
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
      ++i
  ) {
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
      result = "regularStaffType";
      break;
    case msrStaffDetails::kOssiaStaffType:
      result = "ossiaStaffType";
      break;
    case msrStaffDetails::kCueStaffType:
      result = "cueStaffType";
      break;
    case msrStaffDetails::kEditorialStaffType:
      result = "editorialStaffType";
      break;
    case msrStaffDetails::kAlternateStaffType:
      result = "alternateStaffType";
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
      result = "showFretsNumbers";
      break;
    case msrStaffDetails::kShowFretsLetters:
      result = "showFretsLetters";
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
      result = "printSpacingYes";
      break;
    case msrStaffDetails::kPrintSpacingNo:
      result = "printSpacingNo";
      break;
  } // switch

  return result;
}

string msrStaffDetails::asShortString () const
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
    msrPrintObjectKindAsString (fPrintObjectKind) <<
    ", printSpacingKind = " <<
    printSpacingKindAsString (fPrintSpacingKind);

  return s.str ();
}

void msrStaffDetails::print (ostream& os) const
{
  os <<
    "StaffDetails" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 17;

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
    os << endl;

    list<S_msrStaffTuning>::const_iterator
      iBegin = fStaffTuningsList.begin (),
      iEnd   = fStaffTuningsList.end (),
      i      = iBegin;

    ++gIndenter;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
    os << endl;

    --gIndenter;
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
    msrPrintObjectKindAsString (fPrintObjectKind) <<
    endl <<
    setw (fieldWidth) <<
    "printSpacingKind" << " : " <<
    printSpacingKindAsString (fPrintSpacingKind) <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrStaffDetails& elt)
{
  elt->print (os);
  return os;
}


}
