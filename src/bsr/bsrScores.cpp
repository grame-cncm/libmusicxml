/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // for 'setw()'

#include "utilities.h"

#include "bsrMutualDependencies.h"

#include "bsrScores.h"

#include "bsr2BrailleOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrScore bsrScore::create (
  int            inputLineNumber,
  S_msrScore     mScore)
{
  bsrScore* o = new bsrScore (
    inputLineNumber, mScore);
  assert(o!=0);
  return o;
}

bsrScore::bsrScore (
  int            inputLineNumber,
  S_msrScore     mScore)
    : bsrElement (inputLineNumber)
{
  fMsrScore = mScore;

  // create a transcription note
  fTranscriptionNotes =
    bsrTranscriptionNotes::create (
      inputLineNumber);

  fBrailleLineLength = gGlobalBsr2BrailleOah->fCellsPerLine;
  fBraillePageLength = gGlobalBsr2BrailleOah->fLinesPerPage;
}

bsrScore::~bsrScore ()
{}

void bsrScore::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrScore::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;

#ifdef TRACE_OAH
        if (gGlobalBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrScore::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrScore::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrScore::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrScore>*
    p =
      dynamic_cast<visitor<S_bsrScore>*> (v)) {
        S_bsrScore elem = this;

#ifdef TRACE_OAH
        if (gGlobalBsrOah->fTraceBsrVisitors) {
          gLogOstream <<
            "% ==> Launching bsrScore::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrScore::browseData (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% ==> bsrScore::browseData ()" <<
      endl;
  }
#endif

  if (fTranscriptionNotes) {
    // browse the transcription notes
    msrBrowser<bsrTranscriptionNotes> browser (v);
    browser.browse (*fTranscriptionNotes);
  }

  for (
    list<S_bsrPage>::const_iterator i =
      fScorePagesList.begin ();
    i != fScorePagesList.end ();
    i++
  ) {
    // browse the page
    msrBrowser<bsrPage> browser (v);
    browser.browse (*(*i));
  } // for

#ifdef TRACE_OAH
  if (gGlobalBsrOah->fTraceBsrVisitors) {
    gLogOstream <<
      "% <== bsrScore::browseData ()" <<
      endl;
  }
#endif
}

void bsrScore::print (ostream& os) const
{
  os <<
    "BSR Score" <<
    endl <<
    endl;

  gIndenter++;

  // print the MSR structure (without the voices)
  fMsrScore->
    printSummary (os);
  os << endl;

  // print the transcription notes if any
  const int fieldWidth = 19;

  if (fTranscriptionNotes || gGlobalBsrOah->fDisplayBsrDetails) {
    if (fTranscriptionNotes) {
      os <<
        fTranscriptionNotes;
    }
    else {
      os <<
        "TranscriptionNotes: none" <<
        endl;
    }
  }

/*
  // print the lines and pages maximum lengthes JMI
  os <<
    setw (fieldWidth) <<
    "BrailleLineLength" << " : " << fBrailleLineLength <<
    endl <<
    setw (fieldWidth) <<
    "BraillePageLength" << " : " << fBraillePageLength <<
    endl;
  os << endl;
        */

  // print the score pages if any
  int scorePagesListSize = fScorePagesList.size ();

  if (scorePagesListSize || gGlobalBsrOah->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "ScorePagesList";

    if (scorePagesListSize) {
      os << endl;
      gIndenter++;

      list<S_bsrPage>::const_iterator
        iBegin = fScorePagesList.begin (),
        iEnd   = fScorePagesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here
      } // for

      gIndenter--;
    }
    else {
      os <<
        " : " <<
         "none" <<
      endl;
    }
  }

  gIndenter--;
}

void bsrScore::printShort (ostream& os) const
{
  os <<
    "BSR Score" <<
    endl <<
    endl;

  gIndenter++;

  // print the MSR structure (without the voices)
  fMsrScore->
    printShort (os);
  os << endl;

  // print the transcription notes if any
  const int fieldWidth = 19;

  if (fTranscriptionNotes || gGlobalBsrOah->fDisplayBsrDetails) {
    if (fTranscriptionNotes) {
      os <<
        fTranscriptionNotes;
    }
    else {
      os <<
        "TranscriptionNotes: none" <<
        endl;
    }
  }

/*
  // print the lines and pages maximum lengthes JMI
  os <<
    setw (fieldWidth) <<
    "BrailleLineLength" << " : " << fBrailleLineLength <<
    endl <<
    setw (fieldWidth) <<
    "BraillePageLength" << " : " << fBraillePageLength <<
    endl;
  os << endl;
        */

  // print the score pages if any
  int scorePagesListSize = fScorePagesList.size ();

  if (scorePagesListSize || gGlobalBsrOah->fDisplayBsrDetails) {
    os <<
      setw (fieldWidth) <<
      "ScorePagesList";

    if (scorePagesListSize) {
      os << endl;
      gIndenter++;

      list<S_bsrPage>::const_iterator
        iBegin = fScorePagesList.begin (),
        iEnd   = fScorePagesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        (*i)->printShort (os);
        if (++i == iEnd) break;
        // no endl here
      } // for

      gIndenter--;
    }
    else {
      os <<
        " : " <<
         "none" <<
      endl;
    }
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrScore& score)
{
  score->print (os);
  return os;
}


}
