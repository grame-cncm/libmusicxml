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

#include "bsrTempos.h"

#include "bsrNotes.h"

#include "messagesHandling.h"

#include "bsrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrTempo bsrTempo::create (
  int        inputLineNumber,
  S_msrTempo mTempo)
{
  bsrTempo* o =
    new bsrTempo (
      inputLineNumber, mTempo);
  assert (o!=0);
  return o;
}

bsrTempo::bsrTempo (
  int        inputLineNumber,
  S_msrTempo mTempo)
    : bsrLineElement (inputLineNumber)
{
  fMsrTempo  = mTempo;

  fTempoCellsList = asCellsList ();
}

bsrTempo::~bsrTempo ()
{}

S_bsrCellsList bsrTempo::asCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber, kCellWordSign);

  switch (fMsrTempo->getTempoKind ()) {
    case msrTempo::k_NoTempoKind:
      break;
      
    case msrTempo::kTempoBeatUnitsPerMinute:
      {
        // fetch MSR tempo attributes
        const list<S_msrWords>&
          tempoWordsList =
            fMsrTempo->getTempoWordsList ();
  
        msrDottedDuration
          tempoDottedDuration =
            fMsrTempo->getTempoBeatUnit ();

        // handle tempo words
        int tempoWordsListSize = tempoWordsList.size ();
  
        if (tempoWordsListSize) {
          list<S_msrWords>::const_iterator
            iBegin = tempoWordsList.begin (),
            iEnd   = tempoWordsList.end (),
            i      = iBegin;
            
          for ( ; ; ) {
            S_msrWords words = (*i);
      
//            fLilypondCodeIOstream <<
   //           "\"" << words->getWordsContents () << "\"";
              
            if (++i == iEnd) break;
            
     //       fLilypondCodeIOstream <<
     //         " ";
          } // for
        }

        // handle tempo duration and continuation dots number
        msrDurationKind
          durationKind =
            tempoDottedDuration.getDurationKind ();
        int
          dotsNumber =
            tempoDottedDuration.getDotsNumber ();

        // determine the note sign
        bsrNote::bsrNoteValueKind
          noteValueKind = bsrNote::kNoteKindNone;

        switch (durationKind) {
          case k_NoDuration:
            break;
      
          case k1024th:
            break;
          case k512th:
            break;
          case k256th:
            noteValueKind = bsrNote::kNoteKindC256th;
            break;
          case k128th:
            noteValueKind = bsrNote::kNoteKindC128th;
            break;
          case k64th:
            noteValueKind = bsrNote::kNoteKindC64th;
            break;
          case k32nd:
            noteValueKind = bsrNote::kNoteKindC32nd;
            break;
          case k16th:
            noteValueKind = bsrNote::kNoteKindC16th;
            break;
          case kEighth:
            noteValueKind = bsrNote::kNoteKindC8th;
            break;
          case kQuarter:
            noteValueKind = bsrNote::kNoteKindCQuarter;
            break;
          case kHalf:
            noteValueKind = bsrNote::kNoteKindCHalf;
            break;
          case kWhole:
            noteValueKind = bsrNote::kNoteKindCWhole;
            break;
          case kBreve:
            noteValueKind = bsrNote::kNoteKindCBreve;
            break;
          case kLong:
            break;
          case kMaxima:
            break;
        } // switch

        // create a note to represent the duration
        S_bsrNote
          bNote =
            bsrNote::create (
              fInputLineNumber,
              noteValueKind,
              dotsNumber,
              bsrNote::kNoteOctaveNone,
              bsrNote::kNoteOctaveIsNeededNo);

        // append its cells to result
        result->
          appendCellsListToCellsList (
            bNote->getNoteCellsList ());
            
        // handle per minute value
        string
          tempoPerMinute =
            fMsrTempo->getTempoPerMinute ();
            
            
/*
        fLilypondCodeIOstream <<
          dottedDurationAsLilypondStringWithoutBackSlash (
            inputLineNumber,
            tempoBeatUnit) <<
          endl <<
          "\" = \"" <<
          endl <<
          tempoPerMinute <<
          ")" <<
          endl;
          */
      }
      break;
      
    case msrTempo::kTempoBeatUnitsEquivalence:
      break;
      
    case msrTempo::kTempoNotesRelationShip:
      break;
  } // switch
  
  return result;
}

int bsrTempo::fetchCellsNumber() const
{
  return asCellsList ()->fetchCellsNumber();
}

void bsrTempo::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTempo::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_bsrTempo>*
    p =
      dynamic_cast<visitor<S_bsrTempo>*> (v)) {
        S_bsrTempo elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTempo::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrTempo::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrTempo::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrTempo>*
    p =
      dynamic_cast<visitor<S_bsrTempo>*> (v)) {
        S_bsrTempo elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrTempo::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrTempo::browseData (basevisitor* v)
{}

string bsrTempo::asString () const
{
  stringstream s;

  s <<
    "Tempo" << 
    ", tempoKind: " <<
    msrTempo::tempoKindAsString (fMsrTempo->getTempoKind ()) <<
    ", tempoCellsList: " <<
    fTempoCellsList->asShortString () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void bsrTempo::print (ostream& os)
{
  os <<
    "Tempo" << 
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 23;

  os <<
    setw (fieldWidth) <<
    "tempoKind" << " : " <<
    msrTempo::tempoKindAsString (fMsrTempo->getTempoKind ()) <<
    endl <<
    setw (fieldWidth) <<
    "noteCellsList" << " : " <<
    fTempoCellsList->asShortString () <<
    endl;
        
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_bsrTempo& elt)
{
  elt->print (os);
  return os;
}


}
