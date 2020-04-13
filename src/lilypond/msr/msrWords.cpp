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

#include "msrWords.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrWords msrWords::create (
  int                        inputLineNumber,
  msrPlacementKind           wordsPlacementKind,
  string                     wordsContents,
  msrJustifyKind             wordsJustifyKind,
  msrHorizontalAlignmentKind wordsHorizontalAlignmentKind,
  msrVerticalAlignmentKind   wordsVerticalAlignmentKind,
  msrFontStyleKind           wordsFontStyleKind,
  S_msrFontSize              wordsFontSize,
  msrFontWeightKind          wordsFontWeightKind,
  msrXMLLangKind             wordsXMLLangKind,
  int                        wordsStaffNumber)
{
  msrWords* o =
    new msrWords (
      inputLineNumber,
      wordsPlacementKind,
      wordsContents,
      wordsJustifyKind,
      wordsHorizontalAlignmentKind,
      wordsVerticalAlignmentKind,
      wordsFontStyleKind,
      wordsFontSize,
      wordsFontWeightKind,
      wordsXMLLangKind,
      wordsStaffNumber);
  assert(o!=0);
  return o;
}

msrWords::msrWords (
  int                        inputLineNumber,
  msrPlacementKind           wordsPlacementKind,
  string                     wordsContents,
  msrJustifyKind             wordsJustifyKind,
  msrHorizontalAlignmentKind wordsHorizontalAlignmentKind,
  msrVerticalAlignmentKind   wordsVerticalAlignmentKind,
  msrFontStyleKind           wordsFontStyleKind,
  S_msrFontSize              wordsFontSize,
  msrFontWeightKind          wordsFontWeightKind,
  msrXMLLangKind             wordsXMLLangKind,
  int                        wordsStaffNumber)
    : msrElement (inputLineNumber)
{
  fWordsPlacementKind  = wordsPlacementKind;

  fWordsContents = wordsContents;

  fWordsJustifyKind = wordsJustifyKind;

  fWordsHorizontalAlignmentKind = wordsHorizontalAlignmentKind;
  fWordsVerticalAlignmentKind   = wordsVerticalAlignmentKind;

  fWordsFontStyleKind  = wordsFontStyleKind;
  fWordsFontSize       = wordsFontSize;
  fWordsFontWeightKind = wordsFontWeightKind;

  fWordsXMLLangKind = wordsXMLLangKind;

  fWordsStaffNumber = wordsStaffNumber;
}

msrWords::~msrWords ()
{}

void msrWords::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrWords::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrWords::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrWords::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrWords::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrWords::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrWords::browseData (basevisitor* v)
{}

string msrWords::wordsPlacementKindAsString () const
{
  return
    msrPlacementKindAsString (
      fWordsPlacementKind);
}

string msrWords::wordsJustifyKindAsString () const
{
  return msrJustifyKindAsString (fWordsJustifyKind);
}

string msrWords::wordsHorizontalAlignmentKindAsString () const
{
  return
    msrHorizontalAlignmentKindAsString (
      fWordsHorizontalAlignmentKind);
}

string msrWords::wordsVerticalAlignmentKindAsString () const
{
  return
    msrVerticalAlignmentKindAsString (
      fWordsVerticalAlignmentKind);
}

string msrWords::wordsFontStyleKindAsString () const
{
  return msrFontStyleKindAsString (fWordsFontStyleKind);
}

string msrWords::wordsFontSizeAsString () const
{
  return fWordsFontSize->fontSizeAsString ();
}

string msrWords::wordsFontWeightKindAsString () const
{
  return msrFontWeightKindAsString (fWordsFontWeightKind);
}

string msrWords::asString () const
{
  stringstream s;

  s <<
    "Words" << " " <<
    fWordsContents << ", placement = " <<
    msrPlacementKindAsString (fWordsPlacementKind);

  return s.str ();
}

void msrWords::print (ostream& os) const
{
// JMI  os << asString () << endl;

  os <<
    "Words" <<
    endl;

  gIndenter++;

  const int fieldWidth = 29;

  os << left <<
    setw (fieldWidth) <<
    "wordsContents" << " : \"" << fWordsContents << "\"" <<
    endl <<

    setw (fieldWidth) <<
    "placement" << " : " <<
    msrPlacementKindAsString (fWordsPlacementKind) <<
    endl <<

    setw (fieldWidth) <<
    "wordsJustifyKind" << " : " <<
    msrJustifyKindAsString (fWordsJustifyKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsHorizontalAlignmentKind" << " : " <<
    msrHorizontalAlignmentKindAsString (fWordsHorizontalAlignmentKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsVerticalAlignmentKind" << " : " <<
    msrVerticalAlignmentKindAsString (fWordsVerticalAlignmentKind) <<
    endl <<

    setw (fieldWidth) <<
    "wordsFontStyle" << " : " <<
    msrFontStyleKindAsString (fWordsFontStyleKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontSize" << " : " <<
    fWordsFontSize->fontSizeAsString () <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontWeight" << " : " <<
    msrFontWeightKindAsString (fWordsFontWeightKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontXMLLang" << " : " <<
    msrXMLLangKindAsString (fWordsXMLLangKind) <<
    endl <<

    setw (fieldWidth) <<
    "wordsStaffNumber" << " : " <<
    fWordsStaffNumber <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrWords& elt)
{
  elt->print (os);
  return os;
}


}
