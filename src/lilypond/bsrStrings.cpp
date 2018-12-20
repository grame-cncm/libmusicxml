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

#include <sstream>

#include "bsrStrings.h"

#include "messagesHandling.h"

#include "bsrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
// brailling characters and strings
S_bsrCellsList brailleCharacter (
  int  inputLineNumber,
  char ch)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (inputLineNumber);
  
  switch (ch) {
    case 'a':
      result->
        appendCellKindToCellsList (kCellA);
      break;
    case 'b':
      result->
        appendCellKindToCellsList (kCellB);
      break;
    case 'c':
      result->
        appendCellKindToCellsList (kCellC);
      break;
    case 'd':
      result->
        appendCellKindToCellsList (kCellD);
      break;
    case 'e':
      result->
        appendCellKindToCellsList (kCellE);
      break;
    case 'f':
      result->
        appendCellKindToCellsList (kCellF);
      break;
    case 'g':
      result->
        appendCellKindToCellsList (kCellG);
      break;
    case 'h':
      result->
        appendCellKindToCellsList (kCellH);
      break;
    case 'i':
      result->
        appendCellKindToCellsList (kCellI);
      break;
    case 'j':
      result->
        appendCellKindToCellsList (kCellJ);
      break;
    case 'k':
      result->
        appendCellKindToCellsList (kCellK);
      break;
    case 'l':
      result->
        appendCellKindToCellsList (kCellL);
      break;
    case 'm':
      result->
        appendCellKindToCellsList (kCellM);
      break;
    case 'n':
      result->
        appendCellKindToCellsList (kCellN);
      break;
    case 'o':
      result->
        appendCellKindToCellsList (kCellO);
      break;
    case 'p':
      result->
        appendCellKindToCellsList (kCellP);
      break;
    case 'q':
      result->
        appendCellKindToCellsList (kCellQ);
      break;
    case 'r':
      result->
        appendCellKindToCellsList (kCellR);
      break;
    case 's':
      result->
        appendCellKindToCellsList (kCellS);
      break;
    case 't':
      result->
        appendCellKindToCellsList (kCellT);
      break;
    case 'u':
      result->
        appendCellKindToCellsList (kCellU);
      break;
    case 'v':
      result->
        appendCellKindToCellsList (kCellV);
      break;
    case 'w':
      result->
        appendCellKindToCellsList (kCellW);
      break;
    case 'x':
      result->
        appendCellKindToCellsList (kCellX);
      break;
    case 'y':
      result->
        appendCellKindToCellsList (kCellY);
      break;
    case 'z':
      result->
        appendCellKindToCellsList (kCellZ);
      break;

    case '(': // JMI literary???
      result->
        appendCellKindToCellsList (kCellParenthesis);
      break;
    case ')':
      result->
        appendCellKindToCellsList (kCellParenthesis);
      break;
      

    default:
      result->
        appendCellKindToCellsList (kCellQuestionMark);
  } // switch

  return result;
}

S_bsrCellsList brailleString (
  int    inputLineNumber,
  string str)
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        inputLineNumber, kCellWordSign);

  if (str.size ()) {  
    string::const_iterator
      iBegin = str.begin (),
      iEnd   = str.end (),
      i      = iBegin;
    for ( ; ; ) {
      char ch = (*i);

      result->
        appendCellsListToCellsList (
          brailleCharacter (
            inputLineNumber, ch));
          
      if (++i == iEnd) break;
    } // for
  }

  return result;
}

//______________________________________________________________________________
S_bsrString bsrString::create (
  int    inputLineNumber,
  string stringContents)
{
  bsrString* o =
    new bsrString (
      inputLineNumber, stringContents);
  assert (o!=0);
  return o;
}

bsrString::bsrString (
  int    inputLineNumber,
  string stringContents)
    : bsrLineElement (inputLineNumber)
{
  fStringContents = stringContents;

  fStringCellsList =
    brailleString (
      inputLineNumber, fStringContents);
}

bsrString::~bsrString ()
{}

void bsrString::acceptIn (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrString::acceptIn ()" <<
      endl;
  }
#endif
      
  if (visitor<S_bsrString>*
    p =
      dynamic_cast<visitor<S_bsrString>*> (v)) {
        S_bsrString elem = this;
        
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrString::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

int bsrString::fetchCellsNumber () const
{
  return fStringCellsList->fetchCellsNumber ();
}

void bsrString::acceptOut (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrString::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrString>*
    p =
      dynamic_cast<visitor<S_bsrString>*> (v)) {
        S_bsrString elem = this;
      
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrString::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrString::browseData (basevisitor* v)
{}

string bsrString::asString () const
{
  stringstream s;

  s <<
    "String" <<
    ", stringContents: " << fStringContents <<
    ", stringCellsList: " << fStringCellsList <<
    ", spacesBefore: " << fSpacesBefore <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrString::print (ostream& os)
{
  os <<
    "String" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 16;

  os <<
    setw (fieldWidth) <<
    "stringContents" << " : " << fStringContents <<
    endl <<
    setw (fieldWidth) <<
    "stringCellsList" << " : " << fStringCellsList <<
    endl <<
    setw (fieldWidth) <<
    "spacesBefore" << " : " << fSpacesBefore <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrString& elt)
{
  elt->print (os);
  return os;
}


}
