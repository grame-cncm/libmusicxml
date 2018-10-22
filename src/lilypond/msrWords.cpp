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

#include "msrWords.h"

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

//______________________________________________________________________________
S_msrWords msrWords::create (
  int                      inputLineNumber,
  msrPlacementKind         wordsPlacementKind,
  string                   wordsContents,
  msrJustifyKind           wordsJustifyKind,
  msrVerticalAlignmentKind wordsVerticalAlignmentKind,
  msrFontStyleKind         wordsFontStyleKind,
  S_msrFontSize            wordsFontSize,
  msrFontWeightKind        wordsFontWeightKind,
  msrWordsXMLLangKind      wordsXMLLangKind)
{
  msrWords* o =
    new msrWords (
      inputLineNumber,
      wordsPlacementKind,
      wordsContents,
      wordsJustifyKind,
      wordsVerticalAlignmentKind,
      wordsFontStyleKind,
      wordsFontSize,
      wordsFontWeightKind,
      wordsXMLLangKind);
  assert(o!=0);
  return o;
}

msrWords::msrWords (
  int                      inputLineNumber,
  msrPlacementKind         wordsPlacementKind,
  string                   wordsContents,
  msrJustifyKind           wordsJustifyKind,
  msrVerticalAlignmentKind wordsVerticalAlignmentKind,
  msrFontStyleKind         wordsFontStyleKind,
  S_msrFontSize            wordsFontSize,
  msrFontWeightKind        wordsFontWeightKind,
  msrWordsXMLLangKind      wordsXMLLangKind)
    : msrElement (inputLineNumber)
{
  fWordsPlacementKind  = wordsPlacementKind;
  
  fWordsContents       = wordsContents;
  
  fWordsJustifyKind    = wordsJustifyKind;
  fWordsVerticalAlignmentKind = wordsVerticalAlignmentKind;

  fWordsFontStyleKind  = wordsFontStyleKind;
  fWordsFontSize       = wordsFontSize;
  fWordsFontWeightKind = wordsFontWeightKind;
  
  fWordsXMLLangKind    = wordsXMLLangKind;
}

msrWords::~msrWords ()
{}

void msrWords::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrWords::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrWords::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrWords::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrWords::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrWords>*
    p =
      dynamic_cast<visitor<S_msrWords>*> (v)) {
        S_msrWords elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrWords::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrWords::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrWords& elt)
{
  elt->print (os);
  return os;
}

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

string msrWords::msrWordsXMLLangKindAsString (
  msrWordsXMLLangKind wordsXMLLangKind)
{
  string result;

  switch (wordsXMLLangKind) {
    case kItLang:
      result = "it";
      break;
    case kEnLang:
      result = "en";
      break;
    case kDeLang:
      result = "de";
      break;
    case kFrLang:
      result = "fr";
    case kJaLang:
      result = "ja";
      break;
    case kLaLang:
      result = "la";
      break;
  } // switch

  return result;
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

void msrWords::print (ostream& os)
{
// JMI  os << asString () << endl;

  os <<
    "Words" <<
    endl;

  gIndenter++;

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "wordsContents" << " = \"" << fWordsContents << "\"" <<
    endl <<
    setw (fieldWidth) <<
    "placement" << " = " <<
    msrPlacementKindAsString (fWordsPlacementKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsJustifyKind" << " = " <<
    msrJustifyKindAsString (fWordsJustifyKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsVerticalAlignmentKind" << " = " <<
    msrVerticalAlignmentKindAsString (fWordsVerticalAlignmentKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontStyle" << " = " <<
    msrFontStyleKindAsString (fWordsFontStyleKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontSize" << " = " <<
    fWordsFontSize->fontSizeAsString () <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontWeight" << " = " <<
    msrFontWeightKindAsString (fWordsFontWeightKind) <<
    endl <<
    setw (fieldWidth) <<
    "wordsFontXMLLang" << " = " <<
    msrWordsXMLLangKindAsString (fWordsXMLLangKind) <<
    endl;
  
  gIndenter--;
}


}
