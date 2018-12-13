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
#include <iomanip> // for 'setw()'

#include "bsrTranscriptionNotes.h"

#include "utilities.h"

#include "bsrOptions.h"
#include "brailleOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrTranscriptionNotesElement bsrTranscriptionNotesElement::create (
  int    inputLineNumber,
  string transcriptionNoteText)
{
  bsrTranscriptionNotesElement* o =
    new bsrTranscriptionNotesElement (
      inputLineNumber, transcriptionNoteText);
  assert(o!=0);
  return o;
}

bsrTranscriptionNotesElement::bsrTranscriptionNotesElement (
  int    inputLineNumber,
  string transcriptionNoteText)
    : bsrElement (inputLineNumber)
{
  fTranscriptionNoteText = transcriptionNoteText;

  fTranscriptionNotesElementCellsList =
    bsrCellsList::create (inputLineNumber);
}

bsrTranscriptionNotesElement::~bsrTranscriptionNotesElement ()
{}

void bsrTranscriptionNotesElement::acceptIn (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTranscriptionNotesElement::acceptIn ()" <<
      endl;
  }
#endif
      
  if (visitor<S_bsrTranscriptionNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotesElement>*> (v)) {
        S_bsrTranscriptionNotesElement elem = this;
        
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTranscriptionNotesElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTranscriptionNotesElement::acceptOut (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTranscriptionNotesElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTranscriptionNotesElement>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotesElement>*> (v)) {
        S_bsrTranscriptionNotesElement elem = this;
      
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTranscriptionNotesElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTranscriptionNotesElement::browseData (basevisitor* v)
{
}

string bsrTranscriptionNotesElement::asString () const
{
  stringstream s;

  s <<
    "TranscriptionNotesElement" <<
    // JMI ", fTranscriptionNoteText: " <<
    ", \"" << fTranscriptionNoteText << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrTranscriptionNotesElement::print (ostream& os)
{
  os <<
    "TranscriptionNotesElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;
  
  os <<
    "transcriptionNoteText " << " : \"" << fTranscriptionNoteText << "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrTranscriptionNotesElement& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrTranscriptionNotes bsrTranscriptionNotes::create (
  int inputLineNumber)
{
  bsrTranscriptionNotes* o =
    new bsrTranscriptionNotes (
      inputLineNumber);
  assert(o!=0);
  return o;
}

bsrTranscriptionNotes::bsrTranscriptionNotes (
  int inputLineNumber)
    : bsrLine (
        inputLineNumber,
        0, // JMI ???
        gBrailleOptions->fCellsPerLine)
{}

bsrTranscriptionNotes::~bsrTranscriptionNotes ()
{}

void bsrTranscriptionNotes::acceptIn (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTranscriptionNotes::acceptIn ()" <<
      endl;
  }
#endif
      
  if (visitor<S_bsrTranscriptionNotes>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotes>*> (v)) {
        S_bsrTranscriptionNotes elem = this;
        
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTranscriptionNotes::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrTranscriptionNotes::acceptOut (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTranscriptionNotes::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrTranscriptionNotes>*
    p =
      dynamic_cast<visitor<S_bsrTranscriptionNotes>*> (v)) {
        S_bsrTranscriptionNotes elem = this;
      
#ifdef TRACE_OPTIONS
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTranscriptionNotes::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrTranscriptionNotes::browseData (basevisitor* v)
{
  for (
    list<S_bsrTranscriptionNotesElement>::const_iterator i =
      fTranscriptionNotesElementsList.begin ();
    i != fTranscriptionNotesElementsList.end ();
    i++ ) {
    // browse the element
    bsrBrowser<bsrTranscriptionNotesElement> browser (v);
    browser.browse (*(*i));
  } // for
}

void bsrTranscriptionNotes::print (ostream& os)
{
  os <<
    "TranscriptionNotes" <<
    endl;

  gIndenter++;
  
  // print the notes if any
  const int fieldWidth = 19;

  int transcriptionNotesElementsListSize = fTranscriptionNotesElementsList.size ();
  
  if (transcriptionNotesElementsListSize || gBsrOptions->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "transcriptionNotesElementsList" << " : " <<
      endl;
      
    if (transcriptionNotesElementsListSize) {
      gIndenter++;
  
      list<S_bsrTranscriptionNotesElement>::const_iterator
        iBegin = fTranscriptionNotesElementsList.begin (),
        iEnd   = fTranscriptionNotesElementsList.end (),
        i      = iBegin;
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
      os <<
        "none" <<
      endl;
    }
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrTranscriptionNotes& elt)
{
  elt->print (os);
  return os;
}


}
